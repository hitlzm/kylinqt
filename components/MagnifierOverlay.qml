import QtQuick 2.12
import VlcVideo 1.0

// 放大镜固定在主窗口的场景图中，避免第二个 QQuickWindow 创建独立的
// OpenGL context 和渲染表面。窗口显示/隐藏只切换 visible，不销毁资源。
Item {
    id: overlay

    property var frameSource: null

    width: parent ? Math.max(320, parent.width * 0.65) : 800
    height: parent ? Math.max(240, parent.height * 0.65) : 500
    x: parent ? (parent.width - width) / 2 : 0
    y: parent ? (parent.height - height) / 2 : 0
    visible: false
    z: 1000
    clip: true

    function ensureWithinBounds() {
        if (!parent)
            return
        x = Math.max(0, Math.min(x, parent.width - width))
        y = Math.max(0, Math.min(y, parent.height - height))
    }

    onVisibleChanged: {
        if (visible) {
            ensureWithinBounds()
            videoFrame.update()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#000000"
        border.color: "#707070"
        border.width: 2
        radius: 6
    }

    VlcFrame {
        id: videoFrame
        anchors.fill: parent
        source: overlay.frameSource
        zoomFactor: 1.0
    }

    // 不显示标题栏，整个画面区域均可按住拖动。
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.SizeAllCursor
        acceptedButtons: Qt.LeftButton
        drag.target: overlay
        drag.axis: Drag.XAndYAxis
        drag.minimumX: 0
        drag.maximumX: Math.max(0, overlay.parent.width - overlay.width)
        drag.minimumY: 0
        drag.maximumY: Math.max(0, overlay.parent.height - overlay.height)
    }

    Rectangle {
        width: 32
        height: 32
        radius: 16
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 8
        anchors.topMargin: 8
        color: closeArea.pressed ? "#CC666666" : "#CC333333"
        z: 10

        Text {
            anchors.centerIn: parent
            text: "✕"
            color: "#FFFFFF"
            font.pixelSize: 20
            font.bold: true
        }

        MouseArea {
            id: closeArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: overlay.visible = false
        }
    }

    // 只在视频真正渲染出新帧时重绘，不再以固定 60 Hz 空跑。
    Connections {
        target: overlay.frameSource
        onFrameRendered: {
            if (overlay.visible)
                videoFrame.update()
        }
        onStopped: overlay.visible = false
    }

    Connections {
        target: overlay.parent
        onWidthChanged: overlay.ensureWithinBounds()
        onHeightChanged: overlay.ensureWithinBounds()
    }
}
