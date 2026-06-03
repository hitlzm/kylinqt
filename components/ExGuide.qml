import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0


Item {
    id: root

    // 供外部调用的日志接口：ExGuide.addLog("消息内容")
    function addLog(msg) {
        var now = new Date()
        var ts = now.toLocaleTimeString(Qt.locale(), "hh:mm:ss")
        logModel.append({ text: "[" + ts + "] " + msg })
        // 超过 200 条时移除最老的
        if (logModel.count > 200) {
            logModel.remove(0, logModel.count - 200)
        }
        // 自动滚到底部
        logList.positionViewAtEnd()
    }

    // 清空日志
    function clearLog() {
        logModel.clear()
    }

    Rectangle {
        anchors.fill: parent
        color: '#ebe7e7'
        border.color: "#505050"
        border.width: 1
        radius: 4

        // 标题
        Text {
            anchors.top: parent.top; anchors.topMargin: 4
            anchors.left: parent.left; anchors.leftMargin: 10
            text: "日志"; font.pixelSize: 18; color: '#000000'
        }

        // 日志列表
        CusListView {
            id: logList
            anchors.top: parent.top; anchors.topMargin: 22
            anchors.left: parent.left;   anchors.leftMargin: 5
            anchors.right: parent.right; anchors.rightMargin: 5
            anchors.bottom: parent.bottom; anchors.bottomMargin: 4
            clip: true
            spacing: 1

            model: ListModel { id: logModel }

            delegate: Text {
                text: model.text
                font.pixelSize: 12; font.family: "Consolas"
                color: '#1e0101'
                width: logList.width
                wrapMode: Text.Wrap
            }

            // 滚动条
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
                width: 6
                contentItem: Rectangle {
                    color: "#555555"; radius: 3
                }
            }
        }
    }
}
