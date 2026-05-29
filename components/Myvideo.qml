import QtQuick 2.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12

Rectangle {
    id: root
    width: 700
    height: 400
    color: "#202020"

    // =========================
    // 视频播放器
    // =========================
    MediaPlayer {
        id: mediaPlayer

        // 本地视频
        source: "null"

        // RTSP 示例
        // source: "rtsp://192.168.1.100:554/live"

        autoPlay: false
    }

    // =========================
    // 视频显示框
    // =========================
    Rectangle {
        id: videoFrame

        width: 640
        height: 360
        anchors.centerIn: parent

        color: "black"
        border.color: "#505050"
        border.width: 2
        radius: 8

        clip: true

        VideoOutput {
            id: videoOutput

            anchors.fill: parent
            source: mediaPlayer

            fillMode: VideoOutput.PreserveAspectFit
        }

        // 无视频时显示文字
        Text {
            anchors.centerIn: parent

            visible: mediaPlayer.playbackState !== MediaPlayer.PlayingState

            text: "Video Preview"
            color: "#808080"
            font.pixelSize: 28
        }

        
    }

    // =========================
    // 底部控制栏
    // =========================
    Rectangle {
        width: parent.width
        height: 40

        anchors.bottom: parent.bottom

        color: "#2B2B2B"

        Row {
            spacing: 20
            anchors.centerIn: parent

            Button {
                text: "播放"

                onClicked: {
                    mediaPlayer.play()
                }
            }

            Button {
                text: "暂停"

                onClicked: {
                    mediaPlayer.pause()
                }
            }

            Button {
                text: "停止"

                onClicked: {
                    mediaPlayer.stop()
                }
            }
        }
    }
}