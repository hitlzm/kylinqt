import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Item{
    id: root
    property int myindex: -1
    GroupBox {
        background: Rectangle {
        color: "transparent"
        border.color: "gray"
        border.width: 4
        radius: 8
    }
    Row {
        spacing: 10   // 统一控制间距
        Text {
            id: text1
            text: qsTr("控制模式选择")
            font.pixelSize: 20
            font.bold: true
            color: 'black'
            anchors.verticalCenter: parent.verticalCenter
        }
        ButtonGroup { id: modeGroup }
    CusRadioButton {
        id: radio1
        width: 160
        text: qsTr("外引导模式")
        font.pixelSize: 20
        checked: false
        ButtonGroup.group: modeGroup
        onCheckedChanged: {
            if (checked) {
                root.myindex = 0
                modeController.setCurrentMode(0)
                modeController.modeChanged(0)
            }
        }
    }
    CusRadioButton {
        id: radio2
        width: 160
        text: qsTr("程控模式")
        font.pixelSize: 20
        checked: false
        ButtonGroup.group: modeGroup
        onCheckedChanged: {
            if (checked) {
                root.myindex = 1
                modeController.setCurrentMode(1)
                modeController.modeChanged(1)
            }
        }
    }
    CusRadioButton {
        id: radio3
        width: 160
        text: qsTr("遥控模式")
        font.pixelSize: 20
        checked: false
        ButtonGroup.group: modeGroup
        onCheckedChanged: {
            if (checked) {
                root.myindex = 2
                modeController.setCurrentMode(2)
                modeController.modeChanged(2)
            }
        }
    }
}

}
}

