import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import VlcVideo 1.0
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 700; height: 430
    color: "#e9f0f9"

    property bool _connected: false

    // ========== 顶部标题栏 ==========
    RowLayout {
        id: titleBar
        anchors.left: parent.left;  anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: parent.top;     anchors.topMargin: 10
        spacing: 12

        Text { text: "导引头视频(VlcVideo)"; font.pixelSize: 22; font.bold: true; color: "#000000"; Layout.alignment: Qt.AlignVCenter }
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

    Text { anchors.centerIn: videoBorder; text: _connected ? "" : "请设置视频源并点击「连接」"; font.pixelSize: 16; color: "#888888" }

    // ========== 控制按钮栏 ==========
    RowLayout {
        id: controlRow
        anchors.left: parent.left;   anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: videoBorder.bottom; anchors.topMargin: 10
        spacing: 12

        CusButton_Blue {
            text: videoPlayer.playing ? "暂停" : "播放"
            Layout.fillWidth: true; height: 40
            onClicked: {
                if (!_connected) connectToUrl(urlInput.text.trim())
                if (videoPlayer.playing) videoPlayer.pause()
                else videoPlayer.play()
            }
        }
        CusButton_Blue { text: "停止"; Layout.fillWidth: true; height: 40
            onClicked: videoPlayer.stop() }
        Text { text: "🔈"; font.pixelSize: 18; Layout.alignment: Qt.AlignVCenter }
        CusSlider { id: volumeSlider; Layout.preferredWidth: 120; showNumber: true; from: 0; to: 200
            value: videoPlayer.volume; onMoved: videoPlayer.setVolume(value) }
        CusLabel { text: Math.round(videoPlayer.volume / 2) + "%"; font.pixelSize: 14; Layout.preferredWidth: 45; horizontalAlignment: Text.AlignHCenter }
    }

    // ========== URL 输入区 ==========
    RowLayout {
        id: urlRow
        anchors.left: parent.left;   anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: controlRow.bottom; anchors.topMargin: 10
        spacing: 10
        Text { text: "RTSP:"; font.pixelSize: 16; color: "#333333"; Layout.alignment: Qt.AlignVCenter }
        CusTextField { id: urlInput; Layout.fillWidth: true; font.pixelSize: 14; onAccepted: connectToUrl(text.trim()) }
        CusButton_Blue { text: "连接"; Layout.preferredWidth: 70; height: 36; onClicked: connectToUrl(urlInput.text.trim()) }
    }

    function connectToUrl(newUrl) { if (newUrl === "") return; videoPlayer.stop(); videoPlayer.source = newUrl; _connected = true }

    Connections { target: videoPlayer
        function onPlayingChanged() { console.log("VlcVideo playing:", videoPlayer.playing) }
        function onEnded() { console.log("VlcVideo: 播放结束") }
        function onError(msg) { console.log("VlcVideo error:", msg) }
        function onStopped() { console.log("VlcVideo: 已停止") }
    }
}
