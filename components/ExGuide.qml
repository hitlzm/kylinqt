import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0


Item {
    id: root
    anchors.fill: parent

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

    // 外引导源选择
    Row {
        id: guideSourceRow
        anchors.top: parent.top
        anchors.topMargin: 4
        // anchors.left: parent.left
        // anchors.leftMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 12

        Text {
            text: "外引导源选择:"
            font.pixelSize: 17
            font.bold: true
            anchors.verticalCenter: parent.verticalCenter
        }
        ButtonGroup { id: guideSourceGroup }
        CusRadioButton {
            id: guideLaser
            width: 140
            text: "激光导引头"
            font.pixelSize: 16
            ButtonGroup.group: guideSourceGroup
        }
        CusRadioButton {
            id: guideImage
            width: 140
            text: "图像导引头"
            font.pixelSize: 16
            ButtonGroup.group: guideSourceGroup
        }
        CusRadioButton {
            id: guideCCD
            width: 120
            text: "CCD相机"
            font.pixelSize: 16
            ButtonGroup.group: guideSourceGroup
        }
    }

    // 外引导跟踪周期
    Row {
        id: guidePeriodRow
        anchors.top: guideSourceRow.bottom
        anchors.topMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 12

        Text {
            text: "外引导跟踪周期:"
            font.pixelSize: 17
            font.bold: true
            color: black
            anchors.verticalCenter: parent.verticalCenter
        }
        ButtonGroup { id: guidePeriodGroup }
        CusRadioButton {
            id: period250ms
            width: 120
            text: "250ms"
            font.pixelSize: 16
            ButtonGroup.group: guidePeriodGroup
        }
        CusRadioButton {
            id: period5ms
            width: 120
            text: "5ms"
            font.pixelSize: 16
            ButtonGroup.group: guidePeriodGroup
        }
    }

    Rectangle {
        anchors.top: guidePeriodRow.bottom
        anchors.topMargin: 4
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
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
