import QtQuick 2.12
import QtQuick.Window 2.12
import VlcVideo 1.0

Window {
    id: magnifierWindow
    width: Screen.width * 0.65
    height: Screen.height * 0.65
    title: "视频放大镜"
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "#000000"
    visible: true

    property var frameSource: null

    VlcFrame {
        id: magnifier
        anchors.fill: parent
        source: frameSource
        zoomFactor: 1.0
    }

    // ── 定时器驱动渲染循环（~60fps）──
    Timer {
        id: renderTimer
        interval: 16
        running: true
        repeat: true
        onTriggered: magnifier.update()
    }

    // ── 窗口拖动 ──
    MouseArea {
        id: dragArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        property point lastMousePos: Qt.point(0, 0)
        onPressed: {
            lastMousePos = Qt.point(mouseX, mouseY)
        }
        onPositionChanged: {
            var dx = mouseX - lastMousePos.x
            var dy = mouseY - lastMousePos.y
            magnifierWindow.x += dx
            magnifierWindow.y += dy
        }
    }

    // ── 关闭按钮 ──
    Rectangle {
        anchors.right: parent.right;  anchors.rightMargin: 8
        anchors.top: parent.top;       anchors.topMargin: 8
        width: 32; height: 32; radius: 16
        color: "#CC333333"
        z: 10

        Text {
            anchors.centerIn: parent
            text: "✕"
            color: "#FFFFFF"
            font.pixelSize: 20; font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: magnifierWindow.close()
        }
    }

    // ── 窗口关闭时清理 ──
    onClosing: {
        renderTimer.stop()
    }
}
