import QtQuick 2.12
import VlcVideo 1.0

// 放大镜固定在主窗口的场景图中，避免第二个 QQuickWindow 创建独立的
// OpenGL context 和渲染表面。窗口显示/隐藏只切换 visible，不销毁资源。
Item {
    id: overlay

    property var frameSource: null
    property string titleText: "放大"
    property bool overlayActive: false
    property real sizeScale: 1.0

    width: parent ? Math.max(320, parent.width * 0.40 * sizeScale) : 800
    height: parent ? Math.max(240, parent.height * 0.40 * sizeScale) : 500
    x: parent ? (parent.width - width) / 2 : 0
    y: parent ? (parent.height - height) / 2 : 0
    visible: false
    z: overlayActive ? 1100 : 1000
    clip: true

    signal activated()

    function activate() {
        if (!overlayActive) {
            overlayActive = true
            activated()
        }
    }

    function deactivate() {
        overlayActive = false
    }

    function ensureWithinBounds() {
        if (!parent)
            return
        x = Math.max(0, Math.min(x, parent.width - width))
        y = Math.max(0, Math.min(y, parent.height - height))
    }

    onVisibleChanged: {
        if (visible) {
            activate()
            ensureWithinBounds()
            videoFrame.update()
        }
    }

    onWidthChanged: ensureWithinBounds()
    onHeightChanged: ensureWithinBounds()

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

    // 标题只用于区分视频来源，整个画面区域均可按住拖动。
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
        onPressed: overlay.activate()
        onWheel: {
            if (!overlay.parent || wheel.angleDelta.y === 0)
                return

            const oldCenterX = overlay.x + overlay.width / 2
            const oldCenterY = overlay.y + overlay.height / 2
            const step = wheel.angleDelta.y > 0 ? 1.10 : 0.90
            overlay.sizeScale = Math.max(0.50, Math.min(2.00,
                                                        overlay.sizeScale * step))
            // 围绕当前放大窗中心缩放，避免每次滚轮都向同一角扩大。
            overlay.x = oldCenterX - overlay.width / 2
            overlay.y = oldCenterY - overlay.height / 2
            overlay.ensureWithinBounds()
            wheel.accepted = true
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 8
        anchors.topMargin: 8
        width: titleLabel.implicitWidth + 16
        height: 28
        radius: 5
        color: "#CC333333"
        z: 10

        Text {
            id: titleLabel
            anchors.centerIn: parent
            text: overlay.titleText
            color: "#FFFFFF"
            font.pixelSize: 14
            font.bold: true
        }
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
            onClicked: {
                overlay.visible = false
                overlay.activate()
            }
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
