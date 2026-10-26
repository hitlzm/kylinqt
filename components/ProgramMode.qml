import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
import "./"

Item{
    id:root
    anchors.fill: parent

    property var controlNames: [
        "内框（滚转轴）",
        "中框（俯仰轴）",
        "外框（方位轴）"
    ]

    // 三个轴的联动选择。勾选只表示该轴参与联动，不改变单轴发送入口。
    property bool innerLinked: true
    property bool middleLinked: true
    property bool outterLinked: true
    property var axisEnabledStates: [false, false, false]

    function setLinked(axisIndex, checked) {
        if (axisIndex === 0) innerLinked = checked
        else if (axisIndex === 1) middleLinked = checked
        else if (axisIndex === 2) outterLinked = checked
    }

    function linked(axisIndex) {
        if (axisIndex === 0) return innerLinked
        if (axisIndex === 1) return middleLinked
        if (axisIndex === 2) return outterLinked
        return false
    }

    function axisEnabled(axisIndex) {
        return axisEnabledStates[axisIndex] === true
    }

    function setAxisControlEnabled(axisIndex, enabled) {
        var states = axisEnabledStates.slice()
        states[axisIndex] = enabled
        axisEnabledStates = states
    }

    function toggleAxisEnabled(axisIndex) {
        var enabled = !axisEnabled(axisIndex)
        setAxisControlEnabled(axisIndex, enabled)
        turntableSendData.setAxisEnabled(axisIndex + 1, enabled)
    }

    function normalisedNumber(text) {
        var value = Number(text)
        return isFinite(value) ? value : 0
    }

    RowLayout {
        id: modeRow
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
            Layout.preferredWidth: 110
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
        CusTextField {
            id: sinInput
            Layout.preferredWidth: 80
            Layout.preferredHeight: 35
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            placeholderText: "0.0000"
        }

        Text {
            text: "正弦运动频率："
            font.pixelSize: 16
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }
        CusTextField {
            id: frequencyInput
            Layout.preferredWidth: 80
            Layout.preferredHeight: 35
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            placeholderText: "0.0000"
        }
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: modeRow.bottom
        anchors.topMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        spacing: 4

        RowLayout {
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
            id: angleRowRepeater
            model: root.controlNames
            delegate: Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 70
                color: "#f7f9fc"
                radius: 4
                border.width: 1
                border.color: "#c8d4e3"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 2

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
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 85
                            placeholderText: "0.0000"
                            onEditingFinished: {
                                var value = root.normalisedNumber(text)
                                text = value.toFixed(4)
                                if (index === 0) turntableSendData.inner_endangle = value
                                else if (index === 1) turntableSendData.middle_endangle = value
                                else if (index === 2) turntableSendData.outter_endangle = value
                            }
                        }

                        CusTextField {
                            id: speedInput
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 75
                            placeholderText: "0.0000"
                            onEditingFinished: {
                                text = root.normalisedNumber(text).toFixed(4)
                            }
                        }

                        CusTextField {
                            id: accelerationInput
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            Layout.preferredHeight: 34
                            Layout.preferredWidth: 88
                            placeholderText: "0.0000"
                            onEditingFinished: {
                                text = root.normalisedNumber(text).toFixed(4)
                            }
                        }

                        CusCheckBox {
                            id: linkedCheckBox
                            Layout.preferredWidth: 45
                            Layout.preferredHeight: 34
                            checked: root.linked(index)
                            onCheckedChanged: root.setLinked(index, checked)
                        }

                        CusButton_Blue {
                            id: sendButton
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 34
                            font.pixelSize: 14
                            text: "发送数据"
                            onClicked: turntableSendData.buildFrame(index + 1)
                        }

                        CusButton_Blue {
                            id: sinmoveButton
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 34
                            font.pixelSize: 14
                            text: "正弦运动"
                            onClicked: turntableSendData.buildFrame(index + 1)
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
                            Layout.preferredHeight: 34
                            font.pixelSize: 15
                            text: root.axisEnabled(index) ? "释放" : "使能"
                            onClicked: root.toggleAxisEnabled(index)
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 34
                            font.pixelSize: 15
                            text: "停车"
                            onClicked: turntableSendData.stopAxis(index + 1)
                        }
                        CusButton_Blue {
                            Layout.preferredWidth: 82
                            Layout.preferredHeight: 34
                            font.pixelSize: 15
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
