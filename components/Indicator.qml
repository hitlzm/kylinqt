import QtQuick 2.12
import QtQuick.Controls 2.12

Item{
property string arg1:"默认"
property string arg2:"默认"
Row {
            spacing: 6
            Layout.alignment: Qt.AlignVCenter
            Rectangle {
                id: statusDot
                width: 12
                height: 12
                radius: 6
                color: vlcplayer.playing ? "#4CAF50" : "#F44336"
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: vlcplayer.playing ? arg1 : arg2
                font.pixelSize: 16
                color: "#333333"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
}