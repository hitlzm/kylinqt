import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    property int mywidth: 600
    property int myheight: 800
    width: mywidth
    height: myheight
    color: '#e9f0f9'
    property int groupHeight1: 150
    property int stacklayoutindex: 0
    //消息提示框
    MessagePopup {
        id: msg
    }
    MsgPopup2{
        id:innermsg
    }
    MsgPopup2{
        id:middlemsg
    }
    MsgPopup2{
        id:outmsg
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
            model: ["115200"]
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

    // ---------- 转台状态显示区（GroupBox 直接占满剩余空间）----------
    GroupBox {
        id: statusGroup
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: serialctrlrow.bottom
        anchors.topMargin: 5
        // anchors.bottom: parent.bottom       // 填满剩余垂直空间
        // anchors.bottomMargin: 10

        // 自定义背景（原灰色背景 + 边框 + 圆角）
        background: Rectangle {
            color: "#e1d8d8"               // 灰色背景（可改为透明）
            border.color: "gray"
            border.width: 4
            radius: 8
        }


        // 内容区域：三行状态数据，整体居中显示
        Column {
            spacing: 10
            anchors.centerIn: parent

            // 第一行：时间和序号
            Row {
                spacing: 6
                DataLabel { label: "秒时间:"; value: turntableData.time;  fontSize: 18; labelWidth: 80 }
                DataLabel { label: "序号:";   value: turntableData.ctlnumber; fontSize: 18; labelWidth: 65 }
            }
            // 第二行：内框
            Row {
                spacing: 6
                DataLabel {
                    label: "内框状态:"
                    fontSize: 18; labelWidth: 90
                    property int innerstatus: turntableData.inner_statusnumber
                            onInnerstatusChanged: {
                                if (innerstatus === 0x1F) innermsg.showToast("转台内框驱动器报警")
                                else if (innerstatus === 0x20) innermsg.showToast("转台内框伺服超差报警")
                                else if (innerstatus === 0x21) innermsg.showToast("转台内框正向限位报警")
                                else if (innerstatus === 0x22) innermsg.showToast("转台内框逆向限位报警")
                                else if (innerstatus === 0x23) innermsg.showToast("转台内框时钟同步报警")
                                else if (innerstatus === 0x24) innermsg.showToast("转台内框初始化信息报警")
                                else if (innerstatus === 0x25) innermsg.showToast("转台内框限位开关同时导通")
                                else if (innerstatus === 0x26) innermsg.showToast("转台内框编码器数据故障报警")
                                else if (innerstatus === 0x29) innermsg.showToast("转台内框瞬态电流报警")
                                else if (innerstatus === 0x2A) innermsg.showToast("转台内框连续电流报警")
                            }
                    value: { 
                        switch(turntableData.inner_statusnumber) {
                            case 0x01: return "伺服"
                            case 0x02: return "回零执行中"
                            case 0x03: return "位置执行中"
                            case 0x04: return "速率执行中"
                            case 0x05: return "速率稳定"
                            case 0x06: return "摇摆执行中"
                            case 0x07: return "摇摆稳定"
                            case 0x08: return "停车执行中"
                            case 0x09: return "跟踪模式1执行中"
                            case 0x0A: return "停止跟踪"
                            case 0x0B: return "跟踪模式2执行中"
                            case 0x0F: return "速度环模式执行中"
                            case 0x1F: return "驱动器报警"
                            case 0x20: return "伺服超差报警"
                            case 0x21: return "正向限位报警"
                            case 0x22: return "逆向限位报警"
                            case 0x23: return "时钟同步报警"
                            case 0x24: return "初始化信息报警"
                            case 0x25: return "限位开关同时导通"
                            case 0x26: return "编码器数据故障报警"
                            case 0x29: return "瞬态电流报警"
                            case 0x2A: return "连续电流报警"
                            default: return "未知"
                        }
                    }
                }
                DataLabel { label: "内框角度值:"; value: 
                String(turntableData.inner_angle); 
                fontSize: 18; labelWidth: 105 }
                DataLabel { label: "内框控制偏差:"; value: 
                String(turntableData.inner_ctlDeviation); 
                fontSize: 18; labelWidth: 125 }
            }
            // 第三行：中框
            Row {
                spacing: 6
                DataLabel {
                    label: "中框状态:"
                    fontSize: 18; labelWidth: 90
                    property int middlestatus: turntableData.middle_statusnumber
                            onMiddlestatusChanged: {
                                if (middlestatus === 0x1F) middlemsg.showToast("转台中框驱动器报警")
                                else if (middlestatus === 0x20) middlemsg.showToast("转台中框伺服超差报警")
                                else if (middlestatus === 0x21) middlemsg.showToast("转台中框正向限位报警")
                                else if (middlestatus === 0x22) middlemsg.showToast("转台中框逆向限位报警")
                                else if (middlestatus === 0x23) middlemsg.showToast("转台中框时钟同步报警")
                                else if (middlestatus === 0x24) middlemsg.showToast("转台中框初始化信息报警")
                                else if (middlestatus === 0x25) middlemsg.showToast("转台中框限位开关同时导通")
                                else if (middlestatus === 0x26) middlemsg.showToast("转台中框编码器数据故障报警")
                                else if (middlestatus === 0x29) middlemsg.showToast("转台中框瞬态电流报警")
                                else if (middlestatus === 0x2A) middlemsg.showToast("转台中框连续电流报警")
                            }
                    value: {
                        switch(turntableData.middle_statusnumber) {
                            case 0x01: return "伺服"
                            case 0x02: return "回零执行中"
                            case 0x03: return "位置执行中"
                            case 0x04: return "速率执行中"
                            case 0x05: return "速率稳定"
                            case 0x06: return "摇摆执行中"
                            case 0x07: return "摇摆稳定"
                            case 0x08: return "停车执行中"
                            case 0x09: return "跟踪模式1执行中"
                            case 0x0A: return "停止跟踪"
                            case 0x0B: return "跟踪模式2执行中"
                            case 0x0F: return "速度环模式执行中"
                            case 0x1F: return "驱动器报警"
                            case 0x20: return "伺服超差报警"
                            case 0x21: return "正向限位报警"
                            case 0x22: return "逆向限位报警"
                            case 0x23: return "时钟同步报警"
                            case 0x24: return "初始化信息报警"
                            case 0x25: return "限位开关同时导通"
                            case 0x26: return "编码器数据故障报警"
                            case 0x29: return "瞬态电流报警"
                            case 0x2A: return "连续电流报警"
                            default: return "未知"
                        }
                    }
                }
                DataLabel { label: "中框角度值:"; value: turntableData.middle_angle; fontSize: 18; labelWidth: 105 }
                DataLabel { label: "中框控制偏差:"; value: turntableData.middle_ctlDeviation; fontSize: 18; labelWidth: 125 }
            }
            // 第四行：外框
            Row {
                spacing: 6
                DataLabel {
                    label: "外框状态:"
                    fontSize: 18; labelWidth: 90
                    property int outterstatus: turntableData.outter_statusnumber
                            onOutterstatusChanged: {
                                if (outterstatus === 0x1F) outmsg.showToast("转台外框驱动器报警")
                                else if (outterstatus === 0x20) outmsg.showToast("转台外框伺服超差报警")
                                else if (outterstatus === 0x21) outmsg.showToast("转台外框正向限位报警")
                                else if (outterstatus === 0x22) outmsg.showToast("转台外框逆向限位报警")
                                else if (outterstatus === 0x23) outmsg.showToast("转台外框时钟同步报警")
                                else if (outterstatus === 0x24) outmsg.showToast("转台外框初始化信息报警")
                                else if (outterstatus === 0x25) outmsg.showToast("转台外框限位开关同时导通")
                                else if (outterstatus === 0x26) outmsg.showToast("转台外框编码器数据故障报警")
                                else if (outterstatus === 0x29) outmsg.showToast("转台外框瞬态电流报警")
                                else if (outterstatus === 0x2A) outmsg.showToast("转台外框连续电流报警")
                            }
                    value: {
                        switch(turntableData.outter_statusnumber) {
                            case 0x01: return "伺服"
                            case 0x02: return "回零执行中"
                            case 0x03: return "位置执行中"
                            case 0x04: return "速率执行中"
                            case 0x05: return "速率稳定"
                            case 0x06: return "摇摆执行中"
                            case 0x07: return "摇摆稳定"
                            case 0x08: return "停车执行中"
                            case 0x09: return "跟踪模式1执行中"
                            case 0x0A: return "停止跟踪"
                            case 0x0B: return "跟踪模式2执行中"
                            case 0x0F: return "速度环模式执行中"
                            case 0x1F: return "驱动器报警"
                            case 0x20: return "伺服超差报警"
                            case 0x21: return "正向限位报警"
                            case 0x22: return "逆向限位报警"
                            case 0x23: return "时钟同步报警"
                            case 0x24: return "初始化信息报警"
                            case 0x25: return "限位开关同时导通"
                            case 0x26: return "编码器数据故障报警"
                            case 0x29: return "瞬态电流报警"
                            case 0x2A: return "连续电流报警"
                            default: return "未知"
                        }
                    }
                }
                DataLabel { label: "外框角度值:"; value: turntableData.outter_angle; fontSize: 18; labelWidth: 105 }
                DataLabel { label: "外框控制偏差:"; value: turntableData.outter_ctlDeviation; fontSize: 18; labelWidth: 125 }
            }
        }
    }
    StackLayout {
        id: stack
        currentIndex: stacklayoutindex
        width: root.width - 20
        height: 280
        anchors.top: statusGroup.bottom
        anchors.topMargin: 5
        anchors.left:root.left
        anchors.leftMargin: 10
        anchors.right: root.right
        anchors.rightMargin: 10 
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



