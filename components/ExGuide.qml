import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0


Item {
    id: root
    anchors.fill: parent

    // 1s 跟踪模式允许发送一次三轴跟踪偏差修正
    readonly property bool guide1sSelected: period250ms.checked

    function applyTrackingCorrection() {
        turntableSendData.requestTrackingCorrection(
                    turntableData.inner_ctlDeviation,
                    turntableData.middle_ctlDeviation,
                    turntableData.outter_ctlDeviation)
    }

    // 用户确认视频画面已调整为正后，才计算并填入内框偏置修正角。
    function confirmInnerAngleZeroCorrection() {
        turntableSendData.requestInnerAngleZeroCorrection()
        innerAngleZeroPopup.close()
    }

    // 外引导源不可用：C++ 侧已停止对应外引导，
    // 界面只取消对应源选择，保留外引导模式和跟踪周期设置。
    function clearSource(source) {
        if ((source === 3 && guideImage.checked)
                || (source === 4 && guideLaser.checked)
                || (source === 5 && guideCCD.checked)) {
            guideSourceGroup.checkState = Qt.Unchecked
        }
    }

    // ExGuide 页面会随控制模式切换而重新加载，重新加载后从控制器恢复选择。
    function syncSelection() {
        var source = modeController.exguideSource
        if (source === 4) guideLaser.checked = true
        else if (source === 3) guideImage.checked = true
        else if (source === 5) guideCCD.checked = true
        else guideSourceGroup.checkState = Qt.Unchecked

        var period = modeController.exguidePeriod
        if (period === 7) period250ms.checked = true
        else if (period === 6) period20ms.checked = true
        else guidePeriodGroup.checkState = Qt.Unchecked
    }

    Connections {
        target: modeController
        onExguideSourceLost: root.clearSource(source)
        onExguideSourceChanged: root.syncSelection()
        onExguidePeriodChanged: root.syncSelection()
    }

    // 日志条目由 C++ LogManager 单例提供，格式已包含时间戳和来源标签
    // 可通过 logManager.addLog("来源", "消息") 从任意 QML 或 C++ 添加日志
    // 可通过 logManager.clearLog() 清空日志

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
            onCheckedChanged: {
                if (checked) modeController.setExguideSource(4)
            }
        }
        CusRadioButton {
            id: guideImage
            width: 140
            text: "图像导引头"
            font.pixelSize: 16
            ButtonGroup.group: guideSourceGroup
            onCheckedChanged: {
                if (checked) modeController.setExguideSource(3)
            }
        }
        CusRadioButton {
            id: guideCCD
            width: 120
            text: "CCD相机"
            font.pixelSize: 16
            ButtonGroup.group: guideSourceGroup
            onCheckedChanged: {
                if (checked) modeController.setExguideSource(5)
            }
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
            color: "#000000"
            anchors.verticalCenter: parent.verticalCenter
        }
        ButtonGroup { id: guidePeriodGroup }
        CusRadioButton {
            id: period250ms
            width: 120
            text: "1s"
            font.pixelSize: 16
            ButtonGroup.group: guidePeriodGroup
            onCheckedChanged: {
                if (checked) modeController.setExguidePeriod(7)
            }
        }
        CusRadioButton {
            id: period20ms
            width: 120
            text: "20ms"
            font.pixelSize: 16
            ButtonGroup.group: guidePeriodGroup
            onCheckedChanged: {
                if (checked) modeController.setExguidePeriod(6)
            }
        }
        CusButton_Blue {
            id: innerAngleZeroButton
            width: 100
            height: 28
            text: "零位修正"
            font.pixelSize: 14
            anchors.verticalCenter: parent.verticalCenter
            onClicked: innerAngleZeroPopup.open()
        }
    }

    // 零位修正会以点击确认时的内框角度计算偏置，因此打开弹窗本身不修改任何数值。
    Popup {
        id: innerAngleZeroPopup
        parent: Overlay.overlay
        width: Math.min(560, parent ? parent.width - 40 : 560)
        height: 190
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)

        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose

        Overlay.modal: Rectangle {
            color: "#70000000"
        }

        background: Rectangle {
            radius: 10
            color: "white"
            border.color: "#D0D0D0"
            border.width: 1
        }

        Column {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 22

            Text {
                width: parent.width
                text: "请先确认导引头视频画面为正，再利用内框角度值进行修正"
                font.pixelSize: 20
                font.bold: true
                color: "#333333"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 18

                CusButton_White {
                    width: 110
                    height: 38
                    text: "取消"
                    font.pixelSize: 16
                    onClicked: innerAngleZeroPopup.close()
                }

                CusButton_Blue {
                    width: 110
                    height: 38
                    text: "确认"
                    font.pixelSize: 16
                    onClicked: root.confirmInnerAngleZeroCorrection()
                }
            }
        }
    }

    Component.onCompleted: {
        // 每次进入外引导页都从空日志开始，避免上次会话的滞留条目。
        logManager.clearLog()
        root.syncSelection()
    }

    Rectangle {
        id: logArea
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
            text: "外引导跟踪信息"; font.pixelSize: 18; color: '#000000'
        }

        // 仅 1s 跟踪模式显示；修正量取转台当前三轴控制偏差
        CusButton_Blue {
            id: correctionButton
            anchors.top: parent.top; anchors.topMargin: 3
            anchors.right: parent.right; anchors.rightMargin: 8
            width: 96
            height: 26
            text: "跟踪修正"
            font.pixelSize: 14
            visible: root.guide1sSelected
            z: 2
            onClicked: root.applyTrackingCorrection()
        }

        // 日志列表
        CusListView {
            id: logList
            anchors.top: parent.top
            anchors.topMargin: correctionButton.visible ? 34 : 22
            anchors.left: parent.left;   anchors.leftMargin: 5
            anchors.right: parent.right; anchors.rightMargin: 5
            anchors.bottom: parent.bottom; anchors.bottomMargin: 4
            clip: true
            spacing: 1

            model: logManager.logEntries

            delegate: Text {
                text: modelData
                font.pixelSize: 16; font.family: "Consolas"
                color: '#1e0101'
                width: logList.width
                horizontalAlignment: Text.AlignHCenter
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

    // 新日志到达时自动滚到底部
    Connections {
        target: logManager
        onLogEntriesChanged: {
            logList.positionViewAtEnd()  //新日志到达时自动滚到底部
        }
    }
}
