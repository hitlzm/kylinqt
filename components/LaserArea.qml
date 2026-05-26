import QtQuick 2.12
import "."
Item {
    id: root
    width: 1200
    height: 800

    LaserSendArea {
        id: sendArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    LaserRecvArea {
        id: recvArea
        anchors.top: sendArea.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}