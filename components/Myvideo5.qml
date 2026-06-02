import QtQuick 2.12
import QtQuick.Window 2.12
import VlcVideo 1.0

Window {
    visible: true
    width: 800
    height: 600

    VlcVideo {
        anchors.fill: parent
        source: "rtmp://liteavapp.qcloud.com/live/liteavdemoplayerstreamid"
    }
}
