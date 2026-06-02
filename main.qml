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

    // 右侧面板宽度：屏幕宽度的 35%，限制在 400~900 之间
    property int panelWidth: Math.max(400, Math.min(900, Math.round(width * 0.35)))

    // ── 左侧 Tab 区（填满右侧面板之外的剩余空间）──
    Item {
        id: dyt
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: panelWidth + 5

        Column {
            anchors.fill: parent

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

            StackLayout {
                id: stack
                width: parent.width
                height: parent.height - bar.height
                currentIndex: bar.currentIndex

                Item {
                    ImageArea { anchors.fill: parent }
                }
                Item {
                    LaserArea { anchors.fill: parent }
                }
            }
        }
    }

    // ── 右侧面板区 ──
    Myvideo2 {
        id: myvideo
        width: panelWidth
        anchors.top: parent.top
        anchors.right: parent.right
    }
    Modeselect {
        id: modeselect
        height: 50
        anchors.top: myvideo.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        width: panelWidth
    }
    TurnTablestatus {
        id: turntablestatus
        anchors.top: modeselect.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: panelWidth
        mywidth: panelWidth
    }
}
