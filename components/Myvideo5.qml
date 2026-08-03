import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import VlcVideo 1.0
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 700; height: 450
    color: "#e9f0f9"

    property bool _connected: false

    // ========== 顶部标题栏 ==========
    RowLayout {
        id: titleBar
        anchors.left: parent.left;  anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: parent.top;     anchors.topMargin: 10
        // 固定高度：避免麒麟下 22px 标题行高变大，把下方控件挤出 430 底边
        height: 26
        spacing: 12

        Text { text: "导引头视频"; font.pixelSize: 22; font.bold: true; color: "#000000"; Layout.alignment: Qt.AlignVCenter }
        Item { Layout.fillWidth: true }

        Row {
            spacing: 6; Layout.alignment: Qt.AlignVCenter
            Rectangle { width: 12; height: 12; radius: 6; anchors.verticalCenter: parent.verticalCenter
                color: videoPlayer.playing ? "#4CAF50" : (_connected ? "#FF9800" : "#F44336") }
            Text { anchors.verticalCenter: parent.verticalCenter; font.pixelSize: 16; color: "#333333"
                text: videoPlayer.playing ? "播放中" : (_connected ? "已就绪" : "未连接") }
        }
    }

    // ========== 视频区域 ==========
    Rectangle {
        id: videoBorder
        anchors.left: parent.left;   anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: titleBar.bottom; anchors.topMargin: 8
        height: 300
        color: "#202020"; border.color: "#505050"; border.width: 2; radius: 8
    }

    VlcVideo {
        id: videoPlayer
        anchors.left: videoBorder.left;     anchors.leftMargin: 2
        anchors.right: videoBorder.right;   anchors.rightMargin: 2
        anchors.top: videoBorder.top;       anchors.topMargin: 2
        anchors.bottom: videoBorder.bottom; anchors.bottomMargin: 2
    }

    // 停止时的黑色遮罩层（覆盖在视频画面上方）
    Rectangle {
        id: stopOverlay
        anchors.fill: videoBorder
        color: "#000000"
        visible: false
        z: 1
    }

    Text { anchors.centerIn: videoBorder; text: _connected ? "" : "请设置视频源并点击「连接」"; font.pixelSize: 16; color: "#888888" }

    // ========== 控制按钮栏 ==========
    RowLayout {
        id: controlRow
        anchors.left: parent.left;   anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: videoBorder.bottom; anchors.topMargin: 8
        // 固定行高：行内按钮不再随麒麟字体行高变化
        height: 36
        spacing: 12

        CusButton_Blue {
            text: videoPlayer.playing ? "暂停" : "播放"
            Layout.fillWidth: true; Layout.preferredHeight: 32
            onClicked: {
                if (!_connected) connectToUrl(urlInput.text.trim())
                if (videoPlayer.playing) videoPlayer.pause()
                else { videoPlayer.play(); stopOverlay.visible = false }
            }
        }
        CusButton_Blue { text: "停止"; Layout.fillWidth: true; Layout.preferredHeight: 32
            onClicked: {
                videoPlayer.stop()
                stopOverlay.visible = true
            } }
        Text { text: "🔈"; font.pixelSize: 18; Layout.alignment: Qt.AlignVCenter }
        CusSlider { id: volumeSlider; Layout.preferredWidth: 120; showNumber: true; from: 0; to: 200
            value: videoPlayer.volume; onMoved: videoPlayer.setVolume(value) }
        CusLabel { text: Math.round(videoPlayer.volume ) + "%"; font.pixelSize: 14; Layout.preferredWidth: 45; horizontalAlignment: Text.AlignHCenter }
    }

    // ========== URL 输入区 ==========
    RowLayout {
        id: urlRow
        anchors.left: parent.left;   anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: controlRow.bottom; anchors.topMargin: 8
        // 固定行高：输入框和连接按钮不再随麒麟字体行高变化，杜绝溢出父边界
        height: 32
        spacing: 10
        Text { text: "RTSP:"; font.pixelSize: 16; color: "#333333"; Layout.alignment: Qt.AlignVCenter }
        CusTextField { id: urlInput; Layout.fillWidth: true; Layout.preferredHeight: 26; font.pixelSize: 14; onAccepted: connectToUrl(text.trim()) }
        CusButton_Blue { text: "连接"; Layout.preferredWidth: 70; Layout.preferredHeight: 32; onClicked: connectToUrl(urlInput.text.trim()) }
    }

    function connectToUrl(newUrl) { if (newUrl === "") return; videoPlayer.stop(); videoPlayer.source = newUrl; _connected = true }

    Connections { target: videoPlayer
        function onPlayingChanged() { console.log("VlcVideo playing:", videoPlayer.playing) }
        function onEnded() { console.log("VlcVideo: 播放结束"); stopOverlay.visible = true }
        function onError(msg) { console.log("VlcVideo error:", msg) }
        function onStopped() { console.log("VlcVideo: 已停止"); stopOverlay.visible = true }
    }
}
