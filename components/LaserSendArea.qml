import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"
Rectangle {
    id: root
    width: 1200
    height: 540
    color: '#e9f0f9'

    property var commandList: [

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

    property int currentCmd: 0

    Text {
        id: titleText
        text: "激光导引头发送区"
        font.pixelSize: 24
        font.bold: true
        color: "#000000"

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    // 左侧命令区
    Rectangle {
        id: commandArea
        width: 140
        height: 400
        color: '#faf7f7'
        radius: 4

        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: titleText.bottom
        anchors.topMargin: 20

        Column {
            spacing: 6
            anchors.fill: parent
            anchors.margins: 12
            Text {
                text: "控制指令"
                font.pixelSize: 16
                font.bold: true
                color: "#000000"
            }
            Repeater {
                model: root.commandList

                delegate: CusRadioButton {
                    text: modelData
                    checked: index === root.currentCmd
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

                    onClicked: root.currentCmd = index
                }
            }
        }
    }

    // 串口选择 - 绑定到 C++ laserSerial 对象
    CusComboBox {
        id: serialComboBox
        width: 220
        height: 42

        anchors.top: titleText.top
        anchors.left: titleText.right
        anchors.leftMargin: 30

        model: laserSerial.availablePorts
    }

    // 波特率
    CusComboBox {
        id: baudComboBox
        width: 220
        height: 42

        anchors.top: serialComboBox.top
        anchors.left: serialComboBox.right
        anchors.leftMargin: 30

        model: ["9600", "115200", "230400", "460800"]
    }

    // 打开串口按钮
    CusButton_Blue {
        id: openButton
        width: 120
        height: 42
        text: laserSerial.portOpen ? "关闭串口" : "打开串口"

        anchors.top: serialComboBox.top
        anchors.left: baudComboBox.right
        anchors.leftMargin: 30
        onClicked: {
            if (laserSerial.portOpen) {
                laserSerial.closePort()
            } else {
                laserSerial.openPort(serialComboBox.currentText,
                                     parseInt(baudComboBox.currentText))
            }
        }
    }

    // 发送数据按钮
    CusButton_Blue {
        id: sendButton
        width: 120
        height: 42
        text: "发送数据"
        enabled: laserSerial.portOpen

        anchors.top: serialComboBox.top
        anchors.left: openButton.right
        anchors.leftMargin: 30

        onClicked: {
            var data = ""
            root.sendData(data)
        }
    }

    // 左侧输入框
    Column {
        id: firstColumn
        spacing: 30

        anchors.top: serialComboBox.bottom
        anchors.topMargin: 30
        anchors.left:commandArea.right
        anchors.leftMargin: 120

        Repeater {
            model: [
                "激光周期",
                "方位角度",
            ]

            delegate: MyTextField {
                mywidth: 120
                myheight: 40
                title: modelData
            }
        }
    }

    // 右侧输入框
    Column {
        id: secondColumn
        spacing: 30

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: firstColumn.right
        anchors.leftMargin: 30

        Repeater {
            model: [
                "俯仰角度",
                "搜索中心方位角度",
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 40
                title: modelData
            }
        }
    }

    Column {
        id: thirdColumn
        spacing: 30

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: secondColumn.right
        anchors.leftMargin: 30

        Repeater {
            model: [
                "搜索中心俯仰角度",
                "方位搜索范围",
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 40
                title: modelData
            }
        }
    }

    Column {
        id: fourthColumn
        spacing: 30

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: thirdColumn.right
        anchors.leftMargin: 30

        Repeater {
            model: [
                "俯仰搜索范围",
                "搜索半径",
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 40
                title: modelData
            }
        }
    }

    // 发送数据信号
    signal sendData(string data)

    // 连接到 laserSerial 信号
    Connections {
        target: laserSerial
        function onErrorOccurred(msg) {
            console.log("Serial error:", msg)
        }
        function onConnected() {
            console.log("Serial connected")
        }
        function onDisconnected() {
            console.log("Serial disconnected")
        }
        
    }

    
}
