import QtQuick 2.12
import QtQuick.Controls 2.12

// 自检指示灯：正常 = 绿色圆点，异常 = 红色圆点
Item {
    id: root
    height: 40
    width: rowLayout.implicitWidth

    property bool normal: true          // true=正常(绿), false=异常(红)
    property string label: ""           // 标签文字
    property int fontSize: 18

    Row {
        id: rowLayout
        spacing: 6
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: statusDot
            width: 10
            height: 10
            radius: 5
            color: root.normal ? "#4CAF50" : "#F44336"
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            text: root.label
            font.pixelSize: root.fontSize
            color: root.normal ? "#333333" : "#d93025"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
