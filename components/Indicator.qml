import QtQuick 2.12
import QtQuick.Controls 2.12

// 自检指示灯：正常 = 绿色圆点，异常 = 红色圆点
Item {
    id: root
    height: 40
    width: rowLayout.implicitWidth
    //property都有其对应的监听槽函数
    property bool normal: true          // true=正常(绿), false=异常(红)
    property string label: ""           // 标签文字
    property int fontSize: 18

    // 由绿变红（正常→故障）时触发
    signal faultTriggered(string label)

    // 防止初始化时误触发
    property bool _ready: false
    Component.onCompleted: _ready = true

    onNormalChanged: {
        if (_ready && !normal) {
            faultTriggered(label)
        }
    }

    Row {
        id: rowLayout
        spacing: 6
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: statusDot
            width: 10
            height: 10
            radius: 5
            color: root.normal ? "#4CAF50" : "#f44747"
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            text: root.label
            font.pixelSize: root.fontSize
            color: root.normal ? "#c0c0c0" : "#f44747"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
