import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import VLCQt 1.0
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 700; height: 430
    color: "#e9f0f9"

    // Vlc::State: Idle=0, Opening=1, Buffering=2, Playing=3, Paused=4, Stopped=5, Ended=6, Error=7

    property bool _connected: false
    property bool _stopped: false
    function isPlaying() { return mediaPlayer.state === 3 }

    // ========== 顶部标题栏 ==========
    RowLayout {
        id: titleBar
        anchors.left: parent.left;  anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: parent.top;     anchors.topMargin: 10
        spacing: 12

        Text { text: "导引头视频"; font.pixelSize: 22; font.bold: true; color: "#000000"; Layout.alignment: Qt.AlignVCenter }
        Item { Layout.fillWidth: true }

        Row {
            spacing: 6; Layout.alignment: Qt.AlignVCenter
            Rectangle {
                id: statusDot; width: 12; height: 12; radius: 6; anchors.verticalCenter: parent.verticalCenter
                color: {
                    if (_stopped) return "#F44336"
                    switch (mediaPlayer.state) { case 3: return "#4CAF50"; case 4: return "#FF9800"; case 1: case 2: return "#2196F3"; default: return _connected ? "#FF9800" : "#F44336" }
                }
            }
            Text { anchors.verticalCenter: parent.verticalCenter; font.pixelSize: 16; color: "#333333"
                text: { if (_stopped) return "已停止"; switch (mediaPlayer.state) { case 3: return "播放中"; case 4: return "已暂停"; case 1: case 2: return "加载中"; case 6: return "播放完毕"; case 7: return "播放错误"; default: return _connected ? "已就绪" : "未连接" } } }
        }
    }

    // ========== 视频区域 ==========
    Rectangle {
        id: videoBorder
        anchors.left: parent.left;   anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: titleBar.bottom; anchors.topMargin: 8
        height: 290
        color: "#202020"; border.color: "#505050"; border.width: 2; radius: 8
    }

    VlcVideoPlayer {
        id: mediaPlayer
        anchors.left: videoBorder.left;     anchors.leftMargin: 2
        anchors.right: videoBorder.right;   anchors.rightMargin: 2
        anchors.top: videoBorder.top;       anchors.topMargin: 2
        anchors.bottom: videoBorder.bottom; anchors.bottomMargin: 2
        autoplay: false
    }

    Text { anchors.centerIn: videoBorder; text: _connected ? "" : "请设置视频源并点击「连接」"; font.pixelSize: 16; color: "#888888" }
    Rectangle { id: stopOverlay; anchors.fill: videoBorder; color: "black"; visible: _stopped }

    // ========== 控制按钮栏 ==========
    RowLayout {
        id: controlRow
        anchors.left: parent.left;   anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: videoBorder.bottom; anchors.topMargin: 10
        spacing: 12

        CusButton_Blue {
            text: _stopped ? "播放" : (isPlaying() ? "暂停" : "播放")
            Layout.fillWidth: true; height: 40
            onClicked: {
                if (_stopped) { _stopped = false; mediaPlayer.url = vlcplayer.url; mediaPlayer.play(); return }
                if (!_connected) connectToUrl(urlInput.text.trim())
                if (isPlaying()) mediaPlayer.pause(); else mediaPlayer.play()
            }
        }
        CusButton_Blue { text: "停止"; Layout.fillWidth: true; height: 40
            onClicked: { _stopped = true; mediaPlayer.stop(); mediaPlayer.url = "" } }
        Text { text: "🔈"; font.pixelSize: 18; Layout.alignment: Qt.AlignVCenter }
        CusSlider { id: volumeSlider; Layout.preferredWidth: 120; showNumber: true
            from: 0; to: 200; value: mediaPlayer.volume; onMoved: mediaPlayer.setVolume(value) }
        CusLabel { text: Math.round(mediaPlayer.volume / 2) + "%"; font.pixelSize: 14; Layout.preferredWidth: 45; horizontalAlignment: Text.AlignHCenter }
    }

    // ========== URL 输入区 ==========
    RowLayout {
        id: urlRow
        anchors.left: parent.left;   anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: controlRow.bottom; anchors.topMargin: 10
        spacing: 10
        Text { text: "RTSP:"; font.pixelSize: 16; color: "#333333"; Layout.alignment: Qt.AlignVCenter }
        CusTextField { id: urlInput; Layout.fillWidth: true; text: vlcplayer.url.toString(); font.pixelSize: 14; onAccepted: connectToUrl(text.trim()) }
        CusButton_Blue { text: "连接"; Layout.preferredWidth: 70; height: 36; onClicked: connectToUrl(urlInput.text.trim()) }
    }

    function connectToUrl(newUrl) { if (newUrl === "") return; _stopped = false; mediaPlayer.stop(); mediaPlayer.url = newUrl; vlcplayer.openUrl(newUrl); _connected = true }
    Connections { target: mediaPlayer; function onStateChanged() { if (mediaPlayer.state === 6) _stopped = true } }
}
