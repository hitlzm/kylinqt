import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 1200
    height: 330
    color: "#e9f0f9"

    readonly property int pageMargin: 10
    readonly property int leftPanelWidth: 710
    readonly property int ccdPanelLeft: pageMargin + leftPanelWidth + 14

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
        case 0x09: return "3秒跟踪"
        case 0x0a: return "250ms跟踪"
        case 0x0b: return "20ms跟踪"
        case 0x0c: return "5ms跟踪"
        case 0x10: return "时间设置"
        case 0x11: return "跟踪修正"
        case 0x1F: return "复位"
        default: return "未知"
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

    // 界面只保留原来的四档分辨率，映射到当前串口类使用的命令索引。
    function resolutionUiIndex(commandIndex) {
        switch (Number(commandIndex)) {
        case 0: return 0
        case 1: return 1
        case 4: return 2
        case 5: return 3
        default: return 0
        }
    }

    function resolutionCommandIndex(uiIndex) {
        switch (Number(uiIndex)) {
        case 0: return 0
        case 1: return 1
        case 2: return 4
        case 3: return 5
        default: return 0
        }
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
        case 0x9: return "3秒跟踪模式"
        case 0xA: return "250ms跟踪模式"
        case 0xB: return "20ms跟踪模式"
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

    function axisValueColor(alarm) {
        return Number(alarm) === 0 ? CusConfig.dataValueColor : CusConfig.dataValueColorAbnormal
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
    // 左栏上：北斗信息
    // ═══════════════════════════════════════════════════════════════
    GroupBox {
        id: serialBDBox
        x: root.pageMargin
        y: root.pageMargin
        width: root.leftPanelWidth
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
    // 左栏下：转台状态（与北斗信息右边框对齐）
    // ═══════════════════════════════════════════════════════════════
    GroupBox {
        id: statusBox
        x: root.pageMargin
        anchors.top: serialBDBox.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: root.pageMargin
        anchors.bottomMargin: root.pageMargin
        width: root.leftPanelWidth
        topPadding: 25

        // 顶部信息行与三轴状态列共用同一套列宽和间距，保证左侧边界一致。
        readonly property int axisColumnWidth: 215
        readonly property int axisColumnSpacing: 14
        readonly property int valueFontSize: 18

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

        Column {
            anchors.centerIn: parent
            spacing: 10

            Row {
                spacing: statusBox.axisColumnSpacing

                Item {
                    width: statusBox.axisColumnWidth
                    height: savebtn.height
                    DataLabel {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        fontSize: statusBox.valueFontSize
                        labelWidth: 55
                        valueWidth: 100
                        labelBold: true
                        labelHorizontalAlignment: Text.AlignLeft
                        label: "时间:"
                        value: formatTurntableTime(turntableData.time)
                    }
                }
                Item {
                    width: statusBox.axisColumnWidth
                    height: savebtn.height
                    DataLabel {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        fontSize: statusBox.valueFontSize
                        labelWidth: 100
                        valueWidth: 100
                        labelBold: true
                        labelHorizontalAlignment: Text.AlignLeft
                        label: "运行指令:"
                        value: runCommandText
                    }
                }
                Item {
                    width: statusBox.axisColumnWidth
                    height: savebtn.height
                    CusButton_Blue {
                        id: savebtn
                        width: 90
                        height: 30
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
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

            Row {
                id: axisStatusRow
                spacing: statusBox.axisColumnSpacing

                // 内框（滚转轴）
                Column {
                    spacing: 3
                    Text {
                        width: statusBox.axisColumnWidth
                        text: "内框（滚转轴）"
                        font.pixelSize: statusBox.valueFontSize
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 56; valueWidth: 154; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "状态："
                        property int innerstatus: turntableData.inner_status
                        property int inneralarm: turntableData.inner_alarm
                        onInneralarmChanged: handleAxisAlarm("内框", inneralarm)
                        value: axisDisplayText(innerstatus, inneralarm)
                        valueColor: root.axisValueColor(inneralarm)
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "角度值："
                        value: formatAngle(turntableData.inner_angle) + "°"
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "偏差："
                        value: formatAngle(turntableData.inner_ctlDeviation) + "°"
                    }
                }

                // 中框（俯仰轴）
                Column {
                    spacing: 3
                    Text {
                        width: statusBox.axisColumnWidth
                        text: "中框（俯仰轴）"
                        font.pixelSize: statusBox.valueFontSize
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 56; valueWidth: 154; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "状态："
                        property int middlestatus: turntableData.middle_status
                        property int middlealarm: turntableData.middle_alarm
                        onMiddlealarmChanged: handleAxisAlarm("中框", middlealarm)
                        value: axisDisplayText(middlestatus, middlealarm)
                        valueColor: root.axisValueColor(middlealarm)
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "角度值："
                        value: formatAngle(turntableData.middle_angle) + "°"
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "偏差："
                        value: formatAngle(turntableData.middle_ctlDeviation) + "°"
                    }
                }

                // 外框（方位轴）
                Column {
                    spacing: 3
                    Text {
                        width: statusBox.axisColumnWidth
                        text: "外框（方位轴）"
                        font.pixelSize: statusBox.valueFontSize
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 56; valueWidth: 154; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "状态："
                        property int outterstatus: turntableData.outter_status
                        property int outteralarm: turntableData.outter_alarm
                        onOutteralarmChanged: handleAxisAlarm("外框", outteralarm)
                        value: axisDisplayText(outterstatus, outteralarm)
                        valueColor: root.axisValueColor(outteralarm)
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "角度值："
                        value: formatAngle(turntableData.outter_angle) + "°"
                    }
                    DataLabel {
                        fontSize: statusBox.valueFontSize; labelWidth: 80; valueWidth: 130; labelBold: true; labelHorizontalAlignment: Text.AlignLeft
                        label: "偏差："
                        value: formatAngle(turntableData.outter_ctlDeviation) + "°"
                    }
                }
            }
        }
    }

    // ═══════════════════════════════════════════════════════════════
    // 右栏：CCD 配置（替代原系统运行状态区域）
    // ═══════════════════════════════════════════════════════════════
    GroupBox {
        id: ccdBox
        anchors.left: parent.left
        anchors.leftMargin: root.ccdPanelLeft
        anchors.right: parent.right
        anchors.rightMargin: root.pageMargin
        anchors.top: parent.top
        anchors.topMargin: root.pageMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: root.pageMargin
        topPadding: 22

        readonly property int ccdLabelWidth: 84
        readonly property int ccdRowSpacing: 6

        title: "CCD配置"
        font.pixelSize: 17
        label: Label {
            text: parent.title
            font.pixelSize: 17
            font.bold: true
            leftPadding: 10
            topPadding: 5
        }

        background: Rectangle {
            color: "#e1d8d8"
            border.color: "gray"
            border.width: 3
            radius: 8
        }

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 14

            // CCD 串口
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 6
                Text {
                    text: "串口:"
                    font.pixelSize: 13
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: ccdPortComboBox
                    width: 82
                    height: 26
                    model: ccdData.availablePorts
                    font.pixelSize: 12
                    // 串口名称较长时，下拉显示区至少保持该宽度。
                    popupMinWidth: 160
                }
                Text {
                    text: "波特率:"
                    font.pixelSize: 13
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: ccdBaudComboBox
                    width: 82
                    height: 26
                    model: ["9600"]
                    font.pixelSize: 12
                    currentIndex: 0
                }
                CusButton_Blue {
                    text: "扫描串口"
                    font.pixelSize: 12
                    width: 74
                    height: 26
                    onClicked: ccdData.scanPorts()
                }
                CusButton_Blue {
                    text: ccdData.portOpen ? "关闭串口" : "打开串口"
                    font.pixelSize: 12
                    width: 78
                    height: 26

                    Timer {
                        id: ccdDelayTimer
                        interval: 1
                        onTriggered: {
                            ccdmsg.message = ccdData.portOpen
                                    ? "CCD串口已打开！"
                                    : "CCD串口打开失败！"
                            ccdmsg.open()
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

            // 变焦与聚焦。按下立即运动，长按持续运动，松开停止。
            Item {
                width: parent.width
                height: 31

                Row {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 6

                Text {
                    text: "变焦:"
                    font.pixelSize: 14
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Item {
                    width: 14
                    height: 1
                }
                CusButton_White {
                    id: zoomInButton
                    width: 31
                    height: 31
                    Layout.minimumWidth: 31
                    Layout.preferredWidth: 31
                    Layout.maximumWidth: 31
                    Layout.minimumHeight: 31
                    Layout.preferredHeight: 31
                    Layout.maximumHeight: 31
                    contentItem: Image {
                        source: "qrc:/components/plus.png"
                        width: 24; height: 24
                        sourceSize.width: 24; sourceSize.height: 24
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        opacity: parent.pressed ? 0.65 : 1.0
                    }
                    onPressed: ccdData.startZoom(1)
                    onReleased: ccdData.stopZoom()
                    Timer {
                        interval: 20
                        repeat: true
                        running: zoomInButton.pressed
                        onTriggered: {
                            if (zoomInButton.pressed)
                                ccdData.startZoom(1)
                        }
                    }
                }
                CusButton_White {
                    id: zoomOutButton
                    width: 31
                    height: 31
                    Layout.minimumWidth: 31
                    Layout.preferredWidth: 31
                    Layout.maximumWidth: 31
                    Layout.minimumHeight: 31
                    Layout.preferredHeight: 31
                    Layout.maximumHeight: 31
                    contentItem: Image {
                        source: "qrc:/components/minus.png"
                        width: 24; height: 24
                        sourceSize.width: 24; sourceSize.height: 24
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        opacity: parent.pressed ? 0.65 : 1.0
                    }
                    onPressed: ccdData.startZoom(-1)
                    onReleased: ccdData.stopZoom()
                    Timer {
                        interval: 20
                        repeat: true
                        running: zoomOutButton.pressed
                        onTriggered: {
                            if (zoomOutButton.pressed)
                                ccdData.startZoom(-1)
                        }
                    }
                }

                }

                Row {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 6

                Text {
                    text: "自动聚焦:"
                    font.pixelSize: 14
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusSwitch {
                    width: 38
                    height: 26
                    Layout.minimumWidth: 38
                    Layout.preferredWidth: 38
                    Layout.maximumWidth: 38
                    anchors.verticalCenter: parent.verticalCenter
                    checked: ccdData.focusMode === 0
                    onCheckedChanged: {
                        ccdData.setFocusMode(checked ? 0 : 1)
                    }
                }
                Item {
                    width: 10
                    height: 1
                }
                CusButton_White {
                    id: focusPlusButton
                    width: 31
                    height: 31
                    Layout.minimumWidth: 31
                    Layout.preferredWidth: 31
                    Layout.maximumWidth: 31
                    Layout.minimumHeight: 31
                    Layout.preferredHeight: 31
                    Layout.maximumHeight: 31
                    enabled: ccdData.focusMode === 1
                    contentItem: Image {
                        source: "qrc:/components/plus.png"
                        width: 24; height: 24
                        sourceSize.width: 24; sourceSize.height: 24
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        opacity: parent.pressed ? 0.65 : 1.0
                    }
                    onPressed: ccdData.startFocus(1)
                    onReleased: ccdData.stopFocus()
                    Timer {
                        interval: 20
                        repeat: true
                        running: focusPlusButton.pressed
                        onTriggered: {
                            if (focusPlusButton.pressed)
                                ccdData.startFocus(1)
                        }
                    }
                }
                CusButton_White {
                    id: focusMinusButton
                    width: 31
                    height: 31
                    Layout.minimumWidth: 31
                    Layout.preferredWidth: 31
                    Layout.maximumWidth: 31
                    Layout.minimumHeight: 31
                    Layout.preferredHeight: 31
                    Layout.maximumHeight: 31
                    enabled: ccdData.focusMode === 1
                    contentItem: Image {
                        source: "qrc:/components/minus.png"
                        width: 24; height: 24
                        sourceSize.width: 24; sourceSize.height: 24
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        opacity: parent.pressed ? 0.65 : 1.0
                    }
                    onPressed: ccdData.startFocus(-1)
                    onReleased: ccdData.stopFocus()
                    Timer {
                        interval: 20
                        repeat: true
                        running: focusMinusButton.pressed
                        onTriggered: {
                            if (focusMinusButton.pressed)
                                ccdData.startFocus(-1)
                        }
                    }
                }

                }
            }

            // 分辨率与热浪去除
            RowLayout {
                width: parent.width
                height: 28
                spacing: ccdBox.ccdRowSpacing

                Text {
                    text: "分辨率:"
                    font.pixelSize: 14
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Item {
                    Layout.fillWidth: true
                    Layout.minimumWidth: 86
                    Layout.preferredWidth: 110
                    height: 27

                    CusComboBox {
                        anchors.fill: parent
                        font.pixelSize: 12
                        model: ["1080p/30", "1080p/25", "720p/30", "720p/25"]
                        currentIndex: root.resolutionUiIndex(ccdData.resolutionIndex)
                        onCurrentIndexChanged: {
                            if (enabled && currentIndex >= 0)
                                ccdData.setResolution(root.resolutionCommandIndex(currentIndex))
                        }
                    }
                }

                Text {
                    text: "热浪去除等级:"
                    font.pixelSize: 14
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                }
                Item {
                    Layout.minimumWidth: 64
                    Layout.preferredWidth: 64
                    Layout.maximumWidth: 64
                    height: 26

                    CusComboBox {
                        anchors.fill: parent
                        model: ["0", "1", "2", "3", "4", "5",
                                "6", "7", "8", "9", "10"]
                        font.pixelSize: 12
                        currentIndex: ccdData.heatHazeLevel
                        onCurrentIndexChanged: {
                            if (enabled && currentIndex >= 0)
                                ccdData.setHeatHazeLevel(currentIndex)
                        }
                    }
                }
            }

            // 降噪开关与强度
            RowLayout {
                width: parent.width
                height: 28
                spacing: ccdBox.ccdRowSpacing

                Text {
                    text: "降噪:"
                    font.pixelSize: 14
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                CusSwitch {
                    width: 38
                    height: 26
                    checked: ccdData.noiseReductionEnabled
                    onCheckedChanged: ccdData.setNoiseReduction(checked)
                }
                CusSlider {
                    id: noiseSlider
                    Layout.fillWidth: true
                    Layout.minimumWidth: 160
                    Layout.preferredWidth: 235
                    height: 26
                    showNumber: false
                    from: 0
                    to: 100
                    stepSize: 1
                    value: ccdData.noiseReductionLevel
                    enabled: ccdData.noiseReductionEnabled
                    onMoved: ccdData.setNoiseReductionLevel(Math.round(value))
                }
                Text {
                    text: Math.round(noiseSlider.value)
                    font.pixelSize: 14
                    font.bold: true
                    color: "#1a73e8"
                    width: 24
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // 防抖
            Item {
                width: parent.width
                height: 28

                Row {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 2

                    Text {
                        text: "防抖:"
                        width: 42
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    CusSwitch {
                        width: 36
                        height: 24
                        leftPadding: 0
                        rightPadding: 0
                        clip: true
                        anchors.verticalCenter: parent.verticalCenter
                        checked: ccdData.stabilizationEnabled
                        onCheckedChanged: ccdData.setStabilization(checked)
                    }
                    Text {
                        text: "防抖强度:"
                        width: 70
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    CusComboBox {
                        width: 46
                        height: 26
                        leftPadding: 2
                        rightPadding: 0
                        spacing: 0
                        anchors.verticalCenter: parent.verticalCenter
                        model: ["低", "中", "高"]
                        font.pixelSize: 12
                        currentIndex: ccdData.stabilizationLevel - 1
                        enabled: ccdData.stabilizationEnabled
                        onCurrentIndexChanged: {
                            if (enabled && currentIndex >= 0)
                                ccdData.setStabilizationLevel(currentIndex + 1)
                        }
                    }
                }

                Row {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 2

                    Text {
                        text: "透雾:"
                        width: 42
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    CusSwitch {
                        width: 36
                        height: 24
                        leftPadding: 0
                        rightPadding: 0
                        clip: true
                        anchors.verticalCenter: parent.verticalCenter
                        checked: ccdData.defogEnabled
                        onCheckedChanged: ccdData.setDefog(checked)
                    }
                    Text {
                        text: "透雾强度:"
                        width: 70
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    CusComboBox {
                        width: 46
                        height: 26
                        leftPadding: 2
                        rightPadding: 0
                        spacing: 0
                        anchors.verticalCenter: parent.verticalCenter
                        model: ["低", "中", "高"]
                        font.pixelSize: 12
                        currentIndex: ccdData.defogLevel - 1
                        enabled: ccdData.defogEnabled
                        onCurrentIndexChanged: {
                            if (enabled && currentIndex >= 0)
                                ccdData.setDefogLevel(currentIndex + 1)
                        }
                    }
                }
            }

            // 自动白平衡、开关分别与 R/B 增益放在同一行
            Column {
                width: parent.width
                height: 60
                spacing: 4

                RowLayout {
                    width: parent.width
                    height: 28
                    spacing: 7
                    Text {
                        text: "自动白平衡:"
                        Layout.preferredWidth: 84
                        font.pixelSize: 14
                        font.bold: true
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        Layout.preferredWidth: 52
                        text: "R增益:"
                        font.pixelSize: 14
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                    }
                    CusSlider {
                        id: redGainSlider
                        Layout.fillWidth: true
                        Layout.minimumWidth: 210
                        Layout.preferredWidth: 255
                        height: 28
                        from: 0
                        to: 255
                        stepSize: 1
                        value: ccdData.redGain
                        enabled: ccdData.whiteBalanceMode === 1
                        onMoved: ccdData.setRedGain(Math.round(value))
                    }
                }

                RowLayout {
                    width: parent.width
                    height: 28
                    spacing: 7
                    Item {
                        Layout.preferredWidth: 84
                        Layout.fillHeight: true
                        CusSwitch {
                            width: 38
                            height: 26
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            checked: ccdData.whiteBalanceMode === 0
                            onCheckedChanged: {
                                ccdData.setWhiteBalanceMode(checked ? 0 : 1)
                            }
                        }
                    }
                    Text {
                        Layout.preferredWidth: 52
                        text: "B增益:"
                        font.pixelSize: 14
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                    }
                    CusSlider {
                        id: blueGainSlider
                        Layout.fillWidth: true
                        Layout.minimumWidth: 210
                        Layout.preferredWidth: 255
                        height: 28
                        from: 0
                        to: 255
                        stepSize: 1
                        value: ccdData.blueGain
                        enabled: ccdData.whiteBalanceMode === 1
                        onMoved: ccdData.setBlueGain(Math.round(value))
                    }
                }
            }
        }
    }
}
