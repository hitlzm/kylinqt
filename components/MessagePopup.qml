import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

Popup {
    id: popup

    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    property alias message: messageText.text

    // 内边距
    property int margin: 30

    // 自动计算大小
    width: Math.max(320,
                    Math.min(messageText.implicitWidth + margin * 2, 600))

    height: messageText.implicitHeight + button.height + margin * 3

    // 半透明背景
    Overlay.modal: Rectangle {
        color: "#70000000"
    }

    background: Rectangle {
        id: bg
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

    Column {
        anchors.fill: parent
        anchors.margins: popup.margin
        spacing: 25

        Text {
            id: messageText

            width: popup.width - popup.margin * 2

            wrapMode: Text.WordWrap

            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: 22
            font.bold: true

            color: "#333333"

            text: "提示信息"
        }

        Button {
            id: button

            anchors.horizontalCenter: parent.horizontalCenter

            width: 120
            height: 42

            text: "确认"

            onClicked: popup.close()

            background: Rectangle {

                radius: 6

                color:
                    button.down ? "#1565C0" :
                    button.hovered ? "#2F80ED" :
                                     "#1976D2"

                Behavior on color {
                    ColorAnimation {
                        duration: 120
                    }
                }
            }

            contentItem: Text {

                text: button.text

                color: "white"

                font.pixelSize: 18

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}