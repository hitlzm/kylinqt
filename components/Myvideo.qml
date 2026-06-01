import QtQuick 2.12
import QtQuick.Controls 2.12
import VLCQt 1.0
import taoQuick 1.0
Rectangle {
    id: root
    width: 700
    height: 400
    color: "#202020"

    // 播放器
    VlcVideoPlayer {
        id: mediaPlayer
        anchors.fill: parent
        url: vlcplayer.url
        
    }

    // // 视频显示框
    // Rectangle {
    //     id: videoFrame
    //     width: 640
    //     height: 360
    //     anchors.centerIn: parent

    //     color: "black"
    //     border.color: "#505050"
    //     border.width: 2
    //     radius: 8
    //     clip: true
    
    // }

    // 底部控制栏
    Rectangle {
        width: parent.width
        height: 40
        anchors.bottom: parent.bottom
        color: "#2B2B2B"

        Row {
            spacing: 20
            anchors.centerIn: parent

            CusButton_Blue {
                text: "播放"
                onClicked: vlcplayer.play()
            }
            CusButton_Blue {
                text: "暂停"
                onClicked: vlcplayer.pause()
            }
            CusButton_Blue {
                text: "停止"
                onClicked: vlcplayer.stop()
            }
        }
    }
}
