import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
import "./"
import "InputRules.js" as InputRules

Item{
    id:root
    anchors.fill: parent

    // 程控模式允许的最大速度与加速度
    readonly property real maxTrackingSpeed: 14.0
    readonly property real maxAcceleration: 10.0
    readonly property var axisAngleLimits: [
        { min: -200.0, max: 200.0 },
        { min: -10.0, max: 70.0 },
        { min: -100.0, max: 100.0 }
    ]

    property var controlNames: [
        "内框（滚转轴）",
        "中框（俯仰轴）",
        "外框（方位轴）"
    ]

    property var axisEnabledStates: [false, false, false]
    property var axisLinkedStates: [false, false, false]
    property bool globalAxesEnabled: false

    //正弦摆动的幅值与频率range,frequency
    property real range: 0.0
    property real frequency: 0.0

    // 程控参数校验提示
    MessagePopup {
        id: validationMsg
    }

    // 空输入按 0 处理；格式非法时返回 0，但调用方必须先检查格式状态。
    function parseInputNumber(str) {
        var result = InputRules.validateNumber(str, true, true, 4)
        return result.state === "valid" ? result.value : 0
    }

    // 正弦运动峰值加速度：a_max = (2πf)² × A
    function sinePeakAcceleration(amplitude, freq) {
        var omega = 2 * Math.PI * Math.abs(Number(freq))
        return omega * omega * Math.abs(Number(amplitude))
    }

    function axisEnabled(axisIndex) {
        return axisEnabledStates[axisIndex] === true
    }

    function setAllAxisEnabled(enabled) {
        if (axisEnabledStates[0] === enabled
                && axisEnabledStates[1] === enabled
                && axisEnabledStates[2] === enabled)
            return

        axisEnabledStates = [enabled, enabled, enabled]
    }

    onGlobalAxesEnabledChanged: setAllAxisEnabled(globalAxesEnabled)
    Component.onCompleted: setAllAxisEnabled(globalAxesEnabled)

    function toggleAxisControl(axisIndex) {
        var states = axisEnabledStates.slice()
        states[axisIndex] = !states[axisIndex]
        axisEnabledStates = states
        turntableSendData.setAxisEnabled(axisIndex + 1, states[axisIndex])
    }

    function axisLinked(axisIndex) {
        return axisLinkedStates[axisIndex] === true
    }

    function setAxisLinked(axisIndex, linked) {
        var states = axisLinkedStates.slice()
        states[axisIndex] = linked
        axisLinkedStates = states
    }

    function linkedAxesFor(axisIndex) {
        if (!axisLinked(axisIndex))
            return [axisIndex]

        var axes = []
        for (var i = 0; i < axisLinkedStates.length; ++i) {
            if (axisLinked(i))
                axes.push(i)
        }
        return axes
    }

    function isLinkLeader(axisIndex) {
        if (!axisLinked(axisIndex))
            return true

        for (var i = 0; i < axisIndex; ++i) {
            if (axisLinked(i))
                return false
        }
        return true
    }

    function showValidationError(messages) {
        validationMsg.message = "输入参数不正确：\n"
                + messages.join("\n")
                + "\n请修改后重新发送。"
        validationMsg.open()
    }

    function axisFieldError(axisIndex, fieldName) {
        var row = axisRowRepeater.itemAt(axisIndex)
        if (!row)
            return ""

        var axisName = controlNames[axisIndex]
        var limits = axisAngleLimits[axisIndex]
        var fieldLabels = {
            target: "目标位置",
            speed: "速度",
            acceleration: "加速度"
        }
        var fieldText = row[fieldName + "Text"]
        var allowNegative = fieldName !== "acceleration"
        var formatResult = InputRules.validateNumber(fieldText, allowNegative, true, 4)

        if (formatResult.state !== "valid") {
            return axisName + " "
                    + InputRules.errorMessage(fieldLabels[fieldName], formatResult,
                                              allowNegative, true, 4)
        }

        if (fieldName === "target"
                && programModeComboBox.currentIndex === 0
                && (row.targetValue < limits.min || row.targetValue > limits.max)) {
            return axisName + "目标位置 "
                    + row.targetValue.toFixed(4) + "° 超出允许范围 "
                    + limits.min.toFixed(0) + "～"
                    + limits.max.toFixed(0) + "°"
        }

        if (fieldName === "speed"
                && (row.speedValue < -maxTrackingSpeed
                    || row.speedValue > maxTrackingSpeed)) {
            return axisName + "速度 "
                    + row.speedValue.toFixed(4) + " °/s 超出允许范围 -"
                    + maxTrackingSpeed.toFixed(0) + "～"
                    + maxTrackingSpeed.toFixed(0) + " °/s"
        }

        if (fieldName === "acceleration"
                && (row.accelerationValue < 0
                    || row.accelerationValue > maxAcceleration)) {
            return axisName + "加速度 "
                    + row.accelerationValue.toFixed(4) + " °/s² 超出允许范围 0～"
                    + maxAcceleration.toFixed(0) + " °/s²"
        }

        return ""
    }

    function validateAxisField(axisIndex, fieldName) {
        var row = axisRowRepeater.itemAt(axisIndex)
        if (!row)
            return false

        var validProperty = fieldName + "InputValid"
        var error = axisFieldError(axisIndex, fieldName)
        if (error === "") {
            row[validProperty] = true
            return true
        }

        // 与 ImageSendArea 的角度输入框一致：同一错误只在首次出现时提示，
        // 修正为合法值后才复位，避免每次切换输入框都重复弹窗。
        if (row[validProperty])
            showValidationError([error])
        row[validProperty] = false
        return false
    }

    function validateAxisGroup(axisIndexes) {
        var errors = []

        for (var i = 0; i < axisIndexes.length; ++i) {
            var axisIndex = axisIndexes[i]
            var fields = programModeComboBox.currentIndex === 0
                    ? ["target", "speed", "acceleration"]
                    : ["speed", "acceleration"]
            for (var fieldIndex = 0; fieldIndex < fields.length; ++fieldIndex) {
                var error = axisFieldError(axisIndex, fields[fieldIndex])
                if (error !== "")
                    errors.push(error)
            }
        }

        if (errors.length > 0) {
            showValidationError(errors)
            return false
        }
        return true
    }

    function sendAxisOrLink(axisIndex) {
        var axes = linkedAxesFor(axisIndex)
        if (!validateAxisGroup(axes))
            return

        for (var i = 0; i < axes.length; ++i) {
            var row = axisRowRepeater.itemAt(axes[i])
            if (!row)
                continue

            turntableSendData.sendProgramCommand(
                        axes[i] + 1,
                        programModeComboBox.currentIndex,
                        row.targetValue,
                        row.speedValue,
                        row.accelerationValue)
        }
    }

    RowLayout {
        id: sinRow
        spacing: 8

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        Text {
            text: "模式选择："
            font.pixelSize: 16
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }
        CusComboBox {
            id: programModeComboBox
            Layout.preferredWidth: 105
            Layout.preferredHeight: 35
            model: ["位置模式", "速率模式"]
            popupMinWidth: 130
        }

        Text {
            text: "正弦运动幅度："
            font.pixelSize: 16
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }
        NumericTextField{
            id:sinInput
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            Layout.preferredHeight: 35
            Layout.preferredWidth: 80
            fieldLabel: "正弦运动幅度"
            onValidationFailed: root.showValidationError([message])
            onEditingFinished: {
                if (!inputFormatValid)
                    return

                range = numericValue
                if (text === "")
                    text = "0"
            }
        }

        Text {
            text: "正弦运动频率："
            font.pixelSize: 16
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }
        NumericTextField{
            id:frequencyInput
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            Layout.preferredHeight: 35
            Layout.preferredWidth: 80
            fieldLabel: "正弦运动频率"
            onValidationFailed: root.showValidationError([message])
            onEditingFinished: {
                if (!inputFormatValid)
                    return

                frequency = numericValue
                if (text === "")
                    text = "0"
            }
        }
        Item { Layout.fillWidth: true }
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: sinRow.bottom
        anchors.topMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3
        spacing: 4

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 24
            spacing: 11

            Item { Layout.preferredWidth: 132 }
            Text {
                text: "目标位置(°)"
                font.pixelSize: 14
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 85
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                text: "速度(°/s)"
                font.pixelSize: 14
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 75
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                text: "加速度(°/s²)"
                font.pixelSize: 14
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 88
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                text: "联动"
                font.pixelSize: 14
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 45
                horizontalAlignment: Text.AlignHCenter
            }
            Item { Layout.preferredWidth: 82 }
            Item { Layout.preferredWidth: 82 }
        }

        Repeater {
            id: axisRowRepeater
            model: root.controlNames
            delegate: Rectangle {
                readonly property string targetText: targetInput.text
                readonly property string speedText: speedInput.text
                readonly property string accelerationText: accelerationInput.text
                readonly property real targetValue: root.parseInputNumber(targetInput.text)
                readonly property real speedValue: root.parseInputNumber(speedInput.text)
                readonly property real accelerationValue: root.parseInputNumber(accelerationInput.text)
                readonly property bool targetFormatValid: targetInput.inputFormatValid
                readonly property bool speedFormatValid: speedInput.inputFormatValid
                readonly property bool accelerationFormatValid:
                    accelerationInput.inputFormatValid
                readonly property bool axisFieldsFormatValid:
                    (programModeComboBox.currentIndex !== 0 || targetFormatValid)
                    && speedFormatValid && accelerationFormatValid
                property bool targetInputValid: true
                property bool speedInputValid: true
                property bool accelerationInputValid: true

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 74
                color: "#f7f9fc"
                radius: 4
                border.width: 1
                border.color: "#c8d4e3"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 3
                    spacing: 1

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 11

                        Text {
                            text: controlNames[index]
                            font.pixelSize: 18
                            font.bold: true
                            color: "#000000"
                            Layout.preferredWidth: 132
                            Layout.alignment: Qt.AlignVCenter
                        }

                        NumericTextField {
                            id: targetInput
                            enabled: programModeComboBox.currentIndex === 0
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 38
                            Layout.preferredWidth: 85
                            placeholderText: "0.0000"
                            fieldLabel: "目标位置"
                            onValidationFailed: root.showValidationError([message])
                            onEditingFinished: {
                                if (!inputFormatValid) {
                                    root.validateAxisField(index, "target")
                                    return
                                }
                                text = numericValue.toFixed(4)
                                root.validateAxisField(index, "target")
                            }
                        }
                        NumericTextField {
                            id: speedInput
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 38
                            Layout.preferredWidth: 75
                            placeholderText: "0.0000"
                            fieldLabel: "速度"
                            onValidationFailed: root.showValidationError([message])
                            onEditingFinished: {
                                if (!inputFormatValid) {
                                    root.validateAxisField(index, "speed")
                                    return
                                }
                                text = numericValue.toFixed(4)
                                root.validateAxisField(index, "speed")
                            }
                        }
                        NumericTextField {
                            id: accelerationInput
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 38
                            Layout.preferredWidth: 88
                            placeholderText: "0.0000"
                            allowNegative: false
                            fieldLabel: "加速度"
                            onValidationFailed: root.showValidationError([message])
                            onEditingFinished: {
                                if (!inputFormatValid) {
                                    root.validateAxisField(index, "acceleration")
                                    return
                                }
                                text = numericValue.toFixed(4)
                                root.validateAxisField(index, "acceleration")
                            }
                        }
                        CusCheckBox {
                            Layout.preferredWidth: 45
                            Layout.preferredHeight: 38
                            checked: root.axisLinked(index)
                            onClicked: root.setAxisLinked(index, checked)
                        }
                        CusButton_Blue {
                            id: sendButton
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 38
                            font.pixelSize: 14
                            text: "发送数据"
                            enabled: root.isLinkLeader(index) && axisFieldsFormatValid
                            onClicked: root.sendAxisOrLink(index)
                        }

                        CusButton_Blue {
                            id: sinmoveButton
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 38
                            font.pixelSize: 14
                            text: "正弦运动"
                            enabled: root.isLinkLeader(index)
                            onClicked: {
                                // 以点击瞬间输入框中的值为准，避免尚未触发 editingFinished 时使用旧值
                                var amplitudeResult = InputRules.validateNumber(
                                            sinInput.text, true, true, 4)
                                var frequencyResult = InputRules.validateNumber(
                                            frequencyInput.text, true, true, 4)
                                var formatErrors = []
                                if (amplitudeResult.state !== "valid")
                                    formatErrors.push(InputRules.errorMessage(
                                                          "正弦运动幅度", amplitudeResult,
                                                          true, true, 4))
                                if (frequencyResult.state !== "valid")
                                    formatErrors.push(InputRules.errorMessage(
                                                          "正弦运动频率", frequencyResult,
                                                          true, true, 4))
                                if (formatErrors.length > 0) {
                                    root.showValidationError(formatErrors)
                                    return
                                }

                                range = amplitudeResult.value
                                frequency = frequencyResult.value

                                var acceleration = sinePeakAcceleration(range, frequency)
                                if (acceleration > maxAcceleration) {
                                    validationMsg.message =
                                        "输入数据应满足（2πf）² × A 得到的加速度小于10 °/s²。\n"
                                        + "当前计算值为 " + acceleration.toFixed(3) + " °/s²。"
                                    validationMsg.open()
                                    return
                                }

                                var axes = root.linkedAxesFor(index)
                                for (var i = 0; i < axes.length; ++i)
                                    turntableSendData.sinMove(axes[i] + 1, range, frequency)
                                //为了保存正弦运动的数据，先定义为正弦运动时默认保存数据（转台这一路）
                                dataRecorder.startTurntableSave()
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 11

                        Item { Layout.preferredWidth: 132 }
                        Text {
                            text: "单轴控制："
                            font.pixelSize: 16
                            font.bold: true
                            color: "#000000"
                            Layout.preferredWidth: 84
                            Layout.alignment: Qt.AlignVCenter
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 36
                            font.pixelSize: 14
                            text: root.axisEnabled(index) ? "释放/解锁" : "使能/锁定"
                            onClicked: root.toggleAxisControl(index)
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 36
                            font.pixelSize: 14
                            text: "停车"
                            onClicked: {
                                turntableSendData.stopAxis(index + 1)
                                dataRecorder.stopTurntableSave()
                            }
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 36
                            font.pixelSize: 14
                            text: "回零"
                            onClicked: turntableSendData.zeroAxis(index + 1)
                        }
                        Item { Layout.fillWidth: true }
                    }
                }
            }
        }
    }
}
