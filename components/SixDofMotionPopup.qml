import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "."
import taoQuick 1.0

/**
 * 六自由度平台调平控制弹窗
 *
 * 对应 C++ 侧 network/SixDofMotionClient（QML 上下文属性 sixDofMotion）。
 * 由 TurnTablestatus.qml 的「转台调平」按钮打开：
 *     SixDofMotionPopup { id: sixDofPopup; motion: sixDofMotion }
 *
 * 界面分区：
 *   1) 连接    —— 控制卡 IP、链路三态（未连接 / 等待应答 / 链路正常）、连接与断开
 *   2) 运动控制 —— X / Y 倾角（±10°）、运动时间、发送、回中位、回底部
 *   3) 状态    —— 控制卡状态与报警（只呈现故障类 100-125 与运行状态类 1-8）
 *   4) 高级设置 —— 发送广播地址、本机端口（默认值正确时无需改动）
 *   5) 底部    —— 急停（失联时仍可发送，因为急停走广播）
 */
Window {
    id: popup

    width: 880
    height: Math.max(600, contentColumn.implicitHeight)
    visible: false
    flags: Qt.Window | Qt.WindowStaysOnTopHint
    modality: Qt.NonModal
    color: "#f5f6fa"
    title: "六自由度平台调平"

    // 对外引用：由调用方注入 sixDofMotion
    property var motion: null

    // ── 与 SixDofMotionClient 保持一致的常量 ──
    readonly property real maxTiltDeg: 10.0          // kMaxTiltDeg
    readonly property int defaultMoveTimeMs: 300     // kDefaultMoveTimeMs
    readonly property int fixedTargetPort: 7408      // kDefaultTargetPort（控制卡固定）

    // ── 从 C++ 对象派生的状态 ──
    // motion 由外部注入；若上下文属性未注册，绑定的结果是 undefined，
    // 因此这里统一归一化成一个布尔量，避免把 undefined 赋给 QObject* / bool
    readonly property bool hasClient: motion !== null && motion !== undefined
    readonly property bool isConnected: hasClient ? motion.connected : false
    readonly property bool isLinkAlive: hasClient ? motion.linkAlive : false
    readonly property int statusCode: hasClient ? motion.statusCode : 0
    readonly property string errorText: hasClient ? motion.lastError : ""
    readonly property bool isFault: statusCode >= 100 && statusCode <= 125      // 故障类
    readonly property bool isRunState: statusCode >= 1 && statusCode <= 8       // 运行状态类

    // 链路三态：0=未连接 1=已打开等待应答 2=链路正常
    readonly property int linkState: !isConnected ? 0 : (isLinkAlive ? 2 : 1)

    readonly property string statusMessageText:
        (hasClient && motion.statusMessage.length > 0) ? motion.statusMessage
                                                       : "（尚未收到控制卡状态应答）"

    // 最近一次成功下发的指令（来自 commandSent 信号）
    property string lastCommand: ""
    property bool advancedExpanded: false

    // ══════════════════════════════════════════════════════════
    // 打开 / 关闭
    // ══════════════════════════════════════════════════════════

    function centerOnScreen() {
        x = Math.round((Screen.width - width) / 2)
        y = Math.round((Screen.height - height) / 2)
    }

    // 首次打开时把 C++ 侧的当前参数回填到界面
    function syncFromClient() {
        if (!motion) return
        ipField.inputIP(motion.controlCardAddress)
        targetField.text = motion.targetAddress
        localPortField.text = motion.localPort.toString()
        timeField.text = (motion.executeTimeMs > 0 ? motion.executeTimeMs
                                                   : defaultMoveTimeMs).toString()
        tiltXField.text = motion.tiltXDeg.toFixed(2)
        tiltYField.text = motion.tiltYDeg.toFixed(2)
    }

    function showPopup() {
        syncFromClient()
        centerOnScreen()
        show()
        raise()
    }

    // ══════════════════════════════════════════════════════════
    // 参数整理与校验
    // ══════════════════════════════════════════════════════════

    // 仅规范显示格式，不做限幅：超限交给 C++ 侧拒绝发送（与参考实现一致）
    function formatTilt(field) {
        var v = parseFloat(field.text)
        field.text = isFinite(v) ? v.toFixed(2) : "0.00"
    }

    function formatTime(field) {
        var t = Math.round(parseFloat(field.text))
        if (!isFinite(t) || t <= 0)
            t = defaultMoveTimeMs
        field.text = Math.max(1, Math.min(60000, t)).toString()
    }

    function ensureConnected() {
        if (!motion || !motion.connected) {
            toast.showToast("请先点击「连接」打开网口", 2500)
            return false
        }
        return true
    }

    // 把界面上的网络参数写回 C++ 对象
    function applySettings() {
        if (!motion) return
        if (ipField.isValid)
            motion.controlCardAddress = ipField.ipAddr
        if (targetField.text.length > 0)
            motion.targetAddress = targetField.text
        var p = parseInt(localPortField.text)
        if (isFinite(p) && p > 0 && p < 65536)
            motion.localPort = p
    }

    // ══════════════════════════════════════════════════════════
    // 指令下发
    // ══════════════════════════════════════════════════════════

    function toggleConnection() {
        if (!motion) return

        if (motion.connected) {
            motion.closeConnection()
            toast.showToast("网口已关闭")
            return
        }

        applySettings()
        if (motion.openConnection()) {
            motion.startConnectionCheck(3000)
            toast.showToast("网口已打开，正在等待控制卡应答…", 2000)
        } else {
            toast.showToast("打开失败：" + motion.lastError, 3500)
        }
    }

    function sendTilt() {
        if (!ensureConnected())
            return

        var rx = parseFloat(tiltXField.text)
        var ry = parseFloat(tiltYField.text)
        if (!isFinite(rx) || !isFinite(ry)) {
            toast.showToast("X / Y 倾角请填写数字", 2500)
            return
        }
        if (Math.abs(rx) > maxTiltDeg || Math.abs(ry) > maxTiltDeg) {
            toast.showToast("倾角超限：限位 ±" + maxTiltDeg + "°，指令未发送", 3000)
            return
        }

        formatTime(timeField)
        var t = parseInt(timeField.text)

        // 逐次传入执行时间，不调用 setExecuteTime()，
        // 以免回中位 / 回底部的默认时间被全局覆盖
        if (motion.sendTiltXYWithTime(rx, ry, t)) {
            toast.showToast("已下发  X=" + rx.toFixed(2) + "°  Y=" + ry.toFixed(2)
                            + "°  (" + t + "ms)")
        } else {
            toast.showToast("发送失败：" + motion.lastError, 3500)
        }
    }

    function doGoMiddle() {
        confirmMiddleLayer.visible = false
        if (!ensureConnected())
            return
        if (motion.goMiddle())
            toast.showToast("已下发：回中位（六轴归零）")
        else
            toast.showToast("发送失败：" + motion.lastError, 3500)
    }

    function doGoBottom() {
        confirmLayer.visible = false
        if (!ensureConnected())
            return
        if (motion.goBottom())
            toast.showToast("已下发：回底部（tz = -0.5m）")
        else
            toast.showToast("发送失败：" + motion.lastError, 3500)
    }

    // 急停：不弹二次确认；只要套接字打开就允许发送，
    // 链路超时不应禁用急停（急停走广播，不需要控制卡应答）
    function doEmergencyStop() {
        if (!motion || !motion.connected) {
            toast.showToast("网口未打开，急停无法发送", 3000)
            return
        }
        if (motion.emergencyStop())
            toast.showToast("急停指令已发送", 2000)
        else
            toast.showToast("急停发送失败：" + motion.lastError, 3500)
    }

    // C++ 侧信号回传
    Connections {
        // 未注入客户端时 target 保持 null，Connections 不会去解析不存在的信号
        target: popup.hasClient ? popup.motion : null

        onConnectionCheckFinished: {
            toast.showToast(message, success ? 2000 : 4000)
        }
        onCommandSent: {
            popup.lastCommand = description
        }
    }

    // ══════════════════════════════════════════════════════════
    // 主布局
    // ══════════════════════════════════════════════════════════
    Rectangle {
        id: frame
        anchors.fill: parent
        color: "#f5f6fa"
        border.color: "#c0c0c0"
        border.width: 1
        radius: 10
        clip: true

        ColumnLayout {
            id: contentColumn
            anchors.fill: parent
            spacing: 0

            // ────── 标题栏（支持拖拽）──────
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                color: "#1976D2"
                radius: 10

                MouseArea {
                    anchors.fill: parent
                    property real lastMouseX: 0
                    property real lastMouseY: 0
                    onPressed: {
                        lastMouseX = mouseX
                        lastMouseY = mouseY
                    }
                    onPositionChanged: {
                        popup.x += mouseX - lastMouseX
                        popup.y += mouseY - lastMouseY
                    }
                }

                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: 10
                    color: "#1976D2"
                }

                Text {
                    anchors.centerIn: parent
                    text: "六自由度平台调平"
                    font.pixelSize: 20
                    font.bold: true
                    color: "#ffffff"
                }

                CusButton_Blue {
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    width: 28
                    height: 28
                    text: "✕"
                    onClicked: popup.close()
                }
            }

            // ────── 内容区 ──────
            ColumnLayout {
                Layout.fillWidth: true
                Layout.margins: 12
                spacing: 12

                // ════════════ 1) 连接 ════════════
                Rectangle {
                    id: connCard
                    Layout.fillWidth: true
                    // Rectangle 没有固有高度，必须由内部内容撑开，
                    // 否则在 ColumnLayout 中会被压成 0 高，内容溢出压到下一块上
                    implicitHeight: connColumn.implicitHeight + 24
                    color: "#ffffff"
                    border.color: "#d0d0d0"
                    border.width: 1
                    radius: 4

                    ColumnLayout {
                        id: connColumn
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 10

                            Text {
                                text: "控制卡IP"
                                font.pixelSize: 15
                                font.bold: true
                                color: "#333333"
                            }

                            CusIPAddress {
                                id: ipField
                                Layout.preferredWidth: 200
                                Layout.preferredHeight: 32
                            }

                            Text {
                                text: "运动端口 " + popup.fixedTargetPort + "（控制卡固定）"
                                font.pixelSize: 13
                                color: "#666666"
                            }

                            Item { Layout.fillWidth: true }

                            CusButton_Blue {
                                id: connectButton
                                width: 100
                                height: 32
                                text: popup.isConnected ? "断开" : "连接"
                                onClicked: popup.toggleConnection()
                            }
                        }

                        // 链路三态指示：未连接 / 等待应答 / 链路正常
                        Row {
                            Layout.fillWidth: true
                            spacing: 18

                            Repeater {
                                model: [
                                    { "label": "未连接",   "state": 0, "color": "#9E9E9E" },
                                    { "label": "等待应答", "state": 1, "color": "#FF9800" },
                                    { "label": "链路正常", "state": 2, "color": "#4CAF50" }
                                ]

                                Item {
                                    width: stateDot.width + 5 + stateLabel.implicitWidth
                                    height: 22

                                    Rectangle {
                                        id: stateDot
                                        width: 10
                                        height: 10
                                        radius: 5
                                        anchors.left: parent.left
                                        anchors.verticalCenter: parent.verticalCenter
                                        color: popup.linkState === modelData.state
                                               ? modelData.color : "#D0D0D0"
                                        border.color: popup.linkState === modelData.state
                                                      ? modelData.color : "#BDBDBD"
                                        border.width: 1
                                    }
                                    Text {
                                        id: stateLabel
                                        anchors.left: stateDot.right
                                        anchors.leftMargin: 5
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: modelData.label
                                        font.pixelSize: 13
                                        font.bold: popup.linkState === modelData.state
                                        color: popup.linkState === modelData.state
                                               ? "#333333" : "#AAAAAA"
                                    }
                                }
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            font.pixelSize: 13
                            color: "#666666"
                            elide: Text.ElideRight
                            text: popup.statusMessageText
                        }
                    }
                }

                // ════════════ 2) 运动控制 ════════════
                Rectangle {
                    id: motionCard
                    Layout.fillWidth: true
                    implicitHeight: motionColumn.implicitHeight + 24
                    color: "#ffffff"
                    border.color: "#d0d0d0"
                    border.width: 1
                    radius: 4

                    ColumnLayout {
                        id: motionColumn
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 10

                        Row {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 55
                            spacing: 30

                            MyTextField {
                                id: tiltXField
                                mywidth: 155
                                myheight: 55
                                title: "X 倾角"
                                labeltext: "°"
                                onEditingFinished: popup.formatTilt(tiltXField)
                            }

                            MyTextField {
                                id: tiltYField
                                mywidth: 155
                                myheight: 55
                                title: "Y 倾角"
                                labeltext: "°"
                                onEditingFinished: popup.formatTilt(tiltYField)
                            }

                            Item {
                                width: 96
                                height: 55

                                CusButton_Blue {
                                    width: 96
                                    height: 32
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 1
                                    text: "输入清零"
                                    onClicked: {
                                        tiltXField.text = "0.00"
                                        tiltYField.text = "0.00"
                                    }
                                }
                            }
                        }

                        Row {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 55
                            spacing: 30

                            MyTextField {
                                id: timeField
                                mywidth: 155
                                myheight: 55
                                title: "运动时间"
                                labeltext: "ms"
                                text: popup.defaultMoveTimeMs.toString()
                                onEditingFinished: popup.formatTime(timeField)
                            }

                            Item {
                                width: parent.width - 155 - 30
                                height: 55

                                Text {
                                    width: parent.width
                                    anchors.verticalCenter: parent.verticalCenter
                                    wrapMode: Text.WordWrap
                                    font.pixelSize: 13
                                    color: "#666666"
                                    text: "默认 " + popup.defaultMoveTimeMs
                                          + "ms，范围 1–60000ms；时间越短动作越快，下限按机构承受能力设定。"
                                }
                            }
                        }

                        Row {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40
                            spacing: 12

                            CusButton_Blue {
                                width: parent.width - 110 * 2 - 12 * 2
                                height: 40
                                text: "发送指令"
                                font.pixelSize: 18
                                onClicked: popup.sendTilt()
                            }

                            CusButton_Blue {
                                width: 110
                                height: 40
                                text: "回中位"
                                font.pixelSize: 16
                                onClicked: confirmMiddleLayer.visible = true
                            }

                            CusButton_Blue {
                                width: 110
                                height: 40
                                text: "回底部"
                                font.pixelSize: 16
                                onClicked: confirmLayer.visible = true
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            font.pixelSize: 12
                            color: "#d93025"
                            elide: Text.ElideRight
                            text: "倾角限位 ±" + popup.maxTiltDeg
                                  + "°（超限将被拒绝发送）；回中位六轴归零，回底部 tz = -0.5m。"
                        }
                    }
                }

                // ════════════ 3) 状态与报警 ════════════
                Rectangle {
                    id: statusCard
                    Layout.fillWidth: true
                    implicitHeight: statusColumn.implicitHeight + 24
                    color: "#ffffff"
                    border.color: "#d0d0d0"
                    border.width: 1
                    radius: 4

                    ColumnLayout {
                        id: statusColumn
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 10

                            Text {
                                text: "控制卡状态"
                                font.pixelSize: 15
                                font.bold: true
                                color: "#333333"
                            }

                            // 类别标签：故障类 / 运行状态类
                            Rectangle {
                                width: chipText.implicitWidth + 18
                                height: 24
                                radius: 12
                                color: popup.isFault ? "#FDECEA"
                                                     : (popup.isRunState ? "#E8F5E9" : "#EEEEEE")
                                border.width: 1
                                border.color: popup.isFault ? "#F44336"
                                                            : (popup.isRunState ? "#4CAF50" : "#BDBDBD")

                                Text {
                                    id: chipText
                                    anchors.centerIn: parent
                                    font.pixelSize: 13
                                    font.bold: true
                                    text: popup.isFault ? "故障"
                                                         : (popup.isRunState ? "运行状态" : "无状态")
                                    color: popup.isFault ? "#C62828"
                                                         : (popup.isRunState ? "#2E7D32" : "#757575")
                                }
                            }

                            Text {
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                                font.pixelSize: 15
                                font.bold: popup.isFault
                                color: popup.isFault ? "#C62828" : "#333333"
                                text: popup.statusMessageText
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                            font.pixelSize: 13
                            color: "#666666"
                            text: "上次下发："
                                  + (popup.lastCommand.length > 0 ? popup.lastCommand : "无")
                        }

                        Text {
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                            visible: popup.errorText.length > 0
                            font.pixelSize: 13
                            color: "#C62828"
                            text: "最近错误：" + popup.errorText
                        }
                    }
                }

                // ════════════ 4) 高级设置（默认折叠）════════════
                Rectangle {
                    id: advCard
                    Layout.fillWidth: true
                    implicitHeight: advColumn.implicitHeight + 24
                    color: "#ffffff"
                    border.color: "#d0d0d0"
                    border.width: 1
                    radius: 4

                    ColumnLayout {
                        id: advColumn
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 10

                            CusButton_Blue {
                                width: 130
                                height: 28
                                // 用文字代替 ▸/▾ 箭头：部分中文字体不含该符号会显示成方框
                                text: popup.advancedExpanded ? "收起高级设置" : "展开高级设置"
                                onClicked: popup.advancedExpanded = !popup.advancedExpanded
                            }

                            Text {
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                                font.pixelSize: 13
                                color: "#666666"
                                text: "默认值适用于现场 192.168.15.x 网段，通常无需修改。"
                            }
                        }

                        Row {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 55
                            spacing: 30
                            visible: popup.advancedExpanded

                            MyTextField {
                                id: targetField
                                mywidth: 155
                                myheight: 55
                                title: "指令发送地址"
                                labeltext: ""
                            }

                            MyTextField {
                                id: localPortField
                                mywidth: 155
                                myheight: 55
                                title: "本机端口"
                                labeltext: ""
                            }

                            Item {
                                width: parent.width - 155 * 2 - 30 * 2
                                height: 55

                                Text {
                                    width: parent.width
                                    anchors.verticalCenter: parent.verticalCenter
                                    wrapMode: Text.WordWrap
                                    font.pixelSize: 12
                                    color: "#666666"
                                    text: "指令发送地址默认为子网广播 192.168.15.255；"
                                          + "本机端口需重新连接后生效。"
                                }
                            }
                        }
                    }
                }

                // ════════════ 5) 急停 ════════════
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        CusButton_Red {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 56
                            text: "急  停"
                            font.pixelSize: 24
                            enabled: popup.isConnected
                            onClicked: popup.doEmergencyStop()
                        }

                        CusButton_Blue {
                            width: 100
                            Layout.preferredHeight: 56
                            text: "关闭"
                            font.pixelSize: 18
                            onClicked: popup.close()
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        font.pixelSize: 12
                        color: "#888888"
                        elide: Text.ElideRight
                        text: "急停走广播，链路超时（等待应答）时仍可发送；该按钮不做二次确认。"
                    }
                }
            }
        }

        // ════════════════ 提示条 ════════════════
        Rectangle {
            id: toast
            z: 100
            anchors.horizontalCenter: parent.horizontalCenter
            y: 60
            width: Math.min(660, toastText.implicitWidth + 40)
            height: 44
            radius: 8
            color: "#E6000000"
            opacity: 0
            visible: opacity > 0

            Behavior on opacity {
                NumberAnimation { duration: 150 }
            }

            Text {
                id: toastText
                anchors.centerIn: parent
                width: parent.width - 32
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideRight
                color: "#ffffff"
                font.pixelSize: 16
            }

            Timer {
                id: toastTimer
                onTriggered: toast.opacity = 0
            }

            function showToast(msg, duration) {
                toastText.text = msg
                var d = (duration !== undefined && duration > 0) ? duration : 1800
                toastTimer.interval = d
                toastTimer.restart()
                toast.opacity = 1
            }
        }

        // ════════════════ 回底部二次确认 ════════════════
        Rectangle {
            id: confirmLayer
            z: 200
            anchors.fill: parent
            visible: false
            color: "#70000000"
            radius: 10

            MouseArea {
                anchors.fill: parent
            }

            Rectangle {
                anchors.centerIn: parent
                width: 460
                height: 200
                radius: 10
                color: "#ffffff"
                border.color: "#c0c0c0"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 14

                    Text {
                        Layout.fillWidth: true
                        text: "确认执行「回底部」？"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 14
                        color: "#666666"
                        text: "平台将执行 tz = -0.5m 的纵向动作，其余轴归零。请确认现场安全后再继续。"
                    }

                    Item { Layout.fillHeight: true }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 16

                        Item { Layout.fillWidth: true }

                        CusButton_Blue {
                            width: 110
                            height: 40
                            text: "取消"
                            font.pixelSize: 16
                            onClicked: confirmLayer.visible = false
                        }

                        CusButton_Red {
                            width: 110
                            height: 40
                            text: "确认回底部"
                            font.pixelSize: 16
                            onClicked: popup.doGoBottom()
                        }

                        Item { Layout.fillWidth: true }
                    }
                }
            }
        }

        // ════════════════ 回中位二次确认 ════════════════
        Rectangle {
            id: confirmMiddleLayer
            z: 200
            anchors.fill: parent
            visible: false
            color: "#70000000"
            radius: 10

            MouseArea {
                anchors.fill: parent
            }

            Rectangle {
                anchors.centerIn: parent
                width: 460
                height: 200
                radius: 10
                color: "#ffffff"
                border.color: "#c0c0c0"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 14

                    Text {
                        Layout.fillWidth: true
                        text: "确认执行「回中位」？"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#333333"
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 14
                        color: "#d93025"
                        text: "六自由度平台将上升，请确保舱盖已打开。"
                    }

                    Item { Layout.fillHeight: true }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 16

                        Item { Layout.fillWidth: true }

                        CusButton_Blue {
                            width: 110
                            height: 40
                            text: "取消"
                            font.pixelSize: 16
                            onClicked: confirmMiddleLayer.visible = false
                        }

                        CusButton_Red {
                            width: 110
                            height: 40
                            text: "确认回中位"
                            font.pixelSize: 16
                            onClicked: popup.doGoMiddle()
                        }

                        Item { Layout.fillWidth: true }
                    }
                }
            }
        }
    }
}
