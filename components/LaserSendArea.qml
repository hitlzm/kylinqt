// LaserSendArea.qml
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    width: 900
    height: 620
    color: "#dce6f0"

    property var commandList: [
        "控制指令",
        "无动作",
        "自检",
        "锁定",
        "参数装订",
        "定轴搜索",
        "矩形搜索",
        "圆形搜索",
        "定轴位置",
        "矩形位置",
        "圆形位置"
    ]

    Text {
        id: titleText
        text: "激光导引头控制区"
        font.pixelSize: 24
        font.bold: true
        color: "#000000"

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    // 左侧命令区
    Rectangle {
        id: commandArea
        width: 140
        height: 520
        color: "#8d8d8d"
        radius: 4

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: titleText.bottom
        anchors.topMargin: 20

        Column {
            spacing: 6
            anchors.fill: parent
            anchors.margins: 12

            Repeater {
                model: root.commandList

                delegate: RadioButton {
                    text: modelData

                    font.pixelSize: 16

                    indicator.width: 18
                    indicator.height: 18

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 16
                        color: "#000000"
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 30
                    }
                }
            }
        }
    }

    // 串口选择
    ComboBox {
        id: serialComboBox
        width: 220
        height: 42

        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: commandArea.right
        anchors.leftMargin: 80

        model: ["COM1", "COM2", "COM3", "COM4"]
    }

    // 波特率
    ComboBox {
        id: baudComboBox
        width: 220
        height: 42

        anchors.top: serialComboBox.top
        anchors.left: serialComboBox.right
        anchors.leftMargin: 80

        model: ["9600", "115200", "230400", "460800"]
    }

    
    // 打开串口按钮
    Button {
        id: openButton
        width: 160
        height: 50
        text: "打开串口"

        anchors.top: serialComboBox.bottom
        anchors.topMargin: 60
        anchors.horizontalCenter: serialComboBox.horizontalCenter
    }

    // 发送数据按钮
    Button {
        id: sendButton
        width: 160
        height: 50
        text: "发送数据"

        anchors.top: baudComboBox.bottom
        anchors.topMargin: 60
        anchors.horizontalCenter: baudComboBox.horizontalCenter
    }

    // 左侧输入框
    Column {
        id: leftInputColumn
        spacing: 20

        anchors.top: openButton.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: openButton.horizontalCenter

        Repeater {
            model: 5

            delegate: TextField {
                width: 220
                height: 40
                placeholderText: "请输入内容"
            }
        }
    }

    // 右侧输入框
    Column {
        id: rightInputColumn
        spacing: 20

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: sendButton.horizontalCenter

        Repeater {
            model: 5

            delegate: TextField {
                width: 220
                height: 40
                placeholderText: "请输入内容"
            }
        }
    }
}