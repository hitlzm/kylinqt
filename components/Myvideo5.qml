import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import VlcVideo 1.0
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 700; height: 450
    color: "#e9f0f9"

    property bool _connected: false
    property bool _stopped: false
    property bool _retrying: false
    property int _retryCount: 0
    property int _maxRetries: 2
    property var magnifierWindow: null

    // ── CPU 帧消费开关（导引头/CCD 模式切换处翻转）──
    // 导引头模式（视频直接显示）→ false：渲染线程跳过 glReadPixels，显示/放大镜纯 GPU、零回读
    // CCD 模式（需 YOLO 检测）  → true ：保留 CPU 回读供 StreamProcessor 取帧
    // 默认 true = 保持现有行为；在接入/切换视频源时按模式置位即可
    property bool cpuFrameConsumer: true
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
                color: _stopped ? "#9E9E9E" : (videoPlayer.playing ? "#4CAF50" : (_connected ? "#FF9800" : "#F44336")) }
            Text { anchors.verticalCenter: parent.verticalCenter; font.pixelSize: 16; color: "#333333"
                text: _stopped ? "已停止" : (videoPlayer.playing ? "播放中" : (_connected ? "已就绪" : "未连接")) }
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

    // 连接失败后自动重连：等待 3 秒重新加载并播放；次数耗尽后由 connFailMsg 弹窗提示
    Timer {
        id: retryTimer
        interval: 3000
        repeat: false
        onTriggered: {
            _retrying = false
            connectToUrl(urlInput.text.trim(), true)
            videoPlayer.play()
        }
    }

    MessagePopup { id: connFailMsg }

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

        CusButton_Blue {
            text: videoPlayer.playing ? "暂停" : "播放"
            Layout.fillWidth: true; Layout.preferredHeight: 32
            onClicked: {
                if (!_connected) {
                    _retryCount = 0
                    connectToUrl(urlInput.text.trim())
                }
                _stopped = false
                if (videoPlayer.playing) videoPlayer.pause()
                else { videoPlayer.play(); stopOverlay.visible = false }
            }
        }
        CusButton_Blue { text: "停止"; Layout.fillWidth: true; Layout.preferredHeight: 32
            onClicked: {
                retryTimer.stop()
                _retrying = false
                _retryCount = 0
                videoPlayer.stop()
                _stopped = true
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
        anchors.top: controlRow.bottom; anchors.topMargin: 8
        // 固定行高：输入框和连接按钮不再随麒麟字体行高变化，杜绝溢出父边界
        height: 32
        spacing: 10
        Text { text: "RTSP:"; font.pixelSize: 16; color: "#333333"; Layout.alignment: Qt.AlignVCenter }
        CusTextField { id: urlInput; Layout.fillWidth: true; Layout.preferredHeight: 26; font.pixelSize: 14; onAccepted: connectToUrl(text.trim()) }
        CusButton_Blue { text: "连接"; Layout.preferredWidth: 70; Layout.preferredHeight: 32; onClicked: connectToUrl(urlInput.text.trim()) }
    }

    // 连接：只有 mpv 真正加载成功（onFileLoaded）后才置 _connected = true；
    // 失败时保持“未连接”，由 onError 触发自动重连。
    function connectToUrl(newUrl, isRetry) {
        if (newUrl === "") return
        retryTimer.stop()
        _retrying = false
        if (!isRetry) _retryCount = 0
        _connected = false
        _stopped = false
        videoPlayer.stop()
        // 先清空再赋值：重连相同 URL 时也能强制重新加载
        videoPlayer.source = ""
        videoPlayer.source = newUrl
        stopOverlay.visible = false
    }

    // 视频源选择：CCD → 保留 CPU 回读供 YOLO 检测；导引头 → 跳过回读纯 GPU 显示
    function selectSource(isCCD) {
        cpuFrameConsumer = isCCD
        // 选中导引头时默认填入导引头 RTSP 流地址   ,  测试时先使用ts流（udp://@226.0.0.80:8001），后期可换成RTSP
        // if (!isCCD && urlInput) urlInput.text = "rtsp://192.168.1.100:554/stream"
        if (!isCCD && urlInput) urlInput.text = "udp://@226.0.0.80:8001"
    }

    // 初始默认导引头模式：关闭 CPU 回读 + 填入默认 RTSP（此时子对象均已创建）
    Component.onCompleted: {
        sourceGuide.checked = true
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

    Connections { target: videoPlayer
        function onPlayingChanged() { console.log("VlcVideo playing:", videoPlayer.playing) }
        function onFileLoaded() {
            console.log("VlcVideo: 流已加载，连接成功")
            _connected = true
            _stopped = false
            _retrying = false
            _retryCount = 0
            retryTimer.stop()
            stopOverlay.visible = false
        }
        function onEnded() { console.log("VlcVideo: 播放结束"); stopOverlay.visible = true }
        function onError(msg) {
            console.log("VlcVideo error:", msg)
            if (_stopped) return          // 用户已主动停止，不再自动重连
            _connected = false
            _stopped = false
            stopOverlay.visible = false
            if (_retrying) return         // 重连进行中，忽略重复错误
            if (_retryCount < _maxRetries) {
                _retryCount++
                _retrying = true
                retryTimer.start()
            } else {
                connFailMsg.message = "视频流连接失败，请检查视频源地址或网络后重试"
                connFailMsg.open()
            }
        }
        function onStopped() { console.log("VlcVideo: 已停止"); stopOverlay.visible = true }
        function onReqDeviationToImg(x, y) {
            console.log("偏差像素请求: (" + x + ", " + y + ")")
            imageSendData.relayDeviationPixel(x, y)
        }
        function onErrorReadingPixel(msg) {
            console.warn("读取像素失败:", msg)
        }
    }
}
