import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 1200
    height: 330
    color: "#e9f0f9"

    readonly property int pageMargin: 10
    readonly property int leftPanelWidth: 650
    readonly property int ccdPanelLeft: pageMargin + leftPanelWidth + pageMargin
    readonly property bool ccdReady: ccdData.portOpen

    // 三轴共用报警弹窗
    MsgPopup2 { id: axisAlarmMsg }

    // 串口打开/关闭提示弹窗（需确认按钮）
    MessagePopup { id: bdmsg }
    MessagePopup { id: ccdmsg }

    property var axisAlarmQueue: []
    property bool axisAlarmShowing: false
    property string runCommandText: ctlNumberText(turntableData.ctlnumber)

    Connections {
        target: axisAlarmMsg
        onClosed: {
            axisAlarmShowing = false
            Qt.callLater(function() { showNextAxisAlarm() })
        }
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
        default: return "未知"
        }
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

    // 当前转台协议用 0x1F-0x2A 表示各轴报警。
    function handleAxisAlarm(axisName, status) {
        switch (Number(status)) {
        case 0x1F: queueAxisAlarm("转台" + axisName + "驱动器报警"); break
        case 0x20: queueAxisAlarm("转台" + axisName + "伺服超差报警"); break
        case 0x21: queueAxisAlarm("转台" + axisName + "正向限位报警"); break
        case 0x22: queueAxisAlarm("转台" + axisName + "逆向限位报警"); break
        case 0x23: queueAxisAlarm("转台" + axisName + "时钟同步报警"); break
        case 0x24: queueAxisAlarm("转台" + axisName + "初始化信息报警"); break
        case 0x25: queueAxisAlarm("转台" + axisName + "限位开关同时导通"); break
        case 0x26: queueAxisAlarm("转台" + axisName + "编码器数据故障报警"); break
        case 0x29: queueAxisAlarm("转台" + axisName + "瞬态电流报警"); break
        case 0x2A: queueAxisAlarm("转台" + axisName + "连续电流报警"); break
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

    // 状态栏优先显示报警，无报警时显示运行状态。
    function axisDisplayText(status) {
        switch (Number(status)) {
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
        default: return axisStatusText(status)
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
        handleAxisAlarm("内框", turntableData.inner_statusnumber)
        handleAxisAlarm("中框", turntableData.middle_statusnumber)
        handleAxisAlarm("外框", turntableData.outter_statusnumber)
    }

    // ═══════════════════════════════════════════════════════════════
    // 左栏上：北斗信息
    // ═══════════════════════════════════════════════════════════════
    GroupBox {
        id: serialBDBox
        x: root.pageMargin
        y: root.pageMargin
        width: root.leftPanelWidth
        height: 122
        topPadding: 22

        title: "北斗信息"
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
            anchors.margins: 7
            spacing: 5

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Text {
                    text: "串口号:"
                    font.pixelSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: portComboBox
                    width: 120
                    height: 27
                    model: imageData.availablePorts
                    font.pixelSize: 13
                    popupMinWidth: 180
                }
                Text {
                    text: "波特率:"
                    font.pixelSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: baudComboBox
                    width: 76
                    height: 27
                    model: ["9600"]
                    font.pixelSize: 13
                    popupMinWidth: 120
                }
                CusButton_Blue {
                    text: "扫描串口"
                    font.pixelSize: 13
                    width: 72
                    height: 27
                    onClicked: imageData.scanPorts()
                }
                CusButton_Blue {
                    text: bdData.portOpen ? "关闭串口" : "打开串口"
                    font.pixelSize: 13
                    width: 72
                    height: 27

                    Timer {
                        id: bdDelayTimer
                        interval: 1
                        onTriggered: {
                            bdmsg.message = bdData.portOpen
                                    ? "北斗串口已打开！"
                                    : "北斗串口打开失败！"
                            bdmsg.open()
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

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5
                DataLabel {
                    fontSize: 14; labelWidth: 65; valueWidth: 135; labelBold: true
                    label: "北斗时间:"
                    value: {
                        if (!bdData.m_isPosValid) return "--"
                        var dt = bdData.m_bjDateTime
                        return dt ? Qt.formatDateTime(dt, "yyyy-MM-dd hh:mm:ss") : "--"
                    }
                }
                DataLabel {
                    fontSize: 14; labelWidth: 35; valueWidth: 115; labelBold: true
                    label: "经度:"
                    value: {
                        if (!bdData.m_isPosValid) return "--"
                        return bdData.m_longitude.toFixed(6)
                                + "°" + (bdData.m_iseast ? "E" : "W")
                    }
                }
                DataLabel {
                    fontSize: 14; labelWidth: 35; valueWidth: 110; labelBold: true
                    label: "纬度:"
                    value: {
                        if (!bdData.m_isPosValid) return "--"
                        return bdData.m_latitude.toFixed(6)
                                + "°" + (bdData.m_isnorth ? "N" : "S")
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
        topPadding: 22

        title: "转台状态显示"
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
            anchors.margins: 7
            spacing: 5

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 14

                DataLabel {
                    fontSize: 14; labelWidth: 40; valueWidth: 88; labelBold: true
                    label: "时间:"
                    value: formatTurntableTime(turntableData.time)
                }
                DataLabel {
                    fontSize: 14; labelWidth: 70; valueWidth: 78; labelBold: true
                    label: "运行指令:"
                    value: runCommandText
                }
                Rectangle {
                    width: 72
                    height: 24
                    radius: 4
                    anchors.verticalCenter: parent.verticalCenter
                    color: turntableData.portOpen ? "#38a9e4" : "#b6bdc5"
                    Text {
                        anchors.centerIn: parent
                        text: turntableData.portOpen ? "转台已连接" : "转台未连接"
                        color: "white"
                        font.pixelSize: 12
                        font.bold: true
                    }
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                // 内框（滚转轴）
                Column {
                    width: 203
                    spacing: 1
                    Text {
                        text: "内框（滚转轴）"
                        font.pixelSize: 14
                        font.bold: true
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 45; valueWidth: 153; labelBold: true
                        label: "状态："
                        property int innerstatus: turntableData.inner_statusnumber
                        onInnerstatusChanged: handleAxisAlarm("内框", innerstatus)
                        value: axisDisplayText(innerstatus)
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 58; valueWidth: 140; labelBold: true
                        label: "角度值："
                        value: formatAngle(turntableData.inner_angle)
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 45; valueWidth: 153; labelBold: true
                        label: "偏差："
                        value: formatAngle(turntableData.inner_ctlDeviation)
                    }
                }

                // 中框（俯仰轴）
                Column {
                    width: 203
                    spacing: 1
                    Text {
                        text: "中框（俯仰轴）"
                        font.pixelSize: 14
                        font.bold: true
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 45; valueWidth: 153; labelBold: true
                        label: "状态："
                        property int middlestatus: turntableData.middle_statusnumber
                        onMiddlestatusChanged: handleAxisAlarm("中框", middlestatus)
                        value: axisDisplayText(middlestatus)
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 58; valueWidth: 140; labelBold: true
                        label: "角度值："
                        value: formatAngle(turntableData.middle_angle)
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 45; valueWidth: 153; labelBold: true
                        label: "偏差："
                        value: formatAngle(turntableData.middle_ctlDeviation)
                    }
                }

                // 外框（方位轴）
                Column {
                    width: 203
                    spacing: 1
                    Text {
                        text: "外框（方位轴）"
                        font.pixelSize: 14
                        font.bold: true
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 45; valueWidth: 153; labelBold: true
                        label: "状态："
                        property int outterstatus: turntableData.outter_statusnumber
                        onOutterstatusChanged: handleAxisAlarm("外框", outterstatus)
                        value: axisDisplayText(outterstatus)
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 58; valueWidth: 140; labelBold: true
                        label: "角度值："
                        value: formatAngle(turntableData.outter_angle)
                    }
                    DataLabel {
                        fontSize: 13; labelWidth: 45; valueWidth: 153; labelBold: true
                        label: "偏差："
                        value: formatAngle(turntableData.outter_ctlDeviation)
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
            anchors.margins: 7
            spacing: 5

            // CCD 串口
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 4
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
                    popupMinWidth: 150
                }
                Text {
                    text: "波特率:"
                    font.pixelSize: 13
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    id: ccdBaudComboBox
                    width: 66
                    height: 26
                    model: ["9600"]
                    font.pixelSize: 12
                    currentIndex: 0
                    popupMinWidth: 100
                }
                CusButton_Blue {
                    text: "扫描"
                    font.pixelSize: 12
                    width: 58
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
                Text {
                    text: ccdData.portOpen ? "已连接" : "未连接"
                    font.pixelSize: 13
                    font.bold: true
                    color: ccdData.portOpen ? "#1a73e8" : "#d93025"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            // 变焦与聚焦。按下立即运动，长按持续运动，松开停止。
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Text {
                    text: "变焦:"
                    font.pixelSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusButton_White {
                    id: zoomInButton
                    width: 31
                    height: 31
                    enabled: root.ccdReady
                    contentItem: Image {
                        source: "qrc:/components/plus.png"
                        width: 24; height: 24
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        opacity: parent.pressed ? 0.65 : 1.0
                    }
                    onPressed: ccdData.startZoom(1)
                    onReleased: ccdData.stopZoom()
                    Timer {
                        interval: 180
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
                    enabled: root.ccdReady
                    contentItem: Image {
                        source: "qrc:/components/minus.png"
                        width: 24; height: 24
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        opacity: parent.pressed ? 0.65 : 1.0
                    }
                    onPressed: ccdData.startZoom(-1)
                    onReleased: ccdData.stopZoom()
                    Timer {
                        interval: 180
                        repeat: true
                        running: zoomOutButton.pressed
                        onTriggered: {
                            if (zoomOutButton.pressed)
                                ccdData.startZoom(-1)
                        }
                    }
                }

                Text {
                    text: "聚焦:"
                    font.pixelSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                ButtonGroup { id: focusModeGroup }
                CusRadioButton {
                    width: 58
                    height: 28
                    text: "自动"
                    font.pixelSize: 13
                    ButtonGroup.group: focusModeGroup
                    checked: ccdData.focusMode === 0
                    enabled: root.ccdReady
                    onCheckedChanged: {
                        if (checked) ccdData.setFocusMode(0)
                    }
                }
                CusRadioButton {
                    width: 58
                    height: 28
                    text: "手动"
                    font.pixelSize: 13
                    ButtonGroup.group: focusModeGroup
                    checked: ccdData.focusMode === 1
                    enabled: root.ccdReady
                    onCheckedChanged: {
                        if (checked) ccdData.setFocusMode(1)
                    }
                }
                CusButton_White {
                    id: focusPlusButton
                    width: 31
                    height: 31
                    enabled: root.ccdReady && ccdData.focusMode === 1
                    contentItem: Image {
                        source: "qrc:/components/plus.png"
                        width: 24; height: 24
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        opacity: parent.pressed ? 0.65 : 1.0
                    }
                    onPressed: ccdData.startFocus(1)
                    onReleased: ccdData.stopFocus()
                    Timer {
                        interval: 180
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
                    enabled: root.ccdReady && ccdData.focusMode === 1
                    contentItem: Image {
                        source: "qrc:/components/minus.png"
                        width: 24; height: 24
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        opacity: parent.pressed ? 0.65 : 1.0
                    }
                    onPressed: ccdData.startFocus(-1)
                    onReleased: ccdData.stopFocus()
                    Timer {
                        interval: 180
                        repeat: true
                        running: focusMinusButton.pressed
                        onTriggered: {
                            if (focusMinusButton.pressed)
                                ccdData.startFocus(-1)
                        }
                    }
                }
            }

            // 图像增强
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 4

                Text {
                    text: "透雾:"
                    font.pixelSize: 13
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusSwitch {
                    width: 38
                    height: 26
                    enabled: root.ccdReady
                    checked: ccdData.defogEnabled
                    onCheckedChanged: ccdData.setDefog(checked)
                }
                CusComboBox {
                    width: 58
                    height: 26
                    model: ["低", "中", "高"]
                    font.pixelSize: 12
                    popupMinWidth: 80
                    currentIndex: ccdData.defogLevel - 1
                    enabled: root.ccdReady && ccdData.defogEnabled
                    onCurrentIndexChanged: {
                        if (enabled && currentIndex >= 0)
                            ccdData.setDefogLevel(currentIndex + 1)
                    }
                }

                Text {
                    text: "热浪:"
                    font.pixelSize: 13
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    width: 52
                    height: 26
                    model: ["0", "1", "2", "3", "4", "5",
                            "6", "7", "8", "9", "10"]
                    font.pixelSize: 12
                    popupMinWidth: 60
                    currentIndex: ccdData.heatHazeLevel
                    enabled: root.ccdReady
                    onCurrentIndexChanged: {
                        if (enabled && currentIndex >= 0)
                            ccdData.setHeatHazeLevel(currentIndex)
                    }
                }

                Text {
                    text: "降噪:"
                    font.pixelSize: 13
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusSwitch {
                    width: 38
                    height: 26
                    enabled: root.ccdReady
                    checked: ccdData.noiseReductionEnabled
                    onCheckedChanged: ccdData.setNoiseReduction(checked)
                }
                CusSlider {
                    id: noiseSlider
                    width: 98
                    height: 26
                    showNumber: false
                    from: 0
                    to: 100
                    stepSize: 1
                    value: ccdData.noiseReductionLevel
                    enabled: root.ccdReady && ccdData.noiseReductionEnabled
                    onMoved: ccdData.setNoiseReductionLevel(Math.round(value))
                }
                Text {
                    text: Math.round(noiseSlider.value)
                    font.pixelSize: 12
                    font.bold: true
                    color: "#1a73e8"
                    width: 24
                    horizontalAlignment: Text.AlignRight
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            // 防抖与分辨率
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Text {
                    text: "防抖:"
                    font.pixelSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusSwitch {
                    width: 38
                    height: 26
                    enabled: root.ccdReady
                    checked: ccdData.stabilizationEnabled
                    onCheckedChanged: ccdData.setStabilization(checked)
                }
                ButtonGroup { id: stabilizationGroup }
                CusRadioButton {
                    width: 44; height: 28; text: "低"; font.pixelSize: 13
                    ButtonGroup.group: stabilizationGroup
                    checked: ccdData.stabilizationLevel === 1
                    enabled: root.ccdReady && ccdData.stabilizationEnabled
                    onCheckedChanged: {
                        if (checked) ccdData.setStabilizationLevel(1)
                    }
                }
                CusRadioButton {
                    width: 44; height: 28; text: "中"; font.pixelSize: 13
                    ButtonGroup.group: stabilizationGroup
                    checked: ccdData.stabilizationLevel === 2
                    enabled: root.ccdReady && ccdData.stabilizationEnabled
                    onCheckedChanged: {
                        if (checked) ccdData.setStabilizationLevel(2)
                    }
                }
                CusRadioButton {
                    width: 44; height: 28; text: "高"; font.pixelSize: 13
                    ButtonGroup.group: stabilizationGroup
                    checked: ccdData.stabilizationLevel === 3
                    enabled: root.ccdReady && ccdData.stabilizationEnabled
                    onCheckedChanged: {
                        if (checked) ccdData.setStabilizationLevel(3)
                    }
                }

                Text {
                    text: "分辨率:"
                    font.pixelSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusComboBox {
                    width: 116
                    height: 27
                    font.pixelSize: 12
                    popupMinWidth: 155
                    model: ["1080p/30", "1080p/25", "720p/60", "720p/50",
                            "720p/30", "720p/25", "1280×960/30", "1280×960/25",
                            "1080p/50", "1080p/60", "2688×1520/25", "2688×1520/30",
                            "2688×1520/50", "2688×1520/60", "3840×2160/25",
                            "3840×2160/30", "3840×2160/50", "3840×2160/60"]
                    currentIndex: ccdData.resolutionIndex
                    enabled: root.ccdReady
                    onCurrentIndexChanged: {
                        if (enabled && currentIndex >= 0)
                            ccdData.setResolution(currentIndex)
                    }
                }
            }

            // 白平衡模式与 R 增益
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Text {
                    text: "白平衡:"
                    font.pixelSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                ButtonGroup { id: whiteBalanceGroup }
                CusRadioButton {
                    width: 58; height: 28; text: "自动"; font.pixelSize: 13
                    ButtonGroup.group: whiteBalanceGroup
                    checked: ccdData.whiteBalanceMode === 0
                    enabled: root.ccdReady
                    onCheckedChanged: {
                        if (checked) ccdData.setWhiteBalanceMode(0)
                    }
                }
                CusRadioButton {
                    width: 58; height: 28; text: "手动"; font.pixelSize: 13
                    ButtonGroup.group: whiteBalanceGroup
                    checked: ccdData.whiteBalanceMode === 1
                    enabled: root.ccdReady
                    onCheckedChanged: {
                        if (checked) ccdData.setWhiteBalanceMode(1)
                    }
                }
                Text {
                    text: "R增益:"
                    font.pixelSize: 13
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusSlider {
                    id: redGainSlider
                    width: 140
                    height: 28
                    from: 0
                    to: 255
                    stepSize: 1
                    value: ccdData.redGain
                    enabled: root.ccdReady && ccdData.whiteBalanceMode === 1
                    onMoved: ccdData.setRedGain(Math.round(value))
                }
            }

            // B 增益
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Text {
                    text: "B增益:"
                    font.pixelSize: 13
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                CusSlider {
                    id: blueGainSlider
                    width: 250
                    height: 28
                    from: 0
                    to: 255
                    stepSize: 1
                    value: ccdData.blueGain
                    enabled: root.ccdReady && ccdData.whiteBalanceMode === 1
                    onMoved: ccdData.setBlueGain(Math.round(value))
                }
            }
        }
    }
}
