import QtQuick 2.12
import QtQuick.Window 2.12
import "./components"
import taoQuick 1.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

// 使用 ApplicationWindow（Window 的子类）：它会自动创建 QQC2 的 Overlay，
// 使 parent: Overlay.overlay 的弹窗（MessagePopup / MsgPopup2）能正常显示。
ApplicationWindow {
    id: root
    visible: true
    // 用“可用桌面区域”而不是整块屏幕尺寸：
    // Screen.width/height 是整块屏幕，会把底部的系统任务栏/面板一起盖住；
    // 而最大化时窗口管理器会自动避开任务栏，两者观感就对不上了。
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    title: qsTr("kylin-qt")

    // ═══ 无边框 + 自绘标题栏 ═══
    // 去掉系统标题栏后，最左侧的应用图标、原生最小化/最大化/关闭按钮都不再绘制，
    // 全部改由下面的 CusTitleBar 提供（标题居中、无图标、底色与主界面一致）。
    // ApplicationWindow 仍然会自动创建 Overlay，不影响 parent: Overlay.overlay 的弹窗。
    flags: Qt.Window | Qt.FramelessWindowHint
    // 窗口自身的底色，拖动/缩放瞬间新露出的区域不会闪白边
    color: "#e9f0f9"

    CusTitleBar {
        id: titleBar
        width: parent.width
        height: 40
        targetWindow: root
        title: root.title
    }

    // ═══ 启动登录门禁 ═══
    // 主界面放在 Loader 里延迟实例化：校验通过前视频播放器不创建（也就不会开始取流），
    // 视频/串口线程由 C++ 侧推迟到 startupRequested 才启动（见 main.cpp）。
    Loader {
        id: appLoader
        // 内容整体下移一个标题栏的高度，避开自绘标题栏
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        active: false
        sourceComponent: appContent
    }

    Component {
        id: appContent

        Item {
            anchors.fill: parent

            //自制标题栏
            Item {
                id:dyt
                width: 1200
                height: 980

                Column {
                    anchors.fill: parent

                    // ===== TabBar =====
                    TabBar {
                        id: bar
                        width: parent.width

                        TabButton {
                            text: qsTr("图像导引头")
                            font.pixelSize: CusConfig.fontPixel + 6
                        }

                        TabButton {
                            text: qsTr("激光导引头")
                            font.pixelSize: CusConfig.fontPixel + 6
                        }
                    }
                    // ===== 页面区域 =====
                    StackLayout {
                        id: stack
                        width: parent.width
                        height: parent.height - bar.height
                        currentIndex: bar.currentIndex
                        // --- 第一个页面 ---
                        Item {
                            ImageArea {
                                anchors.fill: parent
                            }
                        }
                        // --- 第二个页面 ---
                        Item {
                            LaserArea{
                                anchors.fill: parent
                            }
                        }
                    }
                }
            }

            Myvideo5 {
                id: myvideo
                anchors.top:parent.top
                anchors.left:dyt.right
                anchors.leftMargin: 5
            }
            Modeselect {
                id: modeselect
                height:50
                anchors.top: myvideo.bottom
                anchors.left: dyt.right
                anchors.leftMargin: 40
                anchors.topMargin: 5
                anchors.right: myvideo.right
            }

            TurnTablestatus {
                id: turntablestatus
                anchors.top: modeselect.bottom
                anchors.topMargin: 5
                anchors.left: dyt.right
                anchors.leftMargin: 5
                anchors.right: myvideo.right
                // 高度与左侧 ImageRecvArea 下边沿对齐
                // myvideo(430) + margin(5) + modeselect(50) + margin(5) = 490
                // 左侧内容下边沿 = bar.height + ImageSendArea(610) + ImageRecvArea(310) = bar.height + 920
                // turntablestatus 高度 = bar.height + 920 - 490 = bar.height + 430
                myheight: bar.height + 610 + 310 - 430 - 5 - 50 - 5
                stacklayoutindex: modeselect.myindex
            }
        }
    }

    // 外引导源串口未打开时，弹窗提示
    // 刻意留在窗口层（不放进上面的 Component）：弹窗不依赖登录，保持原有行为
    MessagePopup {
        id: serialWarnMsg
    }

    Connections {
        target: laserData
        onPopupMessage: { serialWarnMsg.message = msg; serialWarnMsg.open() }
    }
    Connections {
        target: imageData
        onPopupMessage: { serialWarnMsg.message = msg; serialWarnMsg.open() }
    }
    Connections {
        target: ccdData
        onPopupMessage: { serialWarnMsg.message = msg; serialWarnMsg.open() }
    }

    // 登录页：校验通过后由 active 绑定自动销毁
    Loader {
        id: loginLoader
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        z: 1000
        active: !auth.authenticated
        sourceComponent: Component { LoginPage { } }
    }

    // 八向缩放边框（taoQuick 自带）。z 最高，所以边框条压在标题栏和内容之上：
    // 标题栏顶部 8px 属于“缩放”区，其余部分用于拖动窗口；
    // 不需要缩放能力时把它整个删掉或改成 visible: false 即可。
    // 注意：它必须排在弹窗（parent: Overlay.overlay）之下也没关系——
    // Overlay 的 z 是 1000001，弹窗打开时仍然盖在缩放边框之上。
    CusResizeBorder {
        anchors.fill: parent
        control: root
        borderWidth: 8
        z: 2000
        visible: !titleBar.maximized
    }

    Connections {
        target: auth
        onLoginSucceeded: {
            // 顺序固定：先实例化主界面，再让 C++ 启动视频/串口线程
            appLoader.active = true
            auth.beginStartup()
        }
    }
}
