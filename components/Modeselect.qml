import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Item{
    id: root
    property int myindex: 0
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
    CusRadioButton {
        id: radio1
        width: 160
        text: qsTr("外引导模式")
        font.pixelSize: 20
        onCheckedChanged: {
            if (checked) {
                root.myindex = 0
            }
        }
    }
    CusRadioButton {
        id: radio2
        width: 160
        text: qsTr("程控模式")
        font.pixelSize: 20
        onCheckedChanged: {
            if (checked) {
                root.myindex = 1
            }
        }
    }
    CusRadioButton {
        id: radio3
        width: 160
        text: qsTr("遥控模式")
        font.pixelSize: 20
        onCheckedChanged: {
            if (checked) {
                root.myindex = 2
            }
        }
    }
}

}
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
