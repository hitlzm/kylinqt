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
        //以下指令发送三拍，三拍后恢复默认值
        "自检",
        "射检",
        "搜索",
        "预置",
        "发射指令",
        "通讯检查",
        "解锁",
        "软件升级",
        "软件升级结束",
        "打开波门叠加",
        "关闭波门叠加"
    
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
        width: 160
        height: 500
        color: '#faf7f7'
        radius: 4

        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: titleText.bottom
        anchors.topMargin: 20
        // anchors.verticalCenter: root.verticalCenter

        Column {
            spacing: 6
            anchors.fill: parent
            anchors.margins: 12
            
            Text {
                text: "导引头控制字"
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

    // 串口选择 - 绑定到 C++ imageSerial 对象
    CusComboBox {
        id: serialComboBox
        width: 120
        height: 32

        anchors.top: titleText.top
        anchors.left: titleText.right
        anchors.leftMargin: 30

        model: imageSerial.availablePorts
    }

    // 波特率
    CusComboBox {
        id: baudComboBox
        width: 120
        height: 32

        anchors.top: serialComboBox.top
        anchors.left: serialComboBox.right
        anchors.leftMargin: 30

        model: ["9600", "115200", "230400", "460800"]
    }

    // 打开串口按钮
    CusButton_Blue {
        id: openButton
        width: 120
        height: 32
        text: imageSerial.portOpen ? "关闭串口" : "打开串口"

        anchors.top: serialComboBox.top
        anchors.left: baudComboBox.right
        anchors.leftMargin: 30 
        // anchors.horizontalCenter: serialComboBox.horizontalCenter

        onClicked: {
            if (imageSerial.portOpen) {
                imageSerial.closePort()
            } else {
                imageSerial.openPort(serialComboBox.currentText,
                                     parseInt(baudComboBox.currentText))
            }
        }
    }

    // 发送数据按钮
    CusButton_Blue {
        id: sendButton
        width: 120
        height: 32
        text: "发送数据"
        enabled: imageSerial.portOpen

        anchors.top: serialComboBox.top
        anchors.left: openButton.right
        anchors.leftMargin: 30
        // anchors.horizontalCenter: baudComboBox.horizontalCenter

        onClicked: {
            var data = ""
            root.sendData(data)
        }
    }

    CusButton_Blue {
        id: photobutton
        width: 120
        height: 32
        text: "拍摄参考图"
        anchors.top: serialComboBox.top
        anchors.left: sendButton.right
        anchors.leftMargin: 30
        // anchors.horizontalCenter: baudComboBox.horizontalCenter
        onClicked: {
            // var data = ""
            // root.sendData(data)
        }
    }

    MySwitch {
        id: mySwitch
        width: 120
        height: 32
        title: "字符叠加"
        anchors.top: photobutton.top
        anchors.left: photobutton.right
        anchors.leftMargin: 30
    }

    // 左侧输入框
    Column {
        id: leftInputColumn
        spacing: 5

        anchors.top: serialComboBox.bottom
        anchors.topMargin: 10
        anchors.left:serialComboBox.left

        Repeater {

            model: [
            { title: "弹目距离", unit: "m" },
            { title: "导弹速度",  unit: "m/s" },
            { title: "弹体俯仰角",  unit: "°" },
            { title: "弹体方位角",  unit: "°" },
            { title: "弹体滚转角",  unit: "°" },
            { title: "弹体俯仰角速度",  unit: "°/s" },
            { title: "修正图像帧序号",  unit: "" }
            
            ]

            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
            }
        }
    }

    // 右侧输入框
    Column {
        id: rightInputColumn
        spacing: 5

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: leftInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: [
            { title: "弹体方位角速度",  unit: "°/s" },
            { title: "弹体滚转角速度", unit: "m/s" },
            { title: "弹体X向速度",  unit: "m/s" },
            { title: "弹体Y向速度",  unit: "m/s" },
            { title: "弹体Z向速度",  unit: "m/s" },
            { title: "弹体X坐标",  unit: "m" }
            
            ]
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
            }
        }
    }

    Column {
        id: thirdInputColumn
        spacing: 5

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: rightInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: [
            { title: "弹体Y坐标",  unit: "m" },
            { title: "弹体Z坐标",  unit: "m" },
            { title: "俯仰框架角预装", unit: "°" },
            { title: "偏航框架角预装",  unit: "°" },
            { title: "红外积分时间",  unit: "ms" },
            { title: "修正后俯仰跟踪位置",  unit: "" }
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
            }
        }
    }

    Column {
        id:fourthInputColumn
        spacing: 5

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: thirdInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            
            model: [
            { title: "修正后偏航跟踪位置",  unit: "" },
            { title: "搜索俯仰角速度",  unit: "°/s" },
            { title: "搜索方位角速度",  unit: "°/s" },
            { title: "目标海拔高度",  unit: "m" },
            { title: "飞机俯仰角",  unit: "°" },
            { title: "飞机方位角",  unit: "°" },
            ]
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
            }
        }
    }
    Column {
        id:mInputColumn5
        spacing: 5

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: fourthInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: [
            { title: "飞机滚转角",  unit: "°" },
            { title: "焦距",  unit: "mm" },
            { title: "吊舱俯仰框架角",  unit: "°" },
            { title: "吊舱方位框架角",  unit: "°" },
            { title: "卫星图比例",  unit: "" },
            { title: "吊舱类型",  unit: "" }
            
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
            }
        }
    }
     Column {
        id:mInputColumn6
        spacing: 5

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: mInputColumn5.right
        anchors.leftMargin: 30
        Repeater {
            model: [
            { title: "目标经度",  unit: "°" },
            { title: "目标纬度",  unit: "°" },
            { title: "飞机经度",  unit: "°" },
            { title: "飞机纬度",  unit: "°" },
            { title: "飞机海拔高度",  unit: "m" },
            { title: "像元尺寸",  unit: "μm" }
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
            }
        }
    }
    // // 发送数据信号
    // signal sendData(string data)

    // 连接到 imageSerial 信号
    Connections {
        target: imageSerial
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
