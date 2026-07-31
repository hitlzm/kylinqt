import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    property int mywidth: 600
    property int myheight: 480
    width: mywidth
    height: myheight
    color: '#e9f0f9'
    property int stacklayoutindex: 0
    //消息提示框
    MessagePopup {
        id: msg
    }
    // 顶部栏：标题 + 按钮行
    RowLayout {
        id: ctrlrow
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 20

        Text {
            text: "转台控制区"
            font.pixelSize: 24
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }

        CusButton_Blue {
            text: "开机"
            font.pixelSize: 18
            Layout.fillWidth: true
            height: 60
            onClicked: {
                turntableSendData.reqopenTurntable()
            }
        }
        CusButton_Blue {
            text: "停机"
            font.pixelSize: 18
            Layout.fillWidth: true
            height: 60
            onClicked: {
                turntableSendData.reqcloseTurntable()
            }
        }
        CusButton_Blue {
            text: "回零"
            font.pixelSize: 18
            Layout.fillWidth: true
            height: 60
            onClicked: {
                turntableSendData.reqzeroTurntable()
            }
        }
        CusButton_Blue {
            text: "复位"
            font.pixelSize: 18
            Layout.fillWidth: true
            height: 60
            onClicked: {
                turntableSendData.reqresetTurntable()
            }
        }
    }

    //加入串口扫描与串口连接
    RowLayout {
        id: serialctrlrow
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: ctrlrow.bottom
        anchors.topMargin: 10
        spacing: 20

        // 串口选择 - 绑定到 C++ imageSerial 对象
        CusComboBox {
            id: serialComboBox
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            
            model: turntableData.availablePorts   //和Imageport使用同一个串口列表即可
        }
         // 波特率
        CusComboBox {
            id: baudComboBox
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            model: ["230400"]
        }
        //扫描串口按钮
        CusButton_Blue {
            id: scanButton
            Layout.fillWidth: true
            height: 32
            text: "扫描串口"

            onClicked: {
                turntableData.requestScanPorts()
            }
        }
       
        CusButton_Blue {
        id: openButton
        Layout.fillWidth: true
        height: 32
        text: turntableData.portOpen ? "关闭串口" : "打开串口"

        Timer {
            id: delayTimer
            interval: 1  
            onTriggered: {
                if (turntableData.portOpen) {msg.message = "串口已打开！"; msg.open()}
                else { msg.message = "串口打开失败！"; msg.open() }
            }
        }
        onClicked: {
            if (turntableData.portOpen) {
                turntableData.closePort()
            } else {
                turntableData.openPort(serialComboBox.currentText,
                                    parseInt(baudComboBox.currentText))
                delayTimer.start()
            }
        }
    }


    }

    StackLayout {
        id: stack
        currentIndex: stacklayoutindex
        width: root.width - 20
        anchors.top: serialctrlrow.bottom
        anchors.topMargin: 10
        anchors.left: root.left
        anchors.leftMargin: 10
        anchors.right: root.right
        anchors.rightMargin: 10
        anchors.bottom: root.bottom
        anchors.bottomMargin: 10
        Repeater {
            model: ["ExGuide.qml", "ProgramMode.qml", "Remotectlmode.qml"]
            Loader {
                asynchronous: true
                source: modelData
                active: stack.currentIndex === index
            }
        }
    }
    // StackLayout {
    //         id: stack
    //         width: root.width - 20
    //         height: 280
    //         currentIndex: stacklayoutindex
    //         anchors.top: statusGroup.bottom
    //         anchors.topMargin: 5
    //         anchors.left:root.left
    //         anchors.leftMargin: 10
    //         anchors.right: root.right
    //         anchors.rightMargin: 10 
    //         // --- 第一个页面 ---
    //         Item {
    //             ExGuide {
    //                 anchors.fill: parent   
    //             }
    //         }
    //         // --- 第二个页面 ---
    //         Item {
    //             ProgramMode{
    //                 anchors.fill: parent
    //             }
    //         }
    //         Item {
    //             Remotectlmode{
    //                 anchors.fill: parent
    //             }
    //         }
    //     }
  
}



