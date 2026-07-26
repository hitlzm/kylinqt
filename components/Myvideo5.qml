import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import VlcVideo 1.0
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 700; height: 430
    color: "#e9f0f9"

    property bool _connected: false
    property var magnifierWindow: null

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

    // 停止时的黑色遮罩层（覆盖在视频画面上方）
    Rectangle {
        id: stopOverlay
        anchors.fill: videoBorder
        color: "#000000"
        visible: false
        z: 1
    }

    // // 像素信息显示（悬浮于视频右下角）
    // Text {
    //     id: pixelInfo
    //     anchors.bottom: videoBorder.bottom; anchors.bottomMargin: 8
    //     anchors.right: videoBorder.right; anchors.rightMargin: 12
    //     color: "#00FF00"; font.pixelSize: 14
    //     text: "点击视频获取像素"
    //     z: 2
    // }

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
                else { videoPlayer.play(); stopOverlay.visible = false }
            }
        }
        CusButton_Blue { text: "停止"; Layout.fillWidth: true; height: 40
            onClicked: {
                videoPlayer.stop()
                stopOverlay.visible = true
            } }
        CusButton_Blue { text: root.magnifierWindow ? "关闭新窗口" : "🔍 放大"; Layout.fillWidth: true; height: 40
            onClicked: toggleMagnifier() }
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

    function toggleMagnifier() {
        if (magnifierWindow) {
            magnifierWindow.close()
            magnifierWindow = null
            console.log("Magnifier: window closed")
            return
        }

        console.log("Magnifier: creating component...")
        var comp = Qt.createComponent("qrc:/components/MagnifierWindow.qml")

        function doCreate() {
            console.log("Magnifier: component ready, creating window...")
            // Window 必须用 null 父对象，不能用 root
            magnifierWindow = comp.createObject(null, { "frameSource": videoPlayer })
            if (magnifierWindow) {
                console.log("Magnifier: window created, showing...")
                magnifierWindow.closing.connect(function() {
                    console.log("Magnifier: closing")
                    magnifierWindow = null
                })
                magnifierWindow.show()
            } else {
                console.warn("Magnifier: createObject returned null! Error:", comp.errorString())
            }
        }

        if (comp.status === Component.Ready) {
            doCreate()
        } else if (comp.status === Component.Error) {
            console.warn("Magnifier: load error:", comp.errorString())
        } else {
            console.log("Magnifier: loading...")
            comp.statusChanged.connect(function() {
                console.log("Magnifier: statusChanged ->", comp.status)
                if (comp.status === Component.Ready) doCreate()
                else if (comp.status === Component.Error) console.warn("Magnifier: load error:", comp.errorString())
            })
        }
    }

    Connections { target: videoPlayer
        function onPlayingChanged() { console.log("VlcVideo playing:", videoPlayer.playing) }
        function onEnded() { console.log("VlcVideo: 播放结束"); stopOverlay.visible = true }
        function onError(msg) { console.log("VlcVideo error:", msg) }
        function onStopped() { console.log("VlcVideo: 已停止"); stopOverlay.visible = true }
        // function onPixelRead(frameX, frameY) {
        //     pixelInfo.text = "帧坐标: (" + frameX + ", " + frameY + ")"
        //     console.log("帧坐标:", pixelInfo.text)
        // }
        function onReqDeviationToImg(x, y) {
            console.log("偏差像素请求: (" + x + ", " + y + ")")
            imageSendData.relayDeviationPixel(x, y)
        }
        function onErrorReadingPixel(msg) {
            console.warn("读取像素失败:", msg)
        }
    }
}
