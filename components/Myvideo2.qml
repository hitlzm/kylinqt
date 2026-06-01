import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import VLCQt 1.0
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 700
    height: 500
    color: "#e9f0f9"

    // ========== VLC 状态常量（Vlc::State 枚举值） ==========
    // Idle=0, Opening=1, Buffering=2, Playing=3, Paused=4, Stopped=5, Ended=6, Error=7

    // ========== 内部状态 ==========
    property bool _connected: false   // 用户已点击"连接"设置了视频源

    // ========== 辅助函数 ==========
    function formatTime(ms) {
        if (ms <= 0 || isNaN(ms)) return "00:00"
        var totalSec = Math.floor(ms / 1000)
        var min = Math.floor(totalSec / 60)
        var sec = totalSec % 60
        return (min < 10 ? "0" + min : min) + ":" + (sec < 10 ? "0" + sec : sec)
    }

    function isPlaying() {
        return mediaPlayer.state === 3  // Vlc::Playing
    }

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

        // 播放状态指示灯
        Row {
            spacing: 6
            Layout.alignment: Qt.AlignVCenter
            Rectangle {
                id: statusDot
                width: 12; height: 12; radius: 6
                anchors.verticalCenter: parent.verticalCenter
                color: {
                    switch (mediaPlayer.state) {
                        case 3: return "#4CAF50"   // Playing → 绿色
                        case 4: return "#FF9800"   // Paused  → 橙色
                        case 1: case 2: return "#2196F3"  // Opening/Buffering → 蓝色
                        default: return _connected ? "#FF9800" : "#F44336"
                    }
                }
            }
            Text {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                color: "#333333"
                text: {
                    switch (mediaPlayer.state) {
                        case 3: return "播放中"
                        case 4: return "已暂停"
                        case 1: case 2: return "加载中"
                        case 5: return "已停止"
                        case 6: return "播放完毕"
                        case 7: return "播放错误"
                        default: return _connected ? "已就绪" : "未连接"
                    }
                }
            }
        }
    }

    // ========== 视频区域 ==========
    // VlcVideoPlayer 是 QQuickPaintedItem 的子类，行内渲染，不弹窗
    // 必须作为 videoBorder 的兄弟节点（不能嵌套在 clip:true 的容器里）
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
        // 锚定到 videoBorder 内部，留出边框
        anchors.left: videoBorder.left
        anchors.leftMargin: 2
        anchors.right: videoBorder.right
        anchors.rightMargin: 2
        anchors.top: videoBorder.top
        anchors.topMargin: 2
        anchors.bottom: videoBorder.bottom
        anchors.bottomMargin: 10

        // 关键：禁止自动播放，也不预绑 URL
        autoplay: false
    }

    // 未连接时的提示文字
    Text {
        anchors.centerIn: videoBorder
        text: _connected ? "" : "请设置视频源并点击「连接」"
        font.pixelSize: 16
        color: "#888888"
    }

    // ========== 进度条 + 时间显示 ==========
    // 所有属性从 mediaPlayer（QML VlcVideoPlayer）读取，保证与渲染同步
    RowLayout {
        id: progressRow
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: videoBorder.bottom
        anchors.topMargin: 8
        spacing: 10

        CusLabel {
            text: formatTime(mediaPlayer.time)
            font.pixelSize: 14
            Layout.preferredWidth: 50
            horizontalAlignment: Text.AlignRight
        }

        CusSlider {
            id: progressSlider
            Layout.fillWidth: true
            showNumber: false
            from: 0
            to: mediaPlayer.length > 0 ? mediaPlayer.length : 1
            value: mediaPlayer.time
            enabled: _connected && mediaPlayer.seekable && mediaPlayer.length > 0

            onMoved: {
                // 用户拖拽进度条 → seek QML 侧播放器
                mediaPlayer.setTime(value)
            }
        }

        CusLabel {
            text: formatTime(mediaPlayer.length)
            font.pixelSize: 14
            Layout.preferredWidth: 50
            horizontalAlignment: Text.AlignLeft
        }
    }

    // ========== 控制按钮栏 ==========
    RowLayout {
        id: controlRow
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: progressRow.bottom
        anchors.topMargin: 10
        spacing: 12

        // 播放/暂停 切换按钮（根据 mediaPlayer.state 动态切换）
        CusButton_Blue {
            text: isPlaying() ? "暂停" : "播放"
            Layout.fillWidth: true
            height: 40
            onClicked: {
                if (!_connected) {
                    // 首次点击：先用输入框的 URL 连接
                    connectToUrl(urlInput.text.trim())
                }
                if (isPlaying()) {
                    mediaPlayer.pause()
                } else {
                    mediaPlayer.play()
                }
            }
        }

        // 停止按钮
        CusButton_Blue {
            text: "停止"
            Layout.fillWidth: true
            height: 40
            onClicked: mediaPlayer.stop()
        }

        // 音量图标
        Text {
            text: "🔈"
            font.pixelSize: 18
            Layout.alignment: Qt.AlignVCenter
        }

        // 音量滑块（直接从 mediaPlayer 读写音量）
        CusSlider {
            id: volumeSlider
            Layout.preferredWidth: 120
            showNumber: true
            from: 0
            to: 200
            value: mediaPlayer.volume

            onMoved: mediaPlayer.setVolume(value)
        }

        // 音量百分比
        CusLabel {
            text: Math.round(mediaPlayer.volume / 2) + "%"
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
            text: vlcplayer.url.toString()   // 仅用于显示 C++ 侧记录的 URL
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

    // ========== 连接函数（唯一会同时操作两边的入口） ==========
    function connectToUrl(newUrl) {
        if (newUrl === "") return
        // 1. 停止当前播放
        mediaPlayer.stop()
        // 2. 设置 QML 侧 URL（不会自动播放，因为 autoplay: false）
        mediaPlayer.url = newUrl
        // 3. 同步 C++ 侧 URL（vlcplayer.openUrl 现在只 setUrl，不 play）
        vlcplayer.openUrl(newUrl)
        // 4. 标记已连接
        _connected = true
    }

    // ========== 信号监听 ==========
    Connections {
        target: mediaPlayer

        function onStateChanged() {
            // state 变化通过属性绑定自动更新 UI，这里仅做日志
            var stateNames = ["空闲", "打开中", "缓冲中", "播放中", "已暂停", "已停止", "播放完毕", "错误"]
            console.log("VlcVideoPlayer state:", stateNames[mediaPlayer.state] || mediaPlayer.state)
        }
    }
}
