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
        "三轴控制",
        "内框控制",
        "中框控制",
        "外框控制"
    ]

    property var myNumbers:[]
    property var axisEnabledStates: [false, false, false, false]

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

    // 角度格式化：固定两位小数，非法值按 0 处理
    function formatAngle(value) {
        var num = Number(value)
        return isFinite(num) ? num.toFixed(2) : "0.00"
    }

    // 读取转台当前角度：TurntableData 中实时反馈的 current 角度
    // （inner_angle / middle_angle / outter_angle 即各轴当前角度）
    function currentAngles() {
        var angles = { inner: 0, middle: 0, outter: 0 }
        if (typeof turntableData !== "undefined" && turntableData) {
            angles.inner = Number(turntableData.inner_angle) || 0
            angles.middle = Number(turntableData.middle_angle) || 0
            angles.outter = Number(turntableData.outter_angle) || 0
        }
        return angles
    }

    // 一键填入起点：把当前角度写进各行「起点(度)」输入框
    function fillStartAngles() {
        var cur = currentAngles()
        // 各行起点文本：三轴控制依次为 内框、中框、外框
        var texts = [
            formatAngle(cur.inner) + ", " + formatAngle(cur.middle) + ", " + formatAngle(cur.outter),
            formatAngle(cur.inner),
            formatAngle(cur.middle),
            formatAngle(cur.outter)
        ]

        for (var i = 0; i < angleRowRepeater.count && i < texts.length; ++i) {
            var row = angleRowRepeater.itemAt(i)
            if (row) {
                row.startText = texts[i]
            }
        }

        // 同步写入发送数据对象，保证「发送数据」用的起点与界面一致
        // （程序改 text 不会触发 onEditingFinished）
        if (typeof turntableSendData !== "undefined" && turntableSendData) {
            turntableSendData.inner_startangle = cur.inner
            turntableSendData.middle_startangle = cur.middle
            turntableSendData.outter_startangle = cur.outter
        }
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

    RowLayout {
        id: sinRow
        spacing: 8

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        // 一键填入起始位置
        CusButton_Blue {
            text: "一键填入起点"
            font.pixelSize: 15
            Layout.preferredWidth: 104
            Layout.preferredHeight: 35
            onClicked: fillStartAngles()
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
            Layout.preferredWidth: 90
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
            Layout.preferredWidth: 90
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
        anchors.bottomMargin: 6
        spacing: 4

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 24
            spacing: 10

            Item { Layout.preferredWidth: 110 }
            Text {
                text: "起点(度)"
                font.pixelSize: 14
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 150
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                text: "终点(度)"
                font.pixelSize: 14
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 150
                horizontalAlignment: Text.AlignHCenter
            }
            Item { Layout.preferredWidth: 82 }
            Item { Layout.preferredWidth: 82 }
        }

        Repeater {
            id: angleRowRepeater
            model: root.controlNames
            delegate: Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 56
                color: "#f7f9fc"
                radius: 4
                border.width: 1
                border.color: "#c8d4e3"

                // 供「一键填入起点」按钮访问本行起点输入框
                property alias startText: input1.text

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 3
                    spacing: 1

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 10

                        Text {
                            text: controlNames[index]
                            font.pixelSize: 18
                            font.bold: true
                            color: "#000000"
                            Layout.preferredWidth: 110
                            Layout.alignment: Qt.AlignVCenter
                        }

                        CusTextField{
                            id:input1
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 26
                            Layout.preferredWidth: 150
                            onEditingFinished: {
                                var mynumber = extractNumbers(text);
                                if (index === 0) {                            // 三轴控制
                                    turntableSendData.inner_startangle = mynumber[0] || 0;
                                    turntableSendData.middle_startangle = mynumber[1] || 0;
                                    turntableSendData.outter_startangle = mynumber[2] || 0;
                                    turntableSendData.index = 0;
                                } else if (index === 1) {
                                    turntableSendData.inner_startangle = Number(text)
                                } else if (index === 2) {
                                    turntableSendData.middle_startangle = Number(text)
                                } else if (index === 3) {
                                    turntableSendData.outter_startangle = Number(text)
                                }
                            }
                        }
                        CusTextField{
                            id:input2
                            Layout.preferredHeight: 26
                            Layout.preferredWidth: 150
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            onEditingFinished: {
                                var mynumber = extractNumbers(text);
                                if (index === 0) {                            // 三轴控制
                                    turntableSendData.inner_endangle = mynumber[0] || 0;
                                    turntableSendData.middle_endangle = mynumber[1] || 0;
                                    turntableSendData.outter_endangle = mynumber[2] || 0;
                                } else if (index === 1) {
                                    turntableSendData.inner_endangle = Number(text)
                                } else if (index === 2) {
                                    turntableSendData.middle_endangle = Number(text)
                                }else if (index === 3) {
                                    turntableSendData.outter_endangle = Number(text)
                                }
                            }
                        }
                        CusButton_Blue {
                            id: sendButton
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 26
                            font.pixelSize: 14
                            text: "发送数据"
                            onClicked: turntableSendData.buildFrame(index)
                        }

                        CusButton_Blue {
                            id: sinmoveButton
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 26
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

                                turntableSendData.sinMove(index, range, frequency)
                                //为了保存正弦运动的数据，先定义为正弦运动时默认保存数据（转台这一路）
                                dataRecorder.startTurntableSave()
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 10

                        Item { Layout.preferredWidth: 110 }
                        Text {
                            text: index === 0 ? "整机控制：" : "单轴控制："
                            font.pixelSize: 15
                            font.bold: true
                            color: "#000000"
                            Layout.preferredWidth: 94
                            Layout.alignment: Qt.AlignVCenter
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 26
                            font.pixelSize: 13
                            text: root.axisEnabled(index) ? "释放/解锁" : "使能/锁定"
                            onClicked: root.toggleAxisControl(index)
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 26
                            font.pixelSize: 13
                            text: "停车"
                            // TODO: 后续接入对应行/轴的停车控制
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 26
                            font.pixelSize: 13
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
