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

    Item {
    id:dyt
    width: 1200
    height: 800

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

Myvideo {
    id: myvideo
    anchors.top:parent.top
    anchors.left:dyt.right
    anchors.leftMargin: 5
}
Modeselect {
    id: modeselect
    height:60
    anchors.top: myvideo.bottom
    anchors.left: dyt.right
    anchors.leftMargin: 50
    anchors.topMargin: 5    
    anchors.right: myvideo.right
}

TurnTablestatus {
    id: turntablestatus
    anchors.top: modeselect.bottom
    anchors.left: dyt.right
    anchors.leftMargin: 5
    anchors.topMargin: 5    
    anchors.right: myvideo.right
}

}