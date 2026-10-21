import QtQuick 2.12
import "."
Item {
    id: root
    width: 1200
    height: 950

    ImageSendArea {
        id: sendArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    ImageRecvArea {
        id: recvArea
        anchors.top: sendArea.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    // 与右侧导引头视频及转台控制区之间的分隔线
    Rectangle {
        id: rightSeparator
        width: 1
        color: "black"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        z: 100
    }

}