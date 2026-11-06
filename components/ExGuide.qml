import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0


Item {
    id: root
    anchors.fill: parent

    // 1s 跟踪模式允许发送一次三轴跟踪偏差修正
    readonly property bool guide1sSelected: period250ms.checked
    property string zeroCorrectionStage: "choose"
    property string zeroCorrectionError: ""

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

    function showImageZeroCorrection() {
        zeroCorrectionStage = "image"
        zeroCorrectionError = ""
    }

    function showLaserZeroCorrection() {
        zeroCorrectionStage = "laser"
        zeroCorrectionError = ""
        laserInnerAngleOffsetInput.text = ""
        laserInnerAngleOffsetInput.forceActiveFocus()
    }

    function confirmLaserZeroCorrection() {
        if (laserInnerAngleOffsetInput.text.trim() === ""
                || !laserInnerAngleOffsetInput.inputFormatValid) {
            zeroCorrectionError = laserInnerAngleOffsetInput.text.trim() === ""
                    ? "请输入激光头零位修正角"
                    : laserInnerAngleOffsetInput.numericError
            return
        }

        turntableSendData.requestLaserInnerAngleZeroCorrection(
                    laserInnerAngleOffsetInput.numericValue)
        innerAngleZeroPopup.close()
    }

    function closeZeroCorrectionPopup() {
        zeroCorrectionStage = "choose"
        zeroCorrectionError = ""
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

    // 图像头零位修正会按点击确认时的内框角度计算偏置；
    // 激光头零位修正只在用户确认输入的偏置角后才修改数值。
    Popup {
        id: innerAngleZeroPopup
        parent: Overlay.overlay
        width: Math.min(680, parent ? parent.width - 48 : 680)
        height: zeroCorrectionStage === "choose" ? 300
                : (zeroCorrectionStage === "image" ? 350 : 390)
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)

        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose
        onClosed: {
            root.zeroCorrectionStage = "choose"
            root.zeroCorrectionError = ""
        }

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
            width: parent.width - 64
            anchors.centerIn: parent
            spacing: 24

            Text {
                width: parent.width
                text: root.zeroCorrectionStage === "choose"
                      ? "请选择需要修正零位的导引头"
                      : (root.zeroCorrectionStage === "image"
                         ? "图像头零位修正"
                         : "激光头零位修正")
                font.pixelSize: 26
                font.bold: true
                color: "#333333"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
            }

            Text {
                width: parent.width
                visible: root.zeroCorrectionStage === "image"
                text: "请先确认图像头视频画面为正，再进行外引导零位修正"
                font.pixelSize: 20
                lineHeight: 1.45
                lineHeightMode: Text.ProportionalHeight
                color: "#333333"
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            Text {
                width: parent.width
                visible: root.zeroCorrectionStage === "laser"
                text: "请输入激光头零位修正角，确认后将立即用于激光头外引导坐标变换"
                font.pixelSize: 19
                lineHeight: 1.45
                lineHeightMode: Text.ProportionalHeight
                color: "#333333"
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            Row {
                visible: root.zeroCorrectionStage === "choose"
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 18

                CusButton_Blue {
                    width: 190
                    height: 50
                    text: "图像头零位修正"
                    font.pixelSize: 19
                    onClicked: root.showImageZeroCorrection()
                }

                CusButton_Blue {
                    width: 190
                    height: 50
                    text: "激光头零位修正"
                    font.pixelSize: 19
                    onClicked: root.showLaserZeroCorrection()
                }
            }

            Row {
                visible: root.zeroCorrectionStage === "laser"
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10

                Text {
                    text: "零位修正角:"
                    font.pixelSize: 19
                    color: "#333333"
                    anchors.verticalCenter: parent.verticalCenter
                }

                NumericTextField {
                    id: laserInnerAngleOffsetInput
                    width: 210
                    height: 46
                    font.pixelSize: 19
                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                    placeholderText: "请输入角度"
                    fieldLabel: "激光头零位修正角"
                    maximumFractionDigits: 4
                    onTextEdited: root.zeroCorrectionError = ""
                }

                Text {
                    text: "°"
                    font.pixelSize: 19
                    color: "#333333"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Text {
                width: parent.width
                visible: root.zeroCorrectionStage === "laser"
                         && root.zeroCorrectionError !== ""
                text: root.zeroCorrectionError
                color: "#c62828"
                font.pixelSize: 17
                lineHeight: 1.35
                lineHeightMode: Text.ProportionalHeight
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 18

                CusButton_Blue {
                    visible: root.zeroCorrectionStage !== "choose"
                    width: 140
                    height: 46
                    text: root.zeroCorrectionStage === "image" ? "确认修正" : "确认"
                    font.pixelSize: 18
                    onClicked: {
                        if (root.zeroCorrectionStage === "image")
                            root.confirmInnerAngleZeroCorrection()
                        else
                            root.confirmLaserZeroCorrection()
                    }
                }

                CusButton_White {
                    width: 140
                    height: 46
                    text: "取消"
                    font.pixelSize: 18
                    onClicked: root.closeZeroCorrectionPopup()
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
