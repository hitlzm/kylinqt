import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Item {
    id: root
    property var controlNames: ["三轴控制","内框控制","中框控制","外框控制"]

    ColumnLayout {
        anchors.fill: parent; spacing: 10

        RowLayout {
            spacing: 10; Layout.alignment: Qt.AlignLeft
            Item { Layout.preferredWidth: 80 }
            Text { text: "起点"; font.pixelSize: 20; font.bold: true; color: "#000000"; Layout.preferredWidth: 150; horizontalAlignment: Text.AlignHCenter }
            Text { text: "终点"; font.pixelSize: 20; font.bold: true; color: "#000000"; Layout.preferredWidth: 150; horizontalAlignment: Text.AlignHCenter }
            Text { text: "运动时间"; font.pixelSize: 20; font.bold: true; color: "#000000"; Layout.preferredWidth: 150; horizontalAlignment: Text.AlignHCenter }
            Item { Layout.fillWidth: true }
        }

        Repeater {
            model: root.controlNames
            delegate: RowLayout {
                spacing: 10; Layout.alignment: Qt.AlignLeft
                Text { text: controlNames[index]; font.pixelSize: 20; font.bold: true; color: '#000000'; Layout.preferredWidth: 80 }
                CusTextField { Layout.preferredHeight: 40; Layout.preferredWidth: 150 }
                CusTextField { Layout.preferredHeight: 40; Layout.preferredWidth: 150 }
                CusTextField { Layout.preferredHeight: 40; Layout.preferredWidth: 150 }
                CusButton_Blue { text: "发送数据"; Layout.fillWidth: true; Layout.preferredHeight: 40; onClicked: {} }
            }
        }
    }
}
