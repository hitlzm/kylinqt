import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
import "./"

Item{
    id:root
    anchors.fill: parent

    // 转台正弦运动允许的最大峰值加速度（°/s²）
    readonly property real maxSineAcceleration: 10.0

    property var controlNames: [
        "内框（滚转轴）",
        "中框（俯仰轴）",
        "外框（方位轴）"
    ]

    property var axisEnabledStates: [false, false, false]
    property var axisLinkedStates: [true, true, true]

    //正弦摆动的幅值与频率range,frequency
    property real range: 0.0
    property real frequency: 0.0

    // 加速度限位提示
    MessagePopup {
        id: sineAccelerationMsg
    }

    // 数字提取函数
    function extractNumbers(str) {
        // 使用正则分割：支持英文逗号、中文逗号、以及它们周围的空白字符
        var parts = str.split(/[,，\s]+/);
        var numbers = [];
        for (var i = 0; i < parts.length; i++) {
            var trimmed = parts[i].trim();
            if (trimmed === "") continue;
            var num = parseFloat(trimmed);
            if (!isNaN(num)) {
                numbers.push(num);
            }
        }
        return numbers;  // 返回数字数组
    }

    // 输入框只取第一个有效数字作为幅值或频率
    function parseInputNumber(str) {
        var numbers = extractNumbers(str)
        return numbers.length > 0 ? Number(numbers[0]) : 0
    }

    // 正弦运动峰值加速度：a_max = (2πf)² × A
    function sinePeakAcceleration(amplitude, freq) {
        var omega = 2 * Math.PI * Math.abs(Number(freq))
        return omega * omega * Math.abs(Number(amplitude))
    }

    function axisEnabled(axisIndex) {
        return axisEnabledStates[axisIndex] === true
    }

    function toggleAxisControl(axisIndex) {
        var states = axisEnabledStates.slice()
        states[axisIndex] = !states[axisIndex]
        axisEnabledStates = states
        // TODO: 后续接入本分支的使能/释放控制逻辑
    }

    function axisLinked(axisIndex) {
        return axisLinkedStates[axisIndex] === true
    }

    function setAxisLinked(axisIndex, linked) {
        var states = axisLinkedStates.slice()
        states[axisIndex] = linked
        axisLinkedStates = states
        // TODO: 后续接入联动控制逻辑
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
        CusTextField{
            id:sinInput
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            Layout.preferredHeight: 35
            Layout.preferredWidth: 80
            onEditingFinished: range = parseInputNumber(text)
        }

        Text {
            text: "正弦运动频率："
            font.pixelSize: 16
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }
        CusTextField{
            id:frequencyInput
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            Layout.preferredHeight: 35
            Layout.preferredWidth: 80
            onEditingFinished: frequency = parseInputNumber(text)
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

                        CusTextField {
                            id: targetInput
                            enabled: programModeComboBox.currentIndex === 0
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 38
                            Layout.preferredWidth: 85
                            placeholderText: "0.0000"
                            onEditingFinished: {
                                var value = parseInputNumber(text)
                                text = value.toFixed(4)
                                if (index === 0) {
                                    turntableSendData.inner_endangle = value
                                } else if (index === 1) {
                                    turntableSendData.middle_endangle = value
                                } else if (index === 2) {
                                    turntableSendData.outter_endangle = value
                                }
                            }
                        }
                        CusTextField {
                            id: speedInput
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 38
                            Layout.preferredWidth: 75
                            placeholderText: "0.0000"
                            onEditingFinished: {
                                text = parseInputNumber(text).toFixed(4)
                            }
                        }
                        CusTextField {
                            id: accelerationInput
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 38
                            Layout.preferredWidth: 88
                            placeholderText: "0.0000"
                            onEditingFinished: {
                                text = parseInputNumber(text).toFixed(4)
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
                            onClicked: turntableSendData.buildFrame(index + 1)
                        }

                        CusButton_Blue {
                            id: sinmoveButton
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 38
                            font.pixelSize: 14
                            text: "正弦运动"
                            onClicked: {
                                // 以点击瞬间输入框中的值为准，避免尚未触发 editingFinished 时使用旧值
                                range = parseInputNumber(sinInput.text)
                                frequency = parseInputNumber(frequencyInput.text)

                                var acceleration = sinePeakAcceleration(range, frequency)
                                if (acceleration > maxSineAcceleration) {
                                    sineAccelerationMsg.message =
                                        "输入数据应满足（2πf）² × A 得到的加速度小于10 °/s²。\n"
                                        + "当前计算值为 " + acceleration.toFixed(3) + " °/s²。"
                                    sineAccelerationMsg.open()
                                    return
                                }

                                turntableSendData.sinMove(index + 1, range, frequency)
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
                            // TODO: 后续接入对应行/轴的停车控制
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 36
                            font.pixelSize: 14
                            text: "回零"
                            // TODO: 后续接入对应行/轴的回零控制
                        }
                        Item { Layout.fillWidth: true }
                    }
                }
            }
        }
    }
}
