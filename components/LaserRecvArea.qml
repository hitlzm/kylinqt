import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: root

    property int panelWidth: 400
    property int panelHeight: 520

    width: panelWidth
    height: panelHeight
    color: "#f5f5f5"
    border { color: "#cccccc"; width: 1 }
    radius: 4

    // ── exposed properties ──
    property string selectedMode: "pulse"
    property string portName: "COM1"
    property int baudRate: 115200
    property bool portOpen: false

    // laser parameters
    property string frequency: "1000"
    property string power: "50"
    property string dutyCycle: "50"
    property string pulseWidth: "100"
    property string current: "10"

    // ── signals ──
    signal openPortClicked(string port, int baud)
    signal closePortClicked()
    signal sendClicked(var params)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 10

        // ═══ Mode Selection ═══
        GroupBox {
            title: qsTr("模式选择")
            Layout.fillWidth: true

            RowLayout {
                spacing: 16
                anchors.fill: parent

                RadioButton {
                    text: qsTr("连续模式")
                    checked: root.selectedMode === "continuous"
                    onClicked: root.selectedMode = "continuous"
                }
                RadioButton {
                    text: qsTr("脉冲模式")
                    checked: root.selectedMode === "pulse"
                    onClicked: root.selectedMode = "pulse"
                }
                RadioButton {
                    text: qsTr("单次模式")
                    checked: root.selectedMode === "single"
                    onClicked: root.selectedMode = "single"
                }
            }
        }

        // ═══ Serial Port Config ═══
        GroupBox {
            title: qsTr("串口设置")
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 8
                anchors.fill: parent

                // port row
                RowLayout {
                    spacing: 8

                    Label { text: qsTr("端口") }
                    ComboBox {
                        id: portCombo
                        model: ["COM1", "COM2", "COM3", "COM4", "COM5", "COM6"]
                        currentIndex: 0
                        onCurrentTextChanged: root.portName = currentText
                    }

                    Label { text: qsTr("波特率") }
                    ComboBox {
                        id: baudCombo
                        model: ["9600", "19200", "38400", "57600", "115200"]
                        currentIndex: 4
                        onCurrentTextChanged: root.baudRate = parseInt(currentText)
                    }
                }

                // open / close buttons
                RowLayout {
                    spacing: 8

                    Button {
                        text: root.portOpen ? qsTr("关闭串口") : qsTr("打开串口")
                        enabled: !root.portOpen
                        onClicked: {
                            root.openPortClicked(root.portName, root.baudRate)
                            root.portOpen = true
                        }
                    }

                    Button {
                        text: qsTr("关闭串口")
                        enabled: root.portOpen
                        onClicked: {
                            root.closePortClicked()
                            root.portOpen = false
                        }
                    }
                }
            }
        }

        // ═══ Data Input Fields ═══
        GroupBox {
            title: qsTr("参数设置")
            Layout.fillWidth: true

            GridLayout {
                columns: 2
                rowSpacing: 8
                columnSpacing: 12
                anchors.fill: parent

                Label { text: qsTr("频率 (Hz)") }
                TextField { text: root.frequency; onTextChanged: root.frequency = text }

                Label { text: qsTr("功率 (%)") }
                TextField { text: root.power; onTextChanged: root.power = text }

                Label { text: qsTr("占空比 (%)") }
                TextField { text: root.dutyCycle; onTextChanged: root.dutyCycle = text }

                Label { text: qsTr("脉宽 (μs)") }
                TextField { text: root.pulseWidth; onTextChanged: root.pulseWidth = text }

                Label { text: qsTr("电流 (A)") }
                TextField { text: root.current; onTextChanged: root.current = text }
            }
        }

        // ═══ Send Button ═══
        Button {
            text: qsTr("发送数据")
            Layout.fillWidth: true
            enabled: root.portOpen

            onClicked: {
                var params = {
                    mode: root.selectedMode,
                    frequency: root.frequency,
                    power: root.power,
                    dutyCycle: root.dutyCycle,
                    pulseWidth: root.pulseWidth,
                    current: root.current
                }
                root.sendClicked(params)
            }
        }
    }
}
