import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 1200
    height: 200
    color: '#e9f0f9'

    // 报警弹窗（内框/中框/外框）
    MsgPopup2 { id: innermsg }
    MsgPopup2 { id: middlemsg }
    MsgPopup2 { id: outmsg }

    // 串口与北斗信息显示框
    GroupBox {
        id: serialBDBox
        width: 420
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        topPadding: 25

        title: "串口与北斗信息"
        font.pixelSize: 18
        label: Label {
            text: parent.title
            font.pixelSize: 18
            leftPadding: 12
            topPadding: 6
        }

        background: Rectangle {
            color: "#e1d8d8"
            border.color: "gray"
            border.width: 4
            radius: 8
        }

        // 蓝色边框 - 串口选择区域
        Rectangle {
            id: serialRect
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 0
            height: 78
            color: "#e8f0fe"
            border.color: "#1a73e8"
            border.width: 2
            radius: 4

            Column {
                anchors.centerIn: parent
                spacing: 4

                // 第一行：串口号 + 波特率
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 8
                    Text {
                        text: "串口号:"
                        font.pixelSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    CusComboBox {
                        id: portComboBox
                        width: 110
                        height: 28
                        model: imageData.availablePorts
                        font.pixelSize: 14
                    }
                    Text {
                        text: "波特率:"
                        font.pixelSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    CusComboBox {
                        id: baudComboBox
                        width: 85
                        height: 28
                        model: ["9600"]
                        font.pixelSize: 14
                        currentIndex: 4  // 默认115200
                    }
                }

                // 第二行：扫描串口 + 打开串口按钮
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 15
                    CusButton_Blue {
                        id: scanButton
                        text: "扫描串口"
                        font.pixelSize: 15
                        width: 110
                        height: 28
                        onClicked: {
                            imageData.scanPorts()
                        }
                    }
                    CusButton_Blue {
                        id: openButton
                        text: bdData.portOpen ? "关闭串口" : "打开串口"
                        font.pixelSize: 15
                        width: 110
                        height: 28
                        onClicked: {
                            if (bdData.portOpen) {
                                bdData.closePort()
                            } else {
                                bdData.openPort(portComboBox.currentText,
                                                parseInt(baudComboBox.currentText))
                            }
                        }
                    }
                }
            }
        }

        // // 红色按钮区域
        // Rectangle {
        //     id: buttonRect
        //     anchors.left: parent.left
        //     anchors.leftMargin: 10
        //     anchors.right: parent.right
        //     anchors.rightMargin: 10
        //     anchors.top: serialRect.bottom
        //     anchors.topMargin: 8
        //     height: 40
        //     color: "#fce8e6"
        //     border.color: "#d93025"
        //     border.width: 2
        //     radius: 4

        //     Row {
        //         anchors.centerIn: parent
        //         spacing: 15
                
        //     }
        // }

        // 北斗数据区域
        Column {
            id: beidouColumn
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.top: serialRect.bottom
            anchors.topMargin: 10
            spacing: 6

            DataLabel {
                fontSize: 18; labelWidth: 85; valueWidth: 250
                label: "北斗时间:"
                value: {
                    if (!bdData.m_isPosValid) return "--"
                    var dt = bdData.m_bjDateTime
                    return dt ? Qt.formatDateTime(dt, "yyyy-MM-dd hh:mm:ss") : "--"
                }
            }
            DataLabel {
                fontSize: 18; labelWidth: 85; valueWidth: 250
                label: "经度:"
                value: {
                    if (!bdData.m_isPosValid) return "--"
                    var lon = bdData.m_longitude.toFixed(6)
                    var dir = bdData.m_iseast ? "E" : "W"
                    return lon + "° " + dir
                }
            }
            DataLabel {
                fontSize: 18; labelWidth: 85; valueWidth: 250
                label: "纬度:"
                value: {
                    if (!bdData.m_isPosValid) return "--"
                    var lat = bdData.m_latitude.toFixed(6)
                    var dir = bdData.m_isnorth ? "N" : "S"
                    return lat + "° " + dir
                }
            }
        }
    }

    // 转台状态显示框
    GroupBox {
        id: statusBox
        width: 565
        anchors.left: serialBDBox.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        title: "转台状态显示"
        font.pixelSize: 18
        label: Label {
            text: parent.title
            font.pixelSize: 18
            leftPadding: 12
            topPadding: 6
        }

        background: Rectangle {
            color: "#e1d8d8"
            border.color: "gray"
            border.width: 4
            radius: 8
        }

        // 四行状态数据（纵向排列）
        Column {
            spacing: 6
            anchors.centerIn: parent

            // 第一行：时间和序号
            Row {
                spacing: 8
                DataLabel { fontSize: 15; labelWidth: 55;  valueWidth: 45; label: "秒时间:"; value: turntableData.time }
                DataLabel { fontSize: 15; labelWidth: 40;  valueWidth: 30; label: "序号:";   value: turntableData.ctlnumber }
            }

            // 第二行：内框
            Row {
                spacing: 6
                DataLabel {
                    fontSize: 15; labelWidth: 68; valueWidth: 125
                    label: "内框状态:"
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
                            case 0x09: return "跟踪模式1"
                            case 0x0A: return "停止跟踪"
                            case 0x0B: return "跟踪模式2"
                            case 0x0F: return "速度环模式"
                            case 0x1F: return "驱动器报警"
                            case 0x20: return "伺服超差报警"
                            case 0x21: return "正向限位报警"
                            case 0x22: return "逆向限位报警"
                            case 0x23: return "时钟同步报警"
                            case 0x24: return "初始化报警"
                            case 0x25: return "限位开关同时导通"
                            case 0x26: return "编码器故障报警"
                            case 0x29: return "瞬态电流报警"
                            case 0x2A: return "连续电流报警"
                            default: return "未知"
                        }
                    }
                }
                DataLabel { fontSize: 15; labelWidth: 82; valueWidth: 65; label: "内框角度值:"; value: String(turntableData.inner_angle) }
                DataLabel { fontSize: 15; labelWidth: 95; valueWidth: 65; label: "内框控制偏差:"; value: String(turntableData.inner_ctlDeviation) }
            }

            // 第三行：中框
            Row {
                spacing: 6
                DataLabel {
                    fontSize: 15; labelWidth: 68; valueWidth: 125
                    label: "中框状态:"
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
                            case 0x09: return "跟踪模式1"
                            case 0x0A: return "停止跟踪"
                            case 0x0B: return "跟踪模式2"
                            case 0x0F: return "速度环模式"
                            case 0x1F: return "驱动器报警"
                            case 0x20: return "伺服超差报警"
                            case 0x21: return "正向限位报警"
                            case 0x22: return "逆向限位报警"
                            case 0x23: return "时钟同步报警"
                            case 0x24: return "初始化报警"
                            case 0x25: return "限位开关同时导通"
                            case 0x26: return "编码器故障报警"
                            case 0x29: return "瞬态电流报警"
                            case 0x2A: return "连续电流报警"
                            default: return "未知"
                        }
                    }
                }
                DataLabel { fontSize: 15; labelWidth: 82; valueWidth: 65; label: "中框角度值:"; value: turntableData.middle_angle }
                DataLabel { fontSize: 15; labelWidth: 95; valueWidth: 65; label: "中框控制偏差:"; value: turntableData.middle_ctlDeviation }
            }

            // 第四行：外框
            Row {
                spacing: 6
                DataLabel {
                    fontSize: 15; labelWidth: 68; valueWidth: 125
                    label: "外框状态:"
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
                            case 0x09: return "跟踪模式1"
                            case 0x0A: return "停止跟踪"
                            case 0x0B: return "跟踪模式2"
                            case 0x0F: return "速度环模式"
                            case 0x1F: return "驱动器报警"
                            case 0x20: return "伺服超差报警"
                            case 0x21: return "正向限位报警"
                            case 0x22: return "逆向限位报警"
                            case 0x23: return "时钟同步报警"
                            case 0x24: return "初始化报警"
                            case 0x25: return "限位开关同时导通"
                            case 0x26: return "编码器故障报警"
                            case 0x29: return "瞬态电流报警"
                            case 0x2A: return "连续电流报警"
                            default: return "未知"
                        }
                    }
                }
                DataLabel { fontSize: 15; labelWidth: 82; valueWidth: 65; label: "外框角度值:"; value: turntableData.outter_angle }
                DataLabel { fontSize: 15; labelWidth: 95; valueWidth: 65; label: "外框控制偏差:"; value: turntableData.outter_ctlDeviation }
            }
        }
    }
}
