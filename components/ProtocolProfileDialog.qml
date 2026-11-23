import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.12
import TaoQuick 1.0

Popup {
    id: root

    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    width: Math.min(680, parent.width - 40)
    height: 330

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    property string targetName: "导引头"
    property string selectedProfileUrl: ""

    signal protocolFileConfirmed(string profileUrl)
    signal exitRequested()

    function openForDevice(name) {
        targetName = name
        selectedProfileUrl = ""
        fileStatus.text = "尚未选择 JSON 文件"
        fileStatus.color = "#777777"
        open()
    }

    function displayName(url) {
        var normalized = String(url).replace(/\\/g, "/")
        var pos = normalized.lastIndexOf("/")
        return pos >= 0 ? normalized.substring(pos + 1) : normalized
    }

    Overlay.modal: Rectangle {
        color: "#70000000"
    }

    background: Rectangle {
        radius: 10
        color: "white"
        border.color: "#D0D0D0"
        border.width: 1

        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 0
            verticalOffset: 5
            radius: 16
            samples: 25
            color: "#55000000"
        }
    }

    contentItem: Item {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 28
            spacing: 18

            Text {
                Layout.fillWidth: true
                text: root.targetName + " — 自定义协议"
                color: "#222222"
                font.pixelSize: 24
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                Layout.fillWidth: true
                text: "当前选择为“其他”，请选择预先导入的 JSON 协议文件。\n"
                      + "确认后将按该文件指定的数据位置和位含义接收数据。"
                color: "#333333"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 52
                color: "#f5f7fa"
                radius: 5
                border.color: "#c8d0da"

                Text {
                    id: fileStatus
                    anchors.fill: parent
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    text: "尚未选择 JSON 文件"
                    color: "#777777"
                    font.pixelSize: 15
                    elide: Text.ElideMiddle
                    verticalAlignment: Text.AlignVCenter
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 18

                CusButton_White {
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 42
                    text: "选择 JSON 文件"
                    onClicked: protocolFileDialog.open()
                }

                CusButton_Blue {
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 42
                    text: "确认"
                    onClicked: {
                        if (root.selectedProfileUrl === "") {
                            fileStatus.text = "请先选择协议 JSON 文件"
                            fileStatus.color = "#c62828"
                            return
                        }
                        var url = root.selectedProfileUrl
                        root.close()
                        root.protocolFileConfirmed(url)
                    }
                }

                CusButton_White {
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 42
                    text: "退出"
                    onClicked: {
                        root.close()
                        root.exitRequested()
                    }
                }
            }
        }
    }

    FileDialog {
        id: protocolFileDialog
        title: root.targetName + "协议 JSON 文件"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        nameFilters: ["协议文件 (*.json)", "所有文件 (*)"]
        onAccepted: {
            root.selectedProfileUrl = fileUrl.toString()
            fileStatus.text = root.displayName(root.selectedProfileUrl)
            fileStatus.color = "#222222"
        }
    }
}
