import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "./"

// 双路视频容器：两路播放器都在后台保持渲染，RadioButton 只决定哪一路
// 位于上方的小窗中。放大镜始终绑定真实播放器，因此不受小窗选择影响。
Rectangle {
    id: root
    width: 700
    height: 430
    color: "#e9f0f9"

    readonly property var videoPlayerItem: guidePane.videoPlayerItem
    readonly property var guideVideoPlayerItem: guidePane.videoPlayerItem
    readonly property var ccdVideoPlayerItem: ccdPane.videoPlayerItem
    readonly property bool guideStreamConnected: guidePane.streamConnected
    readonly property bool ccdStreamConnected: ccdPane.streamConnected
    readonly property bool visibleStreamConnected: sourceGuide.checked
                                                   ? guidePane.streamConnected
                                                   : ccdPane.streamConnected
    readonly property bool visibleStreamPlaying: sourceGuide.checked
                                                 ? guidePane.streamPlaying
                                                 : ccdPane.streamPlaying

    signal guideMagnifierToggleRequested()
    signal ccdMagnifierToggleRequested()

    property bool guideMagnifierVisible: false
    property bool ccdMagnifierVisible: false
    ButtonGroup { id: videoSourceGroup }

    RowLayout {
        id: titleBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        anchors.topMargin: 2
        height: 34
        spacing: 12
        z: 2

        Text {
            text: "视频显示区"
            font.pixelSize: 22
            font.bold: true
            color: "#000000"
            Layout.alignment: Qt.AlignVCenter
        }

        Text {
            text: "视频源:"
            font.pixelSize: 14
            color: "#333333"
            Layout.alignment: Qt.AlignVCenter
        }

        RadioButton {
            id: sourceGuide
            text: "导引头"
            font.pixelSize: 13
            padding: 0
            checked: true
            ButtonGroup.group: videoSourceGroup
            Layout.alignment: Qt.AlignVCenter
        }

        RadioButton {
            id: sourceCCD
            text: "CCD"
            font.pixelSize: 13
            padding: 0
            ButtonGroup.group: videoSourceGroup
            Layout.alignment: Qt.AlignVCenter
        }

        Item { Layout.fillWidth: true }

        Row {
            spacing: 6
            Layout.alignment: Qt.AlignVCenter

            Rectangle {
                width: 12
                height: 12
                radius: 6
                anchors.verticalCenter: parent.verticalCenter
                color: root.visibleStreamConnected
                       ? (root.visibleStreamPlaying ? "#4CAF50" : "#FF9800")
                       : "#F44336"
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                color: "#333333"
                text: root.visibleStreamConnected
                      ? (root.visibleStreamPlaying ? "播放中" : "已就绪")
                      : "未连接"
            }
        }
    }

    // 两路都保持 visible=true，只靠 z 切换最终覆盖关系。
    // 不能把非当前路设为 visible=false，否则 QQuickFramebufferObject 可能停止
    // 渲染该路视频，进而导致它自己的放大窗没有新纹理。
    Item {
        id: displayStack
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleBar.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 8
        anchors.bottomMargin: 0
        z: 1

        VideoStreamPane {
            id: guidePane
            anchors.fill: parent
            paneTitle: "导引头"
            defaultUrl: "udp://@226.0.0.80:8001"
            isCcd: false
            playerObjectName: "guideVideoPlayer"
            showHeader: false
            z: sourceGuide.checked ? 2 : 1
            magnifierVisible: root.guideMagnifierVisible
            onMagnifierToggleRequested: root.guideMagnifierToggleRequested()
            onDeviationPixelRequested: {
                imageSendData.relayDeviationPixel(
                    frameX, frameY, imageData.infraredFrameNum,
                    imageData.cbhTv4405, imageSendData.m_templateIndex)
            }
        }

        VideoStreamPane {
            id: ccdPane
            anchors.fill: parent
            paneTitle: "CCD"
            defaultUrl: "rtsp://admin:abcd1234@192.168.1.68:554/Streaming/Channels/101"
            isCcd: true
            playerObjectName: "ccdVideoPlayer"
            showHeader: false
            z: sourceCCD.checked ? 2 : 1
            magnifierVisible: root.ccdMagnifierVisible
            onMagnifierToggleRequested: root.ccdMagnifierToggleRequested()
        }
    }
}
