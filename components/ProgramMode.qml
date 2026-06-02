import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12

Item{
id:root
property var controlNames: [
    "三轴控制",
    "内框控制",
    "中框控制",
    "外框控制"
]
ColumnLayout{
    anchors.fill: parent
    spacing: 10
    RowLayout{
        spacing: 10
        Layout.alignment: Qt.AlignLeft

        // 占位：对齐下方控制名列（width: 120）
        Item { Layout.preferredWidth: 80 }

        Text {
            text: "起点"
            font.pixelSize: 20
            font.bold: true
            color: "#000000"
            Layout.preferredWidth: 150
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            text: "终点"
            font.pixelSize: 20
            font.bold: true
            color: "#000000"
            Layout.preferredWidth: 150
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            text: "运动时间"
            font.pixelSize: 20
            font.bold: true
            color: "#000000"
            Layout.preferredWidth: 150
            horizontalAlignment: Text.AlignHCenter
        }
        // 占位：对齐下方按钮列（Layout.fillWidth: true）
        Item { Layout.fillWidth: true }
    }

    Repeater {
        model: root.controlNames
        delegate: RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignLeft

            Text {
                text: controlNames[index]
                font.pixelSize: 20
                font.bold: true
                color: '#000000'
                Layout.preferredWidth: 80
            }

            CusTextField{
                id:input1
                // width:150
                Layout.preferredHeight: 40
                Layout.preferredWidth: 150
            }
            CusTextField{
                id:input2
                // width:150
                Layout.preferredHeight: 40
                Layout.preferredWidth: 150  
            }
            CusTextField{
                id:input3
                // width:150
                Layout.preferredHeight: 40
                Layout.preferredWidth: 150
            }
            CusButton_Blue {
            id: sendButton
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            text: "发送数据"

            onClicked: {
            // imageSerial.sendData(imageSerial.imageSendData.buildFrame())
            // console.log("图像导引头数据已发送")
            }
        }

            
        }
    }
}
}