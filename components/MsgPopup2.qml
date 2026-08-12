import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import QtQuick.Layouts 1.12

Popup {
    id: popup

    parent: Overlay.overlay

    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    // 对外属性
    property alias message: messageLabel.text
    property int autoCloseDelay: 1000
    property int margin: 12
    property int minPopupWidth: 180
    property int maxPopupWidth: 600

    // 宽度动态计算
    width: {
        var w = textMetrics.width + margin * 2
        return Math.min(maxPopupWidth, Math.max(minPopupWidth, w))
    }

    // 高度：由 Label 实际内容高度 + 上下边距决定
    implicitHeight: contentLayout.implicitHeight + margin * 2

    // 单行宽度测量
    TextMetrics {
        id: textMetrics
        font: messageLabel.font
        text: messageLabel.text
    }

    // 遮罩
    Overlay.modal: Rectangle {
        color: "#70000000"
    }

    // 背景
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

    // 内容布局 —— 居中放置，不拉伸
    ColumnLayout {
        id: contentLayout
        anchors.centerIn: parent
        width: parent.width - popup.margin * 2

        Label {
            id: messageLabel
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 22
            font.bold: true
            color: "#333333"
            text: "提示信息"
        }
    }

    Timer {
        id: autoCloseTimer
        interval: popup.autoCloseDelay
        onTriggered: popup.close()
    }

    // ----- showToast -----
    function showToast(msg, duration) {
        messageLabel.text = msg
        x = Math.round((parent.width - width) / 2)
        y = Math.round(parent.height * 0.15)

        var delay = (duration !== undefined && duration > 0) ? duration : autoCloseDelay
        autoCloseTimer.interval = delay
        autoCloseTimer.restart()

        open()
    }
}
