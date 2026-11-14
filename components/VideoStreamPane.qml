import QtQuick 2.12
import QtQuick.Layouts 1.12
import VlcVideo 1.0
import taoQuick 1.0
import "./"

// 单路视频面板：连接状态、重连和断流检测全部按实例隔离。
// Myvideo5 中创建导引头和 CCD 两个实例，二者可以同时后台出帧；
// showHeader=false 时由外层统一显示视频源选择和连接状态。
Rectangle {
    id: root
    width: 338
    height: 372
    color: "#e9f0f9"

    property string paneTitle: "视频"
    property string defaultUrl: ""
    property bool isCcd: false
    property string playerObjectName: ""
    property bool magnifierVisible: false
    property bool showHeader: true

    readonly property var videoPlayerItem: videoPlayer
    readonly property bool streamConnected: _connected && !_stopped
    readonly property bool streamPlaying: _isPlaying && streamConnected
    readonly property bool ccdInferenceActive: isCcd && streamConnected
                                               && modeController.currentMode === 0
                                               && modeController.exguideSource === 5
                                               && modeController.exguidePeriod > 0

    signal magnifierToggleRequested()
    // 只有导引头画面的点击才应转发偏差像素；CCD 由视觉处理链生成目标中心。
    signal deviationPixelRequested(int frameX, int frameY)

    property bool _connected: false
    property bool _stopped: false
    property bool _isPlaying: false
    property bool _retrying: false
    property int _retryCount: 0
    readonly property int _maxRetries: 1
    property bool _failureDialogShown: false
    property real _lastWatchdogFrameSerial: 0
    property real _lastVideoFrameAt: 0
    readonly property int _streamStallTimeoutMs: 3000

    // CPU 回读仅服务 CCD 外引导推理，导引头始终直接使用 GPU 纹理显示。
    property bool cpuFrameConsumer: ccdInferenceActive
    onCpuFrameConsumerChanged: videoPlayer.setCpuFrameConsumer(cpuFrameConsumer)
    onStreamConnectedChanged: {
        if (isCcd)
            ccdData.setVideoStreamConnected(streamConnected)
    }

    // ========== 标题和连接状态 ==========
    RowLayout {
        id: paneHeader
        visible: root.showHeader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 26
        spacing: 6

        Text {
            text: root.paneTitle
            font.pixelSize: 18
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }

        Item { Layout.fillWidth: true }

        Rectangle {
            width: 10
            height: 10
            radius: 5
            Layout.alignment: Qt.AlignVCenter
            color: root._stopped ? "#9E9E9E"
                                : (root._connected
                                   ? (root._isPlaying ? "#4CAF50" : "#FF9800")
                                   : "#F44336")
        }

        Text {
            Layout.alignment: Qt.AlignVCenter
            font.pixelSize: 12
            color: "#333333"
            text: root._stopped ? "已停止"
                                : (root._connected
                                   ? (root._isPlaying ? "播放中" : "已就绪")
                                   : "未连接")
        }
    }

    // ========== 视频区域 ==========
    Rectangle {
        id: videoBorder
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        anchors.top: root.showHeader ? paneHeader.bottom : parent.top
        anchors.topMargin: root.showHeader ? 4 : 0
        height: root.showHeader ? 260 : 300
        color: "#202020"
        border.color: "#505050"
        border.width: 2
        radius: 8
    }

    VlcVideo {
        id: videoPlayer
        objectName: root.playerObjectName
        anchors.left: videoBorder.left
        anchors.leftMargin: 2
        anchors.right: videoBorder.right
        anchors.rightMargin: 2
        anchors.top: videoBorder.top
        anchors.topMargin: 2
        anchors.bottom: videoBorder.bottom
        anchors.bottomMargin: 2
    }

    Rectangle {
        id: stopOverlay
        anchors.fill: videoBorder
        color: "#000000"
        visible: false
        z: 1
    }

    Text {
        anchors.centerIn: videoBorder
        text: root._connected ? "" : "请设置视频流并点击「连接」"
        font.pixelSize: 16
        color: "#888888"
        z: 2
    }

    Timer {
        id: retryTimer
        interval: 2000
        repeat: false
        onTriggered: {
            root._retrying = false
            if (urlInput.text.trim() === "") {
                root._isPlaying = false
                return
            }
            root.connectToUrl(urlInput.text.trim(), true)
        }
    }

    Timer {
        id: streamWatchdog
        interval: 500
        repeat: true
        onTriggered: root.checkStreamAlive()
    }

    MessagePopup {
        id: connFailMsg
        onClosed: root._failureDialogShown = false
    }

    // ========== 操作区 ==========
    RowLayout {
        id: controlRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        anchors.top: videoBorder.bottom
        anchors.topMargin: 8
        height: 36
        spacing: 12

        CusButton_Blue {
            text: "停止"
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            onClicked: root.stopStream()
        }

        CusButton_Blue {
            text: root.magnifierVisible ? "关闭放大" : "🔍 放大"
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            onClicked: root.magnifierToggleRequested()
        }
    }

    RowLayout {
        id: urlRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        anchors.top: controlRow.bottom
        anchors.topMargin: 8
        height: 32
        spacing: 10

        Text {
            text: "视频流:"
            font.pixelSize: 16
            color: "#333333"
            Layout.alignment: Qt.AlignVCenter
        }

        CusTextField {
            id: urlInput
            Layout.fillWidth: true
            Layout.preferredHeight: 35
            font.pixelSize: 14
            onAccepted: root.connectToUrl(text.trim())
        }

        CusButton_Blue {
            text: root._connected ? "重连" : "连接"
            Layout.preferredWidth: 70
            Layout.preferredHeight: 32
            onClicked: root.connectToUrl(urlInput.text.trim())
        }
    }

    function connectToUrl(newUrl, isRetry) {
        if (newUrl === "")
            return

        retryTimer.stop()
        streamWatchdog.stop()
        _retrying = false
        if (!isRetry) {
            _retryCount = 0
            if (_failureDialogShown) {
                connFailMsg.close()
                _failureDialogShown = false
            }
        }

        _connected = false
        _stopped = false
        _isPlaying = false
        videoPlayer.ccdSource = isCcd
        videoPlayer.stop()
        // 相同 URL 重连时先清空，确保 mpv 强制重新加载。
        if (videoPlayer.source === newUrl)
            videoPlayer.source = ""
        videoPlayer.source = newUrl
        stopOverlay.visible = false
    }

    function stopStream() {
        retryTimer.stop()
        streamWatchdog.stop()
        _retrying = false
        _retryCount = 0
        videoPlayer.stop()
        videoPlayer.source = ""
        _connected = false
        _stopped = true
        _isPlaying = false
        stopOverlay.visible = true
    }

    function handleConnectionFailure(msg, notifyImmediately) {
        streamWatchdog.stop()
        _connected = false
        _stopped = false
        _isPlaying = false
        stopOverlay.visible = true
        if (_retrying)
            return

        const retryScheduled = _retryCount < _maxRetries
        if (retryScheduled) {
            _retryCount++
            _retrying = true
            retryTimer.start()
        }
        if (notifyImmediately || !retryScheduled) {
            connFailMsg.message = paneTitle + "：" + msg
            if (!connFailMsg.opened)
                connFailMsg.open()
            _failureDialogShown = true
        }
    }

    function checkStreamAlive() {
        if (_stopped || !_connected || !_isPlaying) {
            streamWatchdog.stop()
            return
        }

        const serial = videoPlayer.renderedFrameSerial()
        if (serial !== _lastWatchdogFrameSerial) {
            _lastWatchdogFrameSerial = serial
            _lastVideoFrameAt = Date.now()
            return
        }

        if (Date.now() - _lastVideoFrameAt >= _streamStallTimeoutMs) {
            streamWatchdog.stop()
            handleConnectionFailure(
                "视频流已中断，正在尝试重连，请检查网线或网络连接", true)
        }
    }

    function startStreamWatchdog() {
        _lastWatchdogFrameSerial = videoPlayer.renderedFrameSerial()
        _lastVideoFrameAt = Date.now()
        streamWatchdog.restart()
    }

    Component.onCompleted: {
        urlInput.text = defaultUrl
        videoPlayer.ccdSource = isCcd
        videoPlayer.setCpuFrameConsumer(cpuFrameConsumer)
        if (isCcd)
            ccdData.setVideoStreamConnected(false)
    }

    Connections {
        target: videoPlayer

        onPlayingChanged: {
            console.log(root.paneTitle + " VlcVideo playing:", videoPlayer.playing)
        }

        onVideoReady: {
            console.log(root.paneTitle + "：检测到真实视频流，连接成功")
            root._connected = true
            root._stopped = false
            root._retrying = false
            root._retryCount = 0
            retryTimer.stop()
            videoPlayer.play()
            root._isPlaying = true
            stopOverlay.visible = false
            root.startStreamWatchdog()
            if (root._failureDialogShown)
                connFailMsg.message = root.paneTitle + "：视频流已恢复"
        }

        onEnded: {
            console.log(root.paneTitle + " VlcVideo：播放结束")
            const wasConnected = root._connected && !root._stopped
            streamWatchdog.stop()
            root._connected = false
            root._isPlaying = false
            stopOverlay.visible = true
            if (wasConnected) {
                root.handleConnectionFailure(
                    "视频流已中断，正在尝试重连，请检查网线或网络连接", true)
            }
        }

        onError: {
            console.log(root.paneTitle + " VlcVideo error:", errorMsg)
            if (root._stopped)
                return
            const wasConnected = root._connected
            root.handleConnectionFailure(
                "视频流连接失败，请检查视频源地址或网络后重试", wasConnected)
        }

        onStopped: {
            console.log(root.paneTitle + " VlcVideo：已停止")
            streamWatchdog.stop()
            root._isPlaying = false
            stopOverlay.visible = true
        }

        onReqDeviationToImg: {
            console.log(root.paneTitle + " 偏差像素请求: (" + frameX + ", " + frameY + ")")
            if (!root.isCcd)
                root.deviationPixelRequested(frameX, frameY)
        }

        onErrorReadingPixel: {
            console.warn(root.paneTitle + " 读取像素失败:", message)
        }
    }
}
