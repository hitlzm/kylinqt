import QtQuick 2.12
import QtQuick.Window 2.12

/*!
    无边框窗口用的自绘标题栏。

    只强制要求 targetWindow，其余属性都有默认值。

    行为：
      * 左侧不绘制应用图标，标题相对整条标题栏水平居中；
      * 按住标题栏空白处拖动窗口，双击切换最大化 / 还原；
      * 最大化状态下拖动会自动还原，光标停留在标题栏上的原相对位置；
      * 右侧依次是最小化 / 最大化(还原) / 关闭三个按钮。

    前提：targetWindow 已用 Qt.FramelessWindowHint 去掉系统标题栏，
    否则自绘标题栏会和原生标题栏叠在一起。
*/
Rectangle {
    id: root

    // ── 对外接口 ───────────────────────────────────────────────
    /// 要控制的顶层窗口，写成 Window 的 id 即可
    property var targetWindow: null
    property alias title: titleText.text
    property alias titlePixelSize: titleText.font.pixelSize
    property alias titleBold: titleText.font.bold

    /// 标题栏底色，默认取主界面底色
    property color barColor: "#e9f0f9"
    /// 标题文字颜色
    property color textColor: "#303133"
    /// 按钮悬浮 / 按下的底色
    property color hoverColor: Qt.rgba(0, 0, 0, 0.08)
    property color pressColor: Qt.rgba(0, 0, 0, 0.16)
    /// 关闭按钮悬浮 / 按下时的底色（沿用 Windows 的红）
    property color closeHoverColor: "#e81123"
    property color closePressColor: "#c50f1f"
    property int buttonWidth: 46
    property int iconSize: 12

    implicitHeight: 40
    color: barColor

    /// 最大化前的窗口几何，用于还原
    property rect normalGeometry: Qt.rect(0, 0, 0, 0)

    readonly property bool maximized: targetWindow !== null
        && (targetWindow.visibility === Window.Maximized
            || targetWindow.visibility === Window.FullScreen)

    // ── 窗口控制 ───────────────────────────────────────────────
    function toggleMaximized() {
        if (!targetWindow)
            return
        if (maximized) {
            targetWindow.showNormal()
            if (normalGeometry.width > 0) {
                targetWindow.x = normalGeometry.x
                targetWindow.y = normalGeometry.y
                targetWindow.width = normalGeometry.width
                targetWindow.height = normalGeometry.height
            }
        } else {
            normalGeometry = Qt.rect(targetWindow.x, targetWindow.y,
                                     targetWindow.width, targetWindow.height)
            targetWindow.showMaximized()
        }
    }

    /// 从最大化状态拖出：globalX/globalY 是光标全局坐标，
    /// localX/localY 是光标在标题栏内的坐标
    function restoreFromMaximized(globalX, globalY, localX, localY) {
        if (!targetWindow || !maximized)
            return
        var w = normalGeometry.width > 0 ? normalGeometry.width : targetWindow.width * 0.7
        var h = normalGeometry.height > 0 ? normalGeometry.height : targetWindow.height * 0.7
        var ratio = width > 0 ? localX / width : 0.5
        targetWindow.showNormal()
        targetWindow.width = w
        targetWindow.height = h
        targetWindow.x = Math.round(globalX - ratio * w)
        targetWindow.y = Math.round(globalY - localY)
    }

    // ── 标题：锚在整条标题栏正中，左侧不放图标 ────────────────
    Text {
        id: titleText
        anchors.centerIn: parent
        color: root.textColor
        font.pixelSize: 16
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        // 两侧各留出一个按钮组宽度，长标题不会被右侧按钮压住
        width: Math.max(0, Math.min(implicitWidth, root.width - 2 * buttonRow.width))
    }

    // ── 拖动区：除右侧按钮外的整条标题栏 ──────────────────────
    MouseArea {
        id: dragArea
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: buttonRow.left
        acceptedButtons: Qt.LeftButton

        property real pressGlobalX: 0
        property real pressGlobalY: 0
        property real winX: 0
        property real winY: 0

        onPressed: {
            var g = dragArea.mapToGlobal(mouse.x, mouse.y)
            pressGlobalX = g.x
            pressGlobalY = g.y
            winX = root.targetWindow ? root.targetWindow.x : 0
            winY = root.targetWindow ? root.targetWindow.y : 0
        }

        onPositionChanged: {
            if (!pressed || !root.targetWindow)
                return
            // 窗口自己也在移动，局部坐标会跟着变，所以位移一律用全局坐标算
            var g = dragArea.mapToGlobal(mouse.x, mouse.y)
            if (root.maximized) {
                root.restoreFromMaximized(g.x, g.y, mouse.x, mouse.y)
                pressGlobalX = g.x
                pressGlobalY = g.y
                winX = root.targetWindow.x
                winY = root.targetWindow.y
                return
            }
            root.targetWindow.x = Math.round(winX + (g.x - pressGlobalX))
            root.targetWindow.y = Math.round(winY + (g.y - pressGlobalY))
        }

        onDoubleClicked: root.toggleMaximized()
    }

    // ── 右侧按钮：最小化 / 最大化(还原) / 关闭 ────────────────
    Row {
        id: buttonRow
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        // 最小化
        Rectangle {
            width: root.buttonWidth
            height: buttonRow.height
            color: minArea.pressed ? root.pressColor
                                   : (minArea.containsMouse ? root.hoverColor : "transparent")
            Rectangle {
                anchors.centerIn: parent
                width: root.iconSize
                height: 2
                color: root.textColor
            }
            MouseArea {
                id: minArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: if (root.targetWindow) root.targetWindow.showMinimized()
            }
        }

        // 最大化 / 还原
        Rectangle {
            id: maxButton
            width: root.buttonWidth
            height: buttonRow.height
            color: maxArea.pressed ? root.pressColor
                                   : (maxArea.containsMouse ? root.hoverColor : "transparent")
            // 普通状态：一个空心方框
            Rectangle {
                anchors.centerIn: parent
                width: 10
                height: 10
                visible: !root.maximized
                color: "transparent"
                border.width: 1
                border.color: root.textColor
            }
            // 最大化状态：两个错开的方框，表示“还原”
            Rectangle {
                visible: root.maximized
                x: Math.round((maxButton.width - 10) / 2) + 2
                y: Math.round((maxButton.height - 10) / 2) - 2
                width: 10
                height: 10
                color: "transparent"
                border.width: 1
                border.color: root.textColor
            }
            Rectangle {
                visible: root.maximized
                x: Math.round((maxButton.width - 10) / 2) - 2
                y: Math.round((maxButton.height - 10) / 2) + 2
                width: 10
                height: 10
                color: root.barColor   // 盖住后面那个方框的重叠部分
                border.width: 1
                border.color: root.textColor
            }
            MouseArea {
                id: maxArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: root.toggleMaximized()
            }
        }

        // 关闭
        Rectangle {
            id: closeButton
            width: root.buttonWidth
            height: buttonRow.height
            color: closeArea.pressed ? root.closePressColor
                                     : (closeArea.containsMouse ? root.closeHoverColor : "transparent")
            readonly property color iconColor: closeArea.containsMouse ? "white" : root.textColor
            Rectangle {
                anchors.centerIn: parent
                width: root.iconSize
                height: 2
                rotation: 45
                color: closeButton.iconColor
            }
            Rectangle {
                anchors.centerIn: parent
                width: root.iconSize
                height: 2
                rotation: -45
                color: closeButton.iconColor
            }
            MouseArea {
                id: closeArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: if (root.targetWindow) root.targetWindow.close()
            }
        }
    }
}
