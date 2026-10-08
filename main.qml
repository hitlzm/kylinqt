import QtQuick 2.12
import QtQuick.Window 2.12
import "./components"
import taoQuick 1.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Window {
    id: root
    visible: true
    // 用“可用桌面区域”而不是整块屏幕尺寸：
    // Screen.width/height 是整块屏幕，会把底部的系统任务栏/面板一起盖住；
    // 而最大化时窗口管理器会自动避开任务栏，两者观感就对不上了。
    // desktopAvailable* 取的是窗口管理器预留之后剩下的区域，所以窗口化时也留着任务栏。
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    title: qsTr("kylin-qt")

    // ═══ 无边框 + 自绘标题栏 ═══
    // 去掉系统标题栏后，最左侧的应用图标、原生最小化/最大化/关闭按钮都不再绘制，
    // 全部改由下面的 CusTitleBar 提供（标题居中、无图标、底色与主界面一致）。
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
    // 主界面放在 Loader 里延迟实例化：校验通过前视频播放器、串口界面等都不创建，
    // 串口/手柄线程也由 C++ 侧推迟到 startupRequested 才启动（见 main.cpp）。
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
                            font.pixelSize: 20
                        }

                        TabButton {
                            text: qsTr("激光导引头")
                            font.pixelSize: 20
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
            // 顺序固定：先实例化主界面，再让 C++ 启动串口/手柄线程
            appLoader.active = true
            auth.beginStartup()
        }
    }
}
