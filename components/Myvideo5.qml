import QtQuick 2.12
import QtQuick.Window 2.12
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

    Text { anchors.centerIn: videoBorder; text: _connected ? "" : "请设置视频源并点击「连接」"; font.pixelSize: 16; color: "#888888" }

    // ========== 放大按钮（视频区域右上角） ==========
    Rectangle {
        id: enlargeBtn
        anchors.right: videoBorder.right; anchors.rightMargin: 8
        anchors.top: videoBorder.top;     anchors.topMargin: 8
        width: 32; height: 32
        radius: 6
        color: "#99000000"
        visible: _connected

        Text {
            anchors.centerIn: parent
            text: "⛶"
            font.pixelSize: 20
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: videoPopup.open()
        }
    }

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
        CusLabel { text: Math.round(videoPlayer.volume ) + "%"; font.pixelSize: 14; Layout.preferredWidth: 45; horizontalAlignment: Text.AlignHCenter }
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

    // ========== 视频放大弹窗 ==========
    Popup {
        id: videoPopup
        modal: false
        dim: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        width: 960; height: 680

        Component.onCompleted: {
            x = Math.round((root.Window.width - width) / 2)
            y = Math.round((root.Window.height - height) / 2)
        }

        onOpened: {
            // 每次打开都重新设置源（关闭时已清空），确保使用干净的 RTSP 连接
            videoLarge.source = videoPlayer.source
            if (videoPlayer.playing) {
                videoLarge.play()
            }
        }
        onClosed: {
            // 停止播放并清空源 → 触发 setupPlayer() 内部 releasePlayer() 同步释放，
            // 彻底断开 RTSP 连接，避免 pause/resume 导致的卡顿。
            // C++ 层 stop() 已改为异步延迟释放，此处通过 source="" 确保立即清理。
            console.log("videoPopup closed — stopping videoLarge")
            if (videoLarge.playing) videoLarge.pause()
            videoLarge.stop()
            videoLarge.source = ""
        }

        background: Rectangle {
            color: "#1a1a2e"
            radius: 8
            border.color: "#3a3a5e"
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // ===== 标题栏（可拖拽） =====
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                color: "#252540"
                radius: 8

                // 底部直角（与内容区衔接）
                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: 8
                    color: "#252540"
                }

                // 拖拽区域
                MouseArea {
                    anchors.fill: parent
                    anchors.rightMargin: 52   // 给关闭按钮留空间
                    property real _lastX: 0
                    property real _lastY: 0
                    onPressed: {
                        _lastX = mouse.x
                        _lastY = mouse.y
                    }
                    onPositionChanged: {
                        videoPopup.x += mouse.x - _lastX
                        videoPopup.y += mouse.y - _lastY
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 14
                    spacing: 8

                    Text {
                        text: "视频放大"
                        font.pixelSize: 17; font.bold: true
                        color: "#e0e0e0"
                        Layout.alignment: Qt.AlignVCenter
                    }
                    Item { Layout.fillWidth: true }

                    // 关闭按钮
                    Rectangle {
                        width: 32; height: 32
                        radius: 8
                        color: closeMa.containsMouse ? "#E53935" : "#00000000"
                        Layout.alignment: Qt.AlignVCenter
                        Layout.rightMargin: 8

                        Text {
                            anchors.centerIn: parent
                            text: "✕"
                            font.pixelSize: 16
                            color: closeMa.containsMouse ? "#ffffff" : "#aaaaaa"
                        }

                        MouseArea {
                            id: closeMa
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: videoPopup.close()
                        }
                    }
                }
            }

            // ===== 视频区域 =====
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 10
                color: "#202020"
                border.color: "#404060"; border.width: 1; radius: 4

                VlcVideo {
                    id: videoLarge
                    anchors.fill: parent
                    anchors.margins: 2

                    Component.onDestruction: {
                        // 安全网：即使 onClosed 未触发，析构时也释放解码器
                        console.log("videoLarge destroying — forcing stop")
                        if (playing) pause()
                        stop()
                    }
                }
            }

            // ===== 底部控制栏 =====
            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.bottomMargin: 14
                spacing: 12

                CusButton_Blue {
                    text: videoLarge.playing ? "暂停" : "播放"
                    Layout.fillWidth: true; height: 38
                    onClicked: {
                        if (videoLarge.playing) videoLarge.pause()
                        else videoLarge.play()
                    }
                }
                CusButton_Blue {
                    text: "停止"
                    Layout.fillWidth: true; height: 38
                    onClicked: videoLarge.stop()
                }
            }
        }
    }
}
