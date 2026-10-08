import QtQuick 2.9
import QtQuick.Controls 2.2
Item {
    id: root
    property alias containsMouse: mouseArea.containsMouse
    signal posChange(int xOffset, int yOffset)
    implicitWidth: 12
    implicitHeight: 12
    property int posType: Qt.ArrowCursor

    //5.10之前, qml是不能定义枚举的，用只读的int属性代替一下。
    readonly property int posLeftTop: Qt.SizeFDiagCursor
    readonly property int posLeft: Qt.SizeHorCursor
    readonly property int posLeftBottom: Qt.SizeBDiagCursor
    readonly property int posTop: Qt.SizeVerCursor
    readonly property int posBottom: Qt.SizeVerCursor
    readonly property int posRightTop: Qt.SizeBDiagCursor
    readonly property int posRight: Qt.SizeHorCursor
    readonly property int posRightBottom: Qt.SizeFDiagCursor
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        property int lastX: 0
        property int lastY: 0
        onContainsMouseChanged: {
            if (containsMouse) {
                cursorShape = posType;
            } else {
                cursorShape = Qt.ArrowCursor;
            }
        }
        onPressedChanged: {
            if (containsPress) {
                lastX = mouseX;
                lastY = mouseY;
            }
        }
        onPositionChanged: {
            if (pressed) {
                posChange(mouseX - lastX, mouseY - lastY)
                // 必须把基准点推到当前位置：消费方（CusResizeBorder 等）用的是
                // control.x/width += offset 这种累加写法，需要的是“每次移动的增量”，
                // 而不是“从按下开始的累计位移”，否则窗口会越拖越快。
                lastX = mouseX
                lastY = mouseY
            }
        }
    }
}
