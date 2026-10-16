import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Item{
    id: root
    property int myindex: -1

    // 外引导通信中断（导引头帧编号停止更新）时，C++ 侧已经退出外引导模式，
    // 这里同步取消界面上的「外引导模式」单选：互斥组置为 Unchecked 会取消
    // 当前选中项，避免界面仍显示外引导模式而实际已停止外引导。
    function exitExGuideMode() {
        if (radio1.checked) {
            modeGroup.checkState = Qt.Unchecked
            root.myindex = -1
        }
    }

    Connections {
        target: modeController
        onExguideAborted: root.exitExGuideMode()
    }

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
            }
        }
    }
}

}
}

