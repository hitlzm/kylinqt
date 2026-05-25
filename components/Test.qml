import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"
Rectangle {
    id: root
    width: 1100
    height: 650
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
        text: "图像导引头发送区"
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
        anchors.leftMargin: 10
        // anchors.top: titleText.bottom
        // anchors.topMargin: 20
        anchors.verticalCenter: root.verticalCenter

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

        anchors.top: titleText.bottom
        anchors.topMargin: 20
        anchors.left: commandArea.right
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
        // anchors.horizontalCenter: serialComboBox.horizontalCenter

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
        // anchors.horizontalCenter: baudComboBox.horizontalCenter

        onClicked: {
            var data = ""
            root.sendData(data)
        }
    }

    // 左侧输入框
    Column {
        id: leftInputColumn
        spacing: 30

        anchors.top: serialComboBox.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: serialComboBox.horizontalCenter

        Repeater {
            model: [
                "激光周期",
                "方位角度",
                "俯仰角度",
                "搜索范围/半径 ",
                "搜索范围",
                "预留参数 1",
                "预留参数 2",
                "预留参数 3",
                
            ]

            delegate: MyTextField {
                mywidth: 100
                myheight: 30
                title: modelData
            }
        }
    }

    // 右侧输入框
    Column {
        id: rightInputColumn
        spacing: 30

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: leftInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: 8
            
            delegate: MyTextField {
                mywidth: 100
                myheight: 30
                title: "预留参数 " + (index + 1)
            }
        }
    }

    Column {
        id: thirdInputColumn
        spacing: 30

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: rightInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: 8
            
            delegate: MyTextField {
                mywidth: 100
                myheight: 30
                title: "预留参数 " + (index + 1)
            }
        }
    }

    Column {
        id:fourthInputColumn
        spacing: 30

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: thirdInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: 8
            
            delegate: MyTextField {
                mywidth: 100
                myheight: 30
                title: "预留参数 " + (index + 1)
            }
        }
    }
    Column {
        id:mInputColumn5
        spacing: 30

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: fourthInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: 8
            
            delegate: MyTextField {
                mywidth: 100
                myheight: 30
                title: "预留参数 " + (index + 1)
            }
        }
    }
     Column {
        id:mInputColumn6
        spacing: 30

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: mInputColumn5.right
        anchors.leftMargin: 30
        Repeater {
            model: 8
            
            delegate: MyTextField {
                mywidth: 100
                myheight: 30
                title: "预留参数 " + (index + 1)
            }
        }
    }
    // // 发送数据信号
    // signal sendData(string data)

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
