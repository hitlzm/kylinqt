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

    // ----- 新增属性 -----
    property alias message: messageText.text
    property bool buttonVisible: true          // 控制按钮是否可见
    property int autoCloseDelay: 2500          // 自动关闭延迟（毫秒）

    // 内边距
    property int margin: 30

    // 自动计算宽度（不变）
    width: Math.max(320,
                    Math.min(messageText.implicitWidth + margin * 2, 600))

    // ----- 高度由内容自动撑开（删除原有的固定 height 赋值）-----
    // 不再显式设置 height，让 Popup 自动适应 Column 的 implicitHeight

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

    // ----- 内容列（自动适应高度）-----
    Column {
        id: column
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
            visible: popup.buttonVisible   // 由 buttonVisible 控制
            anchors.horizontalCenter: parent.horizontalCenter
            width: 120
            height: 42
            text: "确认"

            onClicked: popup.close()

            background: Rectangle {
                radius: 6
                color: button.down ? "#1565C0" :
                       button.hovered ? "#2F80ED" :
                       "#1976D2"
                Behavior on color {
                    ColorAnimation { duration: 120 }
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

    // ----- 自动关闭定时器 -----
    Timer {
        id: autoCloseTimer
        interval: popup.autoCloseDelay
        onTriggered: popup.close()
    }

    // ----- 新增函数：显示纯文字提示，无按钮，定时关闭 -----
    function showToast(msg) {
        message = msg
        buttonVisible = false          // 隐藏按钮
        autoCloseTimer.restart()       // 重新计时
        open()
    }

    // ----- 弹窗关闭时重置状态 -----
    onClosed: {
        autoCloseTimer.stop()
        buttonVisible = true           // 恢复按钮可见，以便下次使用普通弹窗
    }
}