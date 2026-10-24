import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 1200
    height: 330
    color: '#e9f0f9'

    // 三轴共用报警弹窗
    MsgPopup2 { id: axisAlarmMsg }

    // 串口打开/关闭提示弹窗（需确认按钮）
    MessagePopup { id: bdmsg }
    MessagePopup { id: ccdmsg }

    property var axisAlarmQueue: []
    property bool axisAlarmShowing: false
    property int lastValidCmdHint: -1
    property string runCommandText: "未知"

    Connections {
        target: axisAlarmMsg
        onClosed: {
            axisAlarmShowing = false
            Qt.callLater(function() { showNextAxisAlarm() })
        }
    }

    Connections {
        target: turntableData
        onCmdHintChanged: updateRunCommandText(turntableData.cmdHint)
    }

    // 根据控制字，返回具体执行的指令名称
    function ctlNumberText(n) {
    switch (n) {
        case 0x01: return "使能"
        case 0x02: return "停车"
        case 0x03: return "回零"
        case 0x04: return "位置"
        case 0x05: return "速率"
        case 0x06: return "摇摆"
        case 0x0a: return "250ms跟踪"
        case 0x0c: return "5ms跟踪"
        case 0x10: return "时间设置"
        case 0x11: return "跟踪修正"
        case 0x1F: return "复位"
        default:   return "未知"
    }
    }

    // 反馈中的指令提示只在收到指令时出现一帧，后续无效提示沿用最近一次有效指令。
    function updateRunCommandText(n) {
        var cmd = Number(n)
        if (ctlNumberText(cmd) !== "未知")
            lastValidCmdHint = cmd

        runCommandText = lastValidCmdHint >= 0
                ? ctlNumberText(lastValidCmdHint)
                : "未知"
    }

    // 转台时间以毫秒为单位，按 4 小时循环，显示为 时:分:秒
    function formatTurntableTime(timeMs) {
        var ms = Number(timeMs)
        if (!isFinite(ms) || ms < 0)
            return "--"

        var loopMs = Math.floor(ms) % 14400000
        var totalSeconds = Math.floor(loopMs / 1000)
        var hours = Math.floor(totalSeconds / 3600)
        var minutes = Math.floor((totalSeconds % 3600) / 60)
        var seconds = totalSeconds % 60

        var hh = hours < 10 ? "0" + hours : String(hours)
        var mm = minutes < 10 ? "0" + minutes : String(minutes)
        var ss = seconds < 10 ? "0" + seconds : String(seconds)
        return hh + ":" + mm + ":" + ss
    }

    // 角度和偏差按协议精度显示四位小数
    function formatAngle(value) {
        var number = Number(value)
        return isFinite(number) ? number.toFixed(4) : "--"
    }

    // 三轴报警统一入队，避免多个模态弹窗同时打开
    function queueAxisAlarm(message) {
        if (axisAlarmShowing && axisAlarmMsg.message === message)
            return
        if (axisAlarmQueue.indexOf(message) !== -1)
            return

        axisAlarmQueue.push(message)
        showNextAxisAlarm()
    }

    // 高4位为报警标志。
    function handleAxisAlarm(axisName, alarm) {
        switch (Number(alarm)) {
        case 0x01: queueAxisAlarm("转台" + axisName + "驱动器报警"); break
        case 0x02: queueAxisAlarm("转台" + axisName + "伺服超差报警"); break
        case 0x05: queueAxisAlarm("转台" + axisName + "时钟同步报警"); break
        case 0x06: queueAxisAlarm("转台" + axisName + "参数初始化错误"); break
        case 0x07: queueAxisAlarm("转台" + axisName + "限位开关同时导通"); break
        case 0x08: queueAxisAlarm("转台" + axisName + "角度传感器数据错误"); break
        case 0x0A: queueAxisAlarm("转台" + axisName + "瞬态电流报警"); break
        case 0x0B: queueAxisAlarm("转台" + axisName + "连续电流报警"); break
        }
    }

    // 低4位为运行状态标志。
    function axisStatusText(status) {
        switch (Number(status)) {
        case 0x0: return "空闲"
        case 0x1: return "伺服"
        case 0x2: return "回零执行中"
        case 0x3: return "位置执行中"
        case 0x4: return "速率执行中"
        case 0x5: return "速率稳定"
        case 0x6: return "摇摆执行中"
        case 0x7: return "摇摆稳定"
        case 0xA: return "250ms跟踪模式"
        case 0xC: return "5ms跟踪模式"
        case 0xF: return "停车执行中"
        default: return "未知"
        }
    }

    // 状态栏优先显示高4位报警；无报警时显示低4位运行状态。
    function axisDisplayText(status, alarm) {
        switch (Number(alarm)) {
        case 0x00: return axisStatusText(status)
        case 0x01: return "驱动器报警"
        case 0x02: return "伺服超差报警"
        case 0x05: return "时钟同步报警"
        case 0x06: return "参数初始化错误"
        case 0x07: return "限位开关同时导通"
        case 0x08: return "角度传感器数据错误"
        case 0x0A: return "瞬态电流报警"
        case 0x0B: return "连续电流报警"
        default: return "未知报警(0x"
                        + Number(alarm).toString(16).toUpperCase() + ")"
        }
    }

    function showNextAxisAlarm() {
        if (axisAlarmShowing || axisAlarmQueue.length === 0)
            return

        var nextMessage = axisAlarmQueue[0]
        axisAlarmQueue.shift()
        axisAlarmShowing = true
        axisAlarmMsg.showToast(nextMessage)
    }

    Component.onCompleted: {
        updateRunCommandText(turntableData.cmdHint)
        handleAxisAlarm("内框", turntableData.inner_alarm)
        handleAxisAlarm("中框", turntableData.middle_alarm)
        handleAxisAlarm("外框", turntableData.outter_alarm)
    }

    // ═══════════════════════════════════════════════════════════════
    // 第一行左侧：串口与北斗信息显示框
    // ═══════════════════════════════════════════════════════════════
    GroupBox {
        id: serialBDBox
        width: 650
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        height: 120
        topPadding: 25

        title: "北斗信息"
        font.pixelSize: 18
        label: Label {
            text: parent.title
            font.pixelSize: 18
            font.bold: true
            leftPadding: 12
            topPadding: 6
        }

        background: Rectangle {
            color: "#e1d8d8"
            border.color: "gray"
            border.width: 4
            radius: 8
        }

        Column {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12

            // 第一行：串口号 + 波特率 + 扫描串口 + 打开串口
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 6
                Text {
                    text: "串口号:"
                    font.pixelSize: 15
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: portComboBox
                    width: 130
                    height: 28
                    model: imageData.availablePorts
                    font.pixelSize: 14
                    popupMinWidth: 200
                }
                Text {
                    text: "波特率:"
                    font.pixelSize: 15
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: baudComboBox
                    width: 110
                    height: 28
                    model: ["9600"]
                    font.pixelSize: 14
                    popupMinWidth: 200
                }
                CusButton_Blue {
                    id: scanButton
                    text: "扫描串口"
                    font.pixelSize: 15
                    width: 85
                    height: 28
                    onClicked: {
                        imageData.scanPorts()
                    }
                }
                CusButton_Blue {
                    id: openButton
                    text: bdData.portOpen ? "关闭串口" : "打开串口"
                    font.pixelSize: 15
                    width: 85
                    height: 28

                    Timer {
                        id: bdDelayTimer
                        interval: 1
                        onTriggered: {
                            if (bdData.portOpen) {
                                bdmsg.message = "北斗串口已打开！"
                                bdmsg.open()
                            } else {
                                bdmsg.message = "北斗串口打开失败！"
                                bdmsg.open()
                            }
                        }
                    }

                    onClicked: {
                        if (bdData.portOpen) {
                            bdData.closePort()
                        } else {
                            bdData.openPort(portComboBox.currentText,
                                            parseInt(baudComboBox.currentText))
                            bdDelayTimer.start()
                        }
                    }
                }
            }

            // 第二行：北斗时间 + 经度 + 纬度
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5
                DataLabel {
                    fontSize: 16; labelWidth: 80; valueWidth: 174; labelBold: true
                    label: "北斗时间:"
                    value: {
                        if (!bdData.m_isPosValid) return "--"
                        var dt = bdData.m_bjDateTime
                        return dt ? Qt.formatDateTime(dt, "yyyy-MM-dd hh:mm:ss") : "--"
                    }
                }
                DataLabel {
                    fontSize: 16; labelWidth: 46; valueWidth: 118; labelBold: true
                    label: "经度:"
                    value: {
                        if (!bdData.m_isPosValid) return "--"
                        var lon = bdData.m_longitude.toFixed(6)
                        var dir = bdData.m_iseast ? "E" : "W"
                        return lon + "°" + dir
                    }
                }
                DataLabel {
                    fontSize: 16; labelWidth: 46; valueWidth: 112; labelBold: true
                    label: "纬度:"
                    value: {
                        if (!bdData.m_isPosValid) return "--"
                        var lat = bdData.m_latitude.toFixed(6)
                        var dir = bdData.m_isnorth ? "N" : "S"
                        return lat + "°" + dir
                    }
                }
            }
        }
    }

    // ═══════════════════════════════════════════════════════════════
    // 第一行右侧：CCD 控制区（无蓝色边框，紧凑布局）
    // ═══════════════════════════════════════════════════════════════
    GroupBox {
        id: ccdBox
        anchors.left: serialBDBox.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        height: 120
        topPadding: 25

        title: "CCD控制区"
        font.pixelSize: 18
        label: Label {
            text: parent.title
            font.pixelSize: 18
            font.bold: true
            leftPadding: 12
            topPadding: 6
        }

        background: Rectangle {
            color: "#e1d8d8"
            border.color: "gray"
            border.width: 4
            radius: 8
        }

        Column {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 6

            // 第一行：串口号 + 波特率 + 扫描串口 + 打开串口
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 3
                Text {
                    text: "串口号:"
                    font.pixelSize: 15
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: ccdPortComboBox
                    width: 98
                    height: 28
                    model: ccdData.availablePorts
                    font.pixelSize: 14
                    popupMinWidth: 200
                }
                Text {
                    text: "波特率:"
                    font.pixelSize: 15
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: ccdBaudComboBox
                    width: 90
                    height: 28
                    model: ["9600"]
                    font.pixelSize: 14
                    currentIndex: 0
                    popupMinWidth: 200
                }
                CusButton_Blue {
                    text: "扫描串口"
                    font.pixelSize: 15
                    width: 77
                    height: 28
                    onClicked: {
                        ccdData.scanPorts()
                    }
                }
                CusButton_Blue {
                    text: ccdData.portOpen ? "关闭串口" : "打开串口"
                    font.pixelSize: 15
                    width: 77
                    height: 28

                    Timer {
                        id: ccdDelayTimer
                        interval: 1
                        onTriggered: {
                            if (ccdData.portOpen) {
                                ccdmsg.message = "CCD串口已打开！"
                                ccdmsg.open()
                            } else {
                                ccdmsg.message = "CCD串口打开失败！"
                                ccdmsg.open()
                            }
                        }
                    }

                    onClicked: {
                        if (ccdData.portOpen) {
                            ccdData.closePort()
                        } else {
                            ccdData.openPort(ccdPortComboBox.currentText,
                                             parseInt(ccdBaudComboBox.currentText))
                            ccdDelayTimer.start()
                        }
                    }
                }
            }

            // 第二行：焦距 + 背光补偿 + 分辨率
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5
                Text {
                    text: "焦距:"
                    font.pixelSize: 15
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                ButtonGroup { id: focusGroup }
                CusRadioButton {
                    id: focal30X
                    width: 75
                    text: "30X"
                    font.pixelSize: 14
                    ButtonGroup.group: focusGroup
                    checked: ccdData.focusMode === 0
                    onCheckedChanged: {
                        if (checked) { ccdData.setFocusMode(0) }
                    }
                }
                CusRadioButton {
                    id: focal1X
                    width: 65
                    text: "1X"
                    font.pixelSize: 14
                    ButtonGroup.group: focusGroup
                    checked: ccdData.focusMode === 1
                    onCheckedChanged: {
                        if (checked) { ccdData.setFocusMode(1) }
                    }
                }
                Text {
                    text: "背光补偿:"
                    font.pixelSize: 15
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusSwitch {
                    id: backlightSwitch
                    anchors.verticalCenter: parent.verticalCenter
                    checked: ccdData.backlightOn
                    onCheckedChanged: {
                        ccdData.setBacklight(checked)
                    }
                }
                Text {
                    text: "分辨率:"
                    font.pixelSize: 15
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: resolutionComboBox
                    width: 95
                    height: 28
                    model: ["1080p/30", "1080p/25", "720p/30", "720p/25"]
                    font.pixelSize: 14
                    popupMinWidth: 130
                    currentIndex: ccdData.resolutionIndex
                    onCurrentIndexChanged: {
                        ccdData.setResolution(currentIndex)
                    }
                }
            }
        }
    }

    // ═══════════════════════════════════════════════════════════════
    // 第二行：转台状态显示框（下移）
    // ═══════════════════════════════════════════════════════════════
    GroupBox {
        id: statusBox
        width: 710
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: serialBDBox.bottom
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        topPadding: 25

        title: "转台状态显示"
        font.pixelSize: 18
        label: Label {
            text: parent.title
            font.pixelSize: 18
            font.bold: true
            leftPadding: 12
            topPadding: 6
        }

        background: Rectangle {
            color: "#e1d8d8"
            border.color: "gray"
            border.width: 4
            radius: 8
        }

        // 顶部运行信息 + 三轴分组状态
        Column {
            spacing: 10
            anchors.centerIn: parent

            // 时间、运行指令和保存数据按钮
            Row {
                spacing: 20
                DataLabel { fontSize: 18; labelWidth: 55;  valueWidth: 100; label: "时间:"; labelBold: true; value: formatTurntableTime(turntableData.time) }
                DataLabel { fontSize: 18; labelWidth: 100;  valueWidth: 100; label: "运行指令:";   labelBold: true; value: runCommandText }
                //加入保存转台数据的按钮
                Item{
                    width: 180
                    height: 20
                        CusButton_Blue {
                            id: savebtn
                            width: 80
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            // 保存数据 ⇄ 停止保存 两种状态切换（只管转台这一路）
                            text: dataRecorder.savingTurntable ? "停止保存" : "保存数据"
                            onClicked: {
                                if (dataRecorder.savingTurntable) {
                                    dataRecorder.stopTurntableSave()
                                } else {
                                    dataRecorder.startTurntableSave()
                                }
                            }
                        }
                }
            }

            // 三轴分别显示状态、角度值和偏差
            Row {
                id: axisStatusRow
                spacing: 14

                // 内框（滚转轴）
                Column {
                    spacing: 3
                    Text {
                        width: 215
                        text: "内框（滚转轴）"
                        font.pixelSize: 18
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                    }
                    DataLabel {
                        fontSize: 16; labelWidth: 56; valueWidth: 154; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "状态："
                        property int innerstatus: turntableData.inner_status
                        property int inneralarm: turntableData.inner_alarm
                        onInneralarmChanged: handleAxisAlarm("内框", inneralarm)
                        value: axisDisplayText(innerstatus, inneralarm)
                    }
                    DataLabel {
                        fontSize: 18; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "角度值："
                        value: formatAngle(turntableData.inner_angle)
                    }
                    DataLabel {
                        fontSize: 18; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "偏差："
                        value: formatAngle(turntableData.inner_ctlDeviation)
                    }
                }

                // 中框（俯仰轴）
                Column {
                    spacing: 3
                    Text {
                        width: 215
                        text: "中框（俯仰轴）"
                        font.pixelSize: 18
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                    }
                    DataLabel {
                        fontSize: 16; labelWidth: 56; valueWidth: 154; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "状态："
                        property int middlestatus: turntableData.middle_status
                        property int middlealarm: turntableData.middle_alarm
                        onMiddlealarmChanged: handleAxisAlarm("中框", middlealarm)
                        value: axisDisplayText(middlestatus, middlealarm)
                    }
                    DataLabel {
                        fontSize: 18; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "角度值："
                        value: formatAngle(turntableData.middle_angle)
                    }
                    DataLabel {
                        fontSize: 18; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "偏差："
                        value: formatAngle(turntableData.middle_ctlDeviation)
                    }
                }

                // 外框（方位轴）
                Column {
                    spacing: 3
                    Text {
                        width: 215
                        text: "外框（方位轴）"
                        font.pixelSize: 18
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                    }
                    DataLabel {
                        fontSize: 16; labelWidth: 56; valueWidth: 154; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "状态："
                        property int outterstatus: turntableData.outter_status
                        property int outteralarm: turntableData.outter_alarm
                        onOutteralarmChanged: handleAxisAlarm("外框", outteralarm)
                        value: axisDisplayText(outterstatus, outteralarm)
                    }
                    DataLabel {
                        fontSize: 18; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "角度值："
                        value: formatAngle(turntableData.outter_angle)
                    }
                    DataLabel {
                        fontSize: 18; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "偏差："
                        value: formatAngle(turntableData.outter_ctlDeviation)
                    }
                }
            }
        }
    }

    // ═══════════════════════════════════════════════════════════════
    // 第二行右侧：系统运行状态显示框
    // ═══════════════════════════════════════════════════════════════
    GroupBox {
        id: systemStatusBox
        anchors.left: statusBox.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: serialBDBox.bottom
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        topPadding: 18

        title: "系统运行状态"
        font.pixelSize: 18
        label: Label {
            text: parent.title
            font.pixelSize: 18
            font.bold: true
            leftPadding: 12
            topPadding: 6
        }

        background: Rectangle {
            color: "#e1d8d8"
            border.color: "gray"
            border.width: 4
            radius: 8
        }

        Column {
            anchors.centerIn: parent
            anchors.horizontalCenterOffset:30
            spacing: 8

            // 系统运行模式
            Row {
                spacing: 8
                Text {
                    text: "系统运行模式:"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: {
                        switch (modeController.currentMode) {
                            case 0: return "外引导模式"
                            case 1: return "程控模式"
                            case 2: return "遥控模式"
                            default: return "--"
                        }
                    }
                    font.pixelSize: 18
                    font.bold: true
                    color: "#1a73e8"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            // 当前使用导引头
            Row {
                spacing: 8
                Text {
                    text: "当前使用导引头:"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: {
                        if (imageData.portOpen) return "图像导引头"
                        if (laserData.portOpen) return "激光导引头"
                        return "未连接导引头"
                    }
                    font.pixelSize: 18
                    font.bold: true
                    color: (imageData.portOpen || laserData.portOpen) ? "#1a73e8" : "#d93025"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            // 转台连接状态
            Row {
                spacing: 8
                Text {
                    text: "转台连接状态:"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: turntableData.portOpen ? "已连接" : "未连接"
                    font.pixelSize: 18
                    font.bold: true
                    color: turntableData.portOpen ? "#1a73e8" : "#d93025"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            // 北斗模块连接状态
            Row {
                spacing: 8
                Text {
                    text: "北斗模块连接状态:"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: bdData.portOpen ? "已连接" : "未连接"
                    font.pixelSize: 18
                    font.bold: true
                    color: bdData.portOpen ? "#1a73e8" : "#d93025"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            // CCD连接状态
            Row {
                spacing: 8
                Text {
                    text: "CCD连接状态:"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: ccdData.portOpen ? "已连接" : "未连接"
                    font.pixelSize: 18
                    font.bold: true
                    color: ccdData.portOpen ? "#1a73e8" : "#d93025"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    // GroupBox {
    //     id: sixDOFBox
    //     anchors.left: systemStatusBox.right
    //     anchors.leftMargin: 10
    //     anchors.right: parent.right
    //     anchors.rightMargin: 10
    //     anchors.top: serialBDBox.bottom
    //     anchors.topMargin: 10
    //     anchors.bottom: parent.bottom
    //     anchors.bottomMargin: 10
    //     topPadding: 18

    //     title: "六自由度平台控制"
    //     font.pixelSize: 18
    //     label: Label {
    //         text: parent.title
    //         font.pixelSize: 18
    //         font.bold: true
    //         leftPadding: 12
    //         topPadding: 6
    //     }

    //     background: Rectangle {
    //         color: "#e1d8d8"
    //         border.color: "gray"
    //         border.width: 4
    //         radius: 8
    //     }

    //     Column {
    //         anchors.centerIn: parent
    //         anchors.horizontalCenterOffset:30
    //         spacing: 8
    //         CusButton_Blue {
    //         id: returnmiddleButton
    //         width: 100
    //         height: 32
    //         text: "回中位"
    //         onClicked: {
    //             console.log("回中位已发送")
    //         }
    //         }
    //         CusButton_Blue {
    //         id: returnbottomButton
    //         width: 100
    //         height: 32
    //         text: "回底部"
    //         onClicked: {
    //             console.log("回底部已发送")
    //         }
    //         }
    //         Row {
    //             spacing: 8
    //             Text {
    //                 text: "X轴倾角"
    //                 font.pixelSize: 18
    //                 font.bold: true
    //                 anchors.verticalCenter: parent.verticalCenter
    //             }
    //             CusTextField{
    //                 id:Xinput
    //                 height: 40
    //                 width: 120
    //                 horizontalAlignment: TextInput.AlignHCenter
    //                 // 垂直居中：设置垂直居中对齐和相同的上下内边距
    //                 verticalAlignment: TextInput.AlignVCenter
    //                 onEditingFinished: {
                           
    //                 }
    //             }
    //         } 
    //         Row {
    //             spacing: 8
    //             Text {
    //                 text: "Y轴倾角"
    //                 font.pixelSize: 18
    //                 font.bold: true
    //                 anchors.verticalCenter: parent.verticalCenter
    //             }
    //             CusTextField{
    //                 id:Yinput
    //                 height: 40
    //                 width: 120
    //                 horizontalAlignment: TextInput.AlignHCenter
    //                 // 垂直居中：设置垂直居中对齐和相同的上下内边距
    //                 verticalAlignment: TextInput.AlignVCenter
    //                 onEditingFinished: {
                           
    //                 }
    //             }
    //         } 
    //     }
    // }
}
