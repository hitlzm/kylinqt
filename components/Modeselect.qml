import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Item {
    id: root

    GroupBox {
        anchors.fill: parent
        background: Rectangle { color: "transparent"; border.color: "gray"; border.width: 4; radius: 8 }
        Row {
            anchors.centerIn: parent
            spacing: 10
            Text { text: qsTr("控制模式选择"); font.pixelSize: 20; font.bold: true; color: 'black'; anchors.verticalCenter: parent.verticalCenter }
            CusRadioButton { width: 160; text: qsTr("外引导模式"); font.pixelSize: 20 }
            CusRadioButton { width: 160; text: qsTr("程控模式"); font.pixelSize: 20 }
            CusRadioButton { width: 160; text: qsTr("遥控模式"); font.pixelSize: 20 }
        }
    }
}
