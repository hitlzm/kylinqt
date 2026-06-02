import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import VLCQt 1.0
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 700
    height: 430
    color: "#e9f0f9"

    // ========== 内部状态 ==========
    property bool _connected: false

    // ========== 顶部标题栏 ==========
    RowLayout {
        id: titleBar
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 12

        Text {
            text: "导引头视频"
            font.pixelSize: 22
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }

        Item { Layout.fillWidth: true }

        // 播放状态指示灯（读 vlcplayer.playing）
        Row {
            spacing: 6
            Layout.alignment: Qt.AlignVCenter
            Rectangle {
                id: statusDot
                width: 12; height: 12; radius: 6
                anchors.verticalCenter: parent.verticalCenter
                color: vlcplayer.playing ? "#4CAF50" : (_connected ? "#FF9800" : "#F44336")
            }
            Text {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                color: "#333333"
                text: vlcplayer.playing ? "播放中" : (_connected ? "已就绪" : "未连接")
            }
        }
    }

    // ========== 视频区域 ==========
    // VlcVideoPlayer 仅用于视频渲染，音频和状态由 C++ vlcplayer 控制
    Rectangle {
        id: videoBorder
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: titleBar.bottom
        anchors.topMargin: 8
        height: 290
        color: "#202020"
        border.color: "#505050"
        border.width: 2
        radius: 8
    }

    VlcVideoPlayer {
        id: mediaPlayer
        anchors.left: videoBorder.left
        anchors.leftMargin: 2
        anchors.right: videoBorder.right
        anchors.rightMargin: 2
        anchors.top: videoBorder.top
        anchors.topMargin: 2
        anchors.bottom: videoBorder.bottom
        anchors.bottomMargin: 2

        autoplay: false
        volume: 0   // 静音：音频由 C++ vlcplayer 统一管理
    }

    Text {
        anchors.centerIn: videoBorder
        text: _connected ? "" : "请设置视频源并点击「连接」"
        font.pixelSize: 16
        color: "#888888"
    }

    // ========== 控制按钮栏（全部调用 vlcplayer 方法） ==========
    RowLayout {
        id: controlRow
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: videoBorder.bottom
        anchors.topMargin: 10
        spacing: 12

        // 播放/暂停 切换按钮
        CusButton_Blue {
            text: vlcplayer.playing ? "暂停" : "播放"
            Layout.fillWidth: true
            height: 40
            onClicked: {
                if (!_connected) {
                    connectToUrl(urlInput.text.trim())
                }
                if (vlcplayer.playing) {
                    vlcplayer.pause()
                    mediaPlayer.pause()     // 同步 QML 渲染
                } else {
                    vlcplayer.play()
                    mediaPlayer.play()      // 同步 QML 渲染
                }
            }
        }

        // 停止按钮
        CusButton_Blue {
            text: "停止"
            Layout.fillWidth: true
            height: 40
            onClicked: {
                vlcplayer.stop()
                mediaPlayer.stop()          // 同步 QML 渲染
            }
        }

        // 音量图标
        Text {
            text: "🔈"
            font.pixelSize: 18
            Layout.alignment: Qt.AlignVCenter
        }

        // 音量滑块（读/写 vlcplayer.volume，范围 0-200）
        CusSlider {
            id: volumeSlider
            Layout.preferredWidth: 120
            showNumber: true
            from: 0
            to: 200
            value: vlcplayer.volume

            onMoved: vlcplayer.setVolume(value)
        }

        // 音量百分比
        CusLabel {
            text: Math.round(vlcplayer.volume / 2) + "%"
            font.pixelSize: 14
            Layout.preferredWidth: 45
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // ========== URL 输入区 ==========
    RowLayout {
        id: urlRow
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: controlRow.bottom
        anchors.topMargin: 10
        spacing: 10

        Text {
            text: "RTSP:"
            font.pixelSize: 16
            color: "#333333"
            Layout.alignment: Qt.AlignVCenter
        }

        CusTextField {
            id: urlInput
            Layout.fillWidth: true
            text: vlcplayer.url.toString()
            font.pixelSize: 14
            onAccepted: connectToUrl(text.trim())
        }

        CusButton_Blue {
            text: "连接"
            Layout.preferredWidth: 70
            height: 36
            onClicked: connectToUrl(urlInput.text.trim())
        }
    }

    // ========== 连接函数 ==========
    function connectToUrl(newUrl) {
        if (newUrl === "") return
        // 1. 两边都停止
        vlcplayer.stop()
        mediaPlayer.stop()
        // 2. C++ 侧设置 URL（openUrl 已改为只 setUrl，不自动 play）
        vlcplayer.openUrl(newUrl)
        // 3. QML 侧设置 URL（autoplay: false，不自动播）
        mediaPlayer.url = newUrl
        // 4. 标记已连接
        _connected = true
    }

    // ========== C++ vlcplayer 信号监听 ==========
    Connections {
        target: vlcplayer

        function onPlayingChanged() {
            console.log("vlcplayer playingChanged:", vlcplayer.playing)
        }

        function onEnded() {
            console.log("vlcplayer: 播放结束")
            mediaPlayer.stop()
        }

        function onError(errorMsg) {
            console.log("vlcplayer error:", errorMsg)
            mediaPlayer.stop()
        }

        function onStopped() {
            console.log("vlcplayer: 已停止")
        }

        function onLengthChanged() {
            console.log("vlcplayer lengthChanged:", vlcplayer.length)
        }

        function onSeekableChanged() {
            console.log("vlcplayer seekableChanged:", vlcplayer.seekable)
        }
    }
}
