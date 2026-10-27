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
    property bool _stopped: false
    property bool _isPlaying: false
    property bool _retrying: false
    property int _retryCount: 0
    property int _maxRetries: 1
    property var magnifierWindow: null
    property bool _failureDialogShown: false
    property real _lastWatchdogFrameSerial: 0
    property real _lastVideoFrameAt: 0
    readonly property int _streamStallTimeoutMs: 3000
    // _connectedSourceIsCCD 记录当前真正连上的流是否来自 CCD，
    // 避免切换到导引头单选后仍沿用上一路视频的连接状态。
    property bool _connectedSourceIsCCD: false
    readonly property bool ccdVideoConnected: _connected && sourceCCD.checked
                                              && _connectedSourceIsCCD && !_stopped
    onCcdVideoConnectedChanged: ccdData.setVideoStreamConnected(ccdVideoConnected)

    // ── CPU 帧消费开关 ────────────────────────────────────────────────
    // 仅在“CCD 视频已连接 + 外引导模式 + CCD 外引导源 + 已选跟踪周期”
    // 四项同时成立时回读 CPU 帧并执行视觉推理。只选中 CCD 或只连接流时，
    // 视频走 GPU 直显，逐帧 glReadPixels 和 YOLO 都不会启动。
    readonly property bool ccdInferenceActive: ccdVideoConnected
                                               && modeController.currentMode === 0
                                               && modeController.exguideSource === 5
                                               && modeController.exguidePeriod > 0
    property bool cpuFrameConsumer: ccdInferenceActive
    onCpuFrameConsumerChanged: videoPlayer.setCpuFrameConsumer(cpuFrameConsumer)

    // 视频源选择互斥组（标题栏 radio）
    ButtonGroup { id: videoSourceGroup }

    // ========== 顶部标题栏 ==========
    RowLayout {
        id: titleBar
        anchors.left: parent.left;  anchors.leftMargin: 12
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: parent.top;     anchors.topMargin: 10
        // 固定高度：避免麒麟下 22px 标题行高变大，把下方控件挤出 430 底边
        height: 26
        spacing: 12

        Text { text: "视频显示区"; font.pixelSize: 22; font.bold: true; color: "#000000"; Layout.alignment: Qt.AlignVCenter }

        // ── 视频源选择：导引头（纯显示，跳过CPU回读）/ CCD（保留回读供检测）──
        Text { text: "视频源:"; font.pixelSize: 14; color: "#333333"; Layout.alignment: Qt.AlignVCenter }
        RadioButton {
            id: sourceGuide
            text: "导引头"
            font.pixelSize: 13
            padding: 0
            ButtonGroup.group: videoSourceGroup
            Layout.alignment: Qt.AlignVCenter
            onCheckedChanged: if (checked) selectSource(false)
        }
        RadioButton {
            id: sourceCCD
            text: "CCD"
            font.pixelSize: 13
            padding: 0
            ButtonGroup.group: videoSourceGroup
            Layout.alignment: Qt.AlignVCenter
            onCheckedChanged: if (checked) selectSource(true)
        }

        Item { Layout.fillWidth: true }

        Row {
            spacing: 6; Layout.alignment: Qt.AlignVCenter
            Rectangle { width: 12; height: 12; radius: 6; anchors.verticalCenter: parent.verticalCenter
                color: _stopped ? "#9E9E9E" : (_connected ? (_isPlaying ? "#4CAF50" : "#FF9800") : "#F44336") }
            Text { anchors.verticalCenter: parent.verticalCenter; font.pixelSize: 16; color: "#333333"
                text: _stopped ? "已停止" : (_connected ? (_isPlaying ? "播放中" : "已就绪") : "未连接") }
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

    // 连接失败后自动重连：等待 2 秒重新加载；仍失败由 connFailMsg 弹窗提示
    Timer {
        id: retryTimer
        interval: 2000
        repeat: false
        onTriggered: {
            _retrying = false
            if (urlInput.text.trim() === "") {
                _isPlaying = false
                return
            }
            // 重连只建立连接，连接成功后自动播放
            connectToUrl(urlInput.text.trim(), true)
        }
    }

    // 直播流断网时，mpv 往往只停在最后一帧，不会发出 error/end-file。
    // 因此以“实际渲染出的新帧”为准：3 秒没有新帧就判定中断。
    Timer {
        id: streamWatchdog
        interval: 500
        repeat: true
        onTriggered: checkStreamAlive()
    }

    MessagePopup {
        id: connFailMsg
        onClosed: root._failureDialogShown = false
    }
    MessagePopup { id: connectFirstMsg }

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
        anchors.top: videoBorder.bottom; anchors.topMargin: 8
        // 固定行高：行内按钮不再随麒麟字体行高变化
        height: 36
        spacing: 12

        // CusButton_Blue {
        //     text: "播放"
        //     Layout.fillWidth: true; Layout.preferredHeight: 32
        //     onClicked: {
        //         // 只保留播放功能（不提供暂停）；视频源未就绪时提示先连接
        //         if (!_connected) {
        //             connectFirstMsg.message = "请先连接视频源"
        //             connectFirstMsg.open()
        //             _isPlaying = false
        //             return
        //         }
        //         _stopped = false
        //         videoPlayer.play()
        //         _isPlaying = true
        //         stopOverlay.visible = false
        //     }
        // }
        CusButton_Blue { text: "停止"; Layout.fillWidth: true; Layout.preferredHeight: 32
            onClicked: {
                // 停止 = 断开当前视频源：停止播放并清空源，再次播放需重新连接
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
            } }
        CusButton_Blue { text: root.magnifierWindow ? "关闭新窗口" : "🔍 放大"; Layout.fillWidth: true; height: 40
            onClicked: toggleMagnifier() }
        // Text { text: "🔈"; font.pixelSize: 18; Layout.alignment: Qt.AlignVCenter }
        // CusSlider { id: volumeSlider; Layout.preferredWidth: 120; showNumber: true; from: 0; to: 100
        //     value: videoPlayer.volume; onMoved: videoPlayer.setVolume(value) }
        // CusLabel { text: Math.round(videoPlayer.volume ) + "%"; font.pixelSize: 14; Layout.preferredWidth: 45; horizontalAlignment: Text.AlignHCenter }
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
        Text { text: "视频流:"; font.pixelSize: 16; color: "#333333"; Layout.alignment: Qt.AlignVCenter }
        CusTextField { id: urlInput; Layout.fillWidth: true; Layout.preferredHeight: 35; font.pixelSize: 14; onAccepted: connectToUrl(text.trim()) }
        CusButton_Blue { text: "连接"; Layout.preferredWidth: 70; Layout.preferredHeight: 32; onClicked: connectToUrl(urlInput.text.trim()) }
    }

    // 连接：只有 mpv 解析到真实视频流（onVideoReady）后才置 _connected = true；
    // 失败时保持“未连接”，由 onError 失败信号触发自动重连。
    function connectToUrl(newUrl, isRetry) {
        if (newUrl === "") return
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
        _connectedSourceIsCCD = sourceCCD.checked
        _stopped = false
        _isPlaying = false
        videoPlayer.stop()
        // 相同 URL 重连时先清空，确保强制重新加载
        if (videoPlayer.source === newUrl) videoPlayer.source = ""
        videoPlayer.source = newUrl
        stopOverlay.visible = false
    }

    // 连接失败统一处理（由连接返回的失败信号触发）：
    // 置未连接 → 2 秒后自动重连（最多 _maxRetries 次）→ 仍失败则弹窗提示
    function handleConnectionFailure(msg, notifyImmediately) {
        streamWatchdog.stop()
        _connected = false
        _stopped = false
        _isPlaying = false
        stopOverlay.visible = true
        if (_retrying) return
        const retryScheduled = _retryCount < _maxRetries
        if (retryScheduled) {
            _retryCount++
            _retrying = true
            retryTimer.start()
        }
        // 已连接后的中断立即提示；首次连接失败仍先静默重试一次。
        if (notifyImmediately || !retryScheduled) {
            connFailMsg.message = msg
            if (!connFailMsg.opened) connFailMsg.open()
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

    // 视频源选择只决定取流地址；是否回读 CPU 帧由 ccdInferenceActive 决定。
    function selectSource(isCCD) {
        // 切换视频源时填入对应默认地址。
        if (isCCD && urlInput) {
            urlInput.text = "rtsp://admin:abcd1234@192.168.1.68:554/Streaming/Channels/101"
        } else if (!isCCD && urlInput) {
            // 测试时先使用 TS 流，后期可换成导引头 RTSP 地址。
            urlInput.text = "udp://@226.0.0.80:8001"
        }
    }

    // 初始默认导引头模式：关闭 CPU 回读 + 填入默认 RTSP（此时子对象均已创建）
    Component.onCompleted: {
        sourceGuide.checked = true
        // VlcVideoItem 的默认值与本页门控绑定可能相同，不会触发
        // onCpuFrameConsumerChanged，因此初始化时显式同步一次。
        videoPlayer.setCpuFrameConsumer(cpuFrameConsumer)
    }

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

    Connections {
        target: videoPlayer
        // 注意：Qt 5.12 不支持 Connections 里的 function onXxx() 写法，
        // 必须使用 onXxx: 属性语法，否则信号处理器不会触发。
        onPlayingChanged: {
            // 注意：playing 只是 pause 标志，不代表流已连通（UDP 打开协议就会上报 true），
            // 不能用它判定连接成功；连接成功只看 onVideoReady（mpv 解析到真实视频参数）
            console.log("VlcVideo playing:", videoPlayer.playing)
        }
        onVideoReady: {
            // mpv 解析到真实视频参数（video-params）才算连接成功；
            // 仅打开 UDP/RTSP 协议、没有视频数据时不会触发
            console.log("VlcVideo: 检测到真实视频流，连接成功")
            _connected = true
            _connectedSourceIsCCD = sourceCCD.checked
            _stopped = false
            _retrying = false
            _retryCount = 0
            retryTimer.stop()
            // 连接成功立即播放
            videoPlayer.play()
            _isPlaying = true
            stopOverlay.visible = false
            startStreamWatchdog()
            if (_failureDialogShown)
                connFailMsg.message = "视频流已恢复"
        }
        onEnded: {
            console.log("VlcVideo: 播放结束")
            const wasConnected = _connected && !_stopped
            streamWatchdog.stop()
            _connected = false
            _connectedSourceIsCCD = false
            _isPlaying = false
            stopOverlay.visible = true
            if (wasConnected)
                handleConnectionFailure(
                    "视频流已中断，正在尝试重连，请检查网线或网络连接", true)
        }
        onError: {
            console.log("VlcVideo error:", errorMsg)
            if (_stopped) return          // 用户已主动停止，不再自动重连
            const wasConnected = _connected
            handleConnectionFailure(
                "视频流连接失败，请检查视频源地址或网络后重试", wasConnected)
        }
        onStopped: {
            console.log("VlcVideo: 已停止")
            streamWatchdog.stop()
            _isPlaying = false
            stopOverlay.visible = true
        }
        onReqDeviationToImg: {
            console.log("偏差像素请求: (" + frameX + ", " + frameY + ")")
            imageSendData.relayDeviationPixel(frameX, frameY,imageData.infraredFrameNum,imageData.cbhTv4405,imageSendData.m_templateIndex)   //再加入红外/电视画面帧，模板序号 一并传输
        }
        onErrorReadingPixel: {
            console.warn("读取像素失败:", message)
        }
    }
}
