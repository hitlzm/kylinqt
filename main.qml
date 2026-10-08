import QtQuick 2.12
import QtQuick.Window 2.12
import "./components"
import taoQuick 1.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Window {
    id: root
    visible: true
    width: Screen.width
    height: Screen.height
    title: qsTr("kylin-qt")

    // ═══ 启动登录门禁 ═══
    // 主界面放在 Loader 里延迟实例化：校验通过前视频播放器、串口界面等都不创建，
    // 串口/手柄线程也由 C++ 侧推迟到 startupRequested 才启动（见 main.cpp）。
    Loader {
        id: appLoader
        anchors.fill: parent
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
        anchors.fill: parent
        z: 1000
        active: !auth.authenticated
        sourceComponent: Component { LoginPage { } }
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
