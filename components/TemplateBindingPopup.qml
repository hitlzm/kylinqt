import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.3
import "."
import taoQuick 1.0

Window {
    id: popup

    width: 1140
    height: 840
    visible: false
    flags: Qt.Window | Qt.WindowStaysOnTopHint
    modality: Qt.NonModal
    color: "#f5f6fa"

    // 居中
    Component.onCompleted: {
        x = Math.round((Screen.width - width) / 2)
        y = Math.round((Screen.height - height) / 2)
    }

    // 对外引用
    property var bindingData: null

    // 本地模式状态（驱动 UI 界面切换）
    property bool isSat: bindingData ? bindingData.isSatellite : false

    // 是否已点击过图片（控制红框显示）
    property bool hasClicked: false
    // 确认后的目标像素尺寸（红框实际使用的尺寸）
    property int confirmedPixelsX: 160
    property int confirmedPixelsY: 160

    // 图片选择对话框
    FileDialog {
        id: imageFileDialog
        title: "选择图片"
        nameFilters: ["图片文件 (*.png *.jpg *.jpeg *.bmp *.tiff)"]
        selectMultiple: true
        onAccepted: {
            for (var i = 0; i < fileUrls.length; i++) {
                var path = fileUrls[i].toString()
                // 去掉 file:/// 前缀，跨平台兼容
                if (Qt.platform.os === "windows") {
                    if (path.indexOf("file:///") === 0)
                        path = path.substring(8)
                } else {
                    // Linux/macOS: file:///home/xxx → /home/xxx
                    if (path.indexOf("file://") === 0)
                        path = path.substring(7)
                }
                if (bindingData) bindingData.addImage(path)
            }
        }
    }

    // ════════════════════ 主布局 ════════════════════
    Rectangle {
        anchors.fill: parent
        color: "#f5f6fa"
        border.color: "#c0c0c0"
        border.width: 1
        radius: 10
        clip: true

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 0
        spacing: 0

        // ────── 标题栏（支持拖拽）──────
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            color: "#1976D2"
            radius: 10

            // 拖拽 — 放在最前面作为底层，不遮挡按钮和文字
            MouseArea {
                anchors.fill: parent
                property real lastMouseX: 0
                property real lastMouseY: 0
                onPressed: {
                    lastMouseX = mouseX
                    lastMouseY = mouseY
                }
                onPositionChanged: {
                    popup.x += mouseX - lastMouseX
                    popup.y += mouseY - lastMouseY
                }
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 10
                color: "#1976D2"
            }

            Text {
                anchors.centerIn: parent
                text: "模板装订"
                font.pixelSize: 20
                font.bold: true
                color: "#ffffff"
            }

            CusButton_Blue {
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                width: 28
                height: 28
                text: "✕"
                onClicked: popup.close()
            }
        }

        // ────── 图片预览 + 参数配置区 ──────
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 12
            spacing: 16

            // ── 左侧：图片预览 ──
            Item {
                Layout.preferredWidth: 500
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    // 图片显示区域
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#e0e0e0"
                        border.color: "#b0b0b0"
                        border.width: 1
                        radius: 4
                        clip: true

                        Image {
                            id: imagePreview
                            anchors.fill: parent
                            anchors.margins: 4
                            fillMode: Image.PreserveAspectFit
                            source: {
                                    if (!bindingData || !bindingData.currentImagePath) return ""
                                    var p = bindingData.currentImagePath
                                    if (Qt.platform.os === "windows")
                                        return "file:///" + p
                                    else
                                        return "file://" + p   // Linux: /home/xxx 自带前导 /
                                }
                            visible: source != ""

                            // 图片加载完成 → 自动填入分辨率，重置默认目标尺寸
                            onStatusChanged: {
                                if (status === Image.Ready && bindingData) {
                                    bindingData.imageWidth  = sourceSize.width
                                    bindingData.imageHeight = sourceSize.height
                                    // 切换到新图片时默认 160×160
                                    bindingData.targetPixelsX = 160
                                    bindingData.targetPixelsY = 160
                                    popup.confirmedPixelsX = 160
                                    popup.confirmedPixelsY = 160
                                    popup.hasClicked = false
                                }
                            }
                        }

                        // 点击获取目标像素坐标
                        MouseArea {
                            anchors.fill: parent
                            enabled: imagePreview.source != "" && bindingData != null
                            cursorShape: Qt.CrossCursor

                            onClicked: {
                                if (!bindingData || imagePreview.sourceSize.width <= 0) return
                                var margin = 4  // 与 imagePreview 的 anchors.margins 保持一致
                                var scaleW = imagePreview.paintedWidth  / imagePreview.sourceSize.width
                                var scaleH = imagePreview.paintedHeight / imagePreview.sourceSize.height
                                // offset 是 Image 内部 letterbox/pillarbox 的偏移
                                var offsetX = (imagePreview.width  - imagePreview.paintedWidth)  / 2
                                var offsetY = (imagePreview.height - imagePreview.paintedHeight) / 2
                                // mouse.x/y 相对于父 Rectangle，需要减去 margin 才对齐 Image 坐标
                                var imgX = Math.round((mouse.x - margin - offsetX) / scaleW)
                                var imgY = Math.round((mouse.y - margin - offsetY) / scaleH)
                                // 使用已确认的目标尺寸进行边界裁剪
                                var halfW = Math.floor(popup.confirmedPixelsX / 2)
                                var halfH = Math.floor(popup.confirmedPixelsY / 2)
                                bindingData.targetPosX = Math.max(halfW, Math.min(imgX, imagePreview.sourceSize.width  - 1 - halfW))
                                bindingData.targetPosY = Math.max(halfH, Math.min(imgY, imagePreview.sourceSize.height - 1 - halfH))
                                popup.hasClicked = true
                            }
                        }

                        // 标框（红色方框，使用确认后的像素尺寸，首次点击后才显示）
                        Rectangle {
                            visible: bindingData && popup.hasClicked
                            color: "transparent"
                            border.color: "#ff0000"
                            border.width: 2

                            width: {
                                if (!bindingData || imagePreview.sourceSize.width <= 0) return 0
                                var s = Math.min(imagePreview.paintedWidth  / imagePreview.sourceSize.width,
                                                 imagePreview.paintedHeight / imagePreview.sourceSize.height)
                                return popup.confirmedPixelsX * s
                            }
                            height: {
                                if (!bindingData || imagePreview.sourceSize.width <= 0) return 0
                                var s = Math.min(imagePreview.paintedWidth  / imagePreview.sourceSize.width,
                                                 imagePreview.paintedHeight / imagePreview.sourceSize.height)
                                return popup.confirmedPixelsY * s
                            }
                            x: {
                                if (!bindingData || imagePreview.sourceSize.width <= 0) return 0
                                var margin = 4
                                var s = Math.min(imagePreview.paintedWidth  / imagePreview.sourceSize.width,
                                                 imagePreview.paintedHeight / imagePreview.sourceSize.height)
                                // Image 内部的 letterbox 偏移 + Image 相对于父 Rectangle 的 margin
                                var offX = margin + (imagePreview.width - imagePreview.sourceSize.width * s) / 2
                                return offX + (bindingData.targetPosX - popup.confirmedPixelsX / 2) * s
                            }
                            y: {
                                if (!bindingData || imagePreview.sourceSize.width <= 0) return 0
                                var margin = 4
                                var s = Math.min(imagePreview.paintedWidth  / imagePreview.sourceSize.width,
                                                 imagePreview.paintedHeight / imagePreview.sourceSize.height)
                                var offY = margin + (imagePreview.height - imagePreview.sourceSize.height * s) / 2
                                return offY + (bindingData.targetPosY - popup.confirmedPixelsY / 2) * s
                            }

                            // 中心十字线
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width
                                height: 2
                                color: "#ff0000"
                            }
                            Rectangle {
                                anchors.centerIn: parent
                                width: 2
                                height: parent.height
                                color: "#ff0000"
                            }

                            // 中心红点
                            Rectangle {
                                anchors.centerIn: parent
                                width: 8; height: 8
                                color: "#ff0000"
                                radius: 4
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: bindingData && bindingData.imageCount > 0
                                  ? "" : "请添加图片"
                            font.pixelSize: 16
                            color: "#888888"
                            visible: imagePreview.source == ""
                        }
                    }

                    // 导航栏
                    RowLayout {
                        Layout.preferredHeight: 36
                        Layout.fillWidth: true
                        spacing: 12

                        CusButton_Blue {
                            text: "< 上一张"
                            width: 90
                            height: 30
                            enabled: bindingData && bindingData.currentImageIndex > 0
                            onClicked: { if (bindingData) bindingData.prevImage() }
                        }

                        Text {
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter
                            text: bindingData
                                  ? "当前图片: " + (bindingData.currentImageIndex + 1)
                                    + " / " + bindingData.imageCount
                                  : "当前图片: 0 / 0"
                            font.pixelSize: 14
                            color: "#333333"
                            verticalAlignment: Text.AlignVCenter
                        }

                        CusButton_Blue {
                            text: "下一张 >"
                            width: 90
                            height: 30
                            enabled: bindingData
                                     && bindingData.currentImageIndex < bindingData.imageCount - 1
                            onClicked: { if (bindingData) bindingData.nextImage() }
                        }
                    }
                }
            }

            // ── 右侧：图片信息配置 ──
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#ffffff"
                border.color: "#d0d0d0"
                border.width: 1
                radius: 4
                clip: true

                Flickable {
                    anchors.fill: parent
                    anchors.margins: 10
                    contentWidth: columnContent.width
                    contentHeight: columnContent.height
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    interactive: columnContent.height > height

                    // 内容超出时显示滚动条
                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    Column {
                        id: columnContent
                        width: parent.width
                        spacing: 8

                        // ── 图像类型（始终可见）──
                        Text {
                            text: "图像类型:"
                            font.pixelSize: 15
                            font.bold: true
                            color: "#333333"
                        }
                        ButtonGroup { id: imageTypeGroup }

                        Row {
                            spacing: 30
                            CusRadioButton {
                                id: satelliteRadio
                                text: "卫星图"
                                font.pixelSize: 14
                                ButtonGroup.group: imageTypeGroup
                                checked: isSat
                                onClicked: {
                                    popup.isSat = true
                                    if (bindingData) bindingData.isSatellite = true
                                }
                            }
                            CusRadioButton {
                                id: nonsatelliteRadio
                                text: "非卫星图"
                                font.pixelSize: 14
                                ButtonGroup.group: imageTypeGroup
                                checked: !isSat
                                onClicked: {
                                    popup.isSat = false
                                    if (bindingData) bindingData.isSatellite = false
                                }
                            }
                        }

                        // C++ 侧属性变化时同步回本地 isSat
                        Connections {
                            target: bindingData
                            onIsSatelliteChanged: {
                                if (bindingData) popup.isSat = bindingData.isSatellite
                            }
                        }

                        // ════════════════════════════════════════
                        // 卫星图参数区块
                        // ════════════════════════════════════════
                        Rectangle {
                            width: parent.width; height: 1; color: "#1976D2"
                            visible: isSat
                        }
                        Text {
                            text: "卫星图参数:"
                            font.pixelSize: 15
                            font.bold: true
                            color: "#1976D2"
                            visible: isSat
                        }

                        Row {
                            visible: isSat
                            spacing: 30
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "比例尺"
                                labeltext: "米/像素"
                                text: bindingData ? bindingData.scale.toFixed(4) : ""
                                onEditingFinished: { if (bindingData) bindingData.scale = parseFloat(text) || 0 }
                            }
                            Item { width: 180; height: 60 }
                            Item { width: 180; height: 60 }
                        }
                        Row {
                            visible: isSat
                            spacing: 30
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "图像宽度"
                                labeltext: "像素"
                                text: bindingData ? bindingData.imageWidth.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.imageWidth = parseInt(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "图像高度"
                                labeltext: "像素"
                                text: bindingData ? bindingData.imageHeight.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.imageHeight = parseInt(text) || 0 }
                            }
                            Item { width: 180; height: 55 }
                        }
                        Row {
                            visible: isSat
                            spacing: 30
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "目标横坐标"
                                labeltext: "像素"
                                text: bindingData ? bindingData.targetPosX.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.targetPosX = parseInt(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "目标纵坐标"
                                labeltext: "像素"
                                text: bindingData ? bindingData.targetPosY.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.targetPosY = parseInt(text) || 0 }
                            }
                            Item { width: 180; height: 55 }
                        }
                        Row {
                            visible: isSat
                            spacing: 30
                            height: 60
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "目标横像素数"
                                labeltext: "像素"
                                text: bindingData ? bindingData.targetPixelsX.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.targetPixelsX = parseInt(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "目标纵像素数"
                                labeltext: "像素"
                                text: bindingData ? bindingData.targetPixelsY.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.targetPixelsY = parseInt(text) || 0 }
                            }
                            CusButton_Blue {
                                text: "确认修改"
                                width: 80
                                height: 32
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: {
                                    if (bindingData) {
                                        popup.confirmedPixelsX = bindingData.targetPixelsX
                                        popup.confirmedPixelsY = bindingData.targetPixelsY
                                    }
                                }
                            }
                        }

                        // ════════════════════════════════════════
                        // 非卫星图参数区块
                        // ════════════════════════════════════════
                        Rectangle {
                            width: parent.width; height: 1; color: "#1976D2"
                            visible: !isSat
                        }
                        Text {
                            text: "非卫星图参数:"
                            font.pixelSize: 15
                            font.bold: true
                            color: "#1976D2"
                            visible: !isSat
                        }

                        Row {
                            visible: !isSat
                            spacing: 30
                            MyComboBox {
                                mywidth: 155; myheight: 55
                                title: "模板图模式"
                                model: ["电视", "红外"]
                                mycurrentIndex: bindingData ? bindingData.templateMode : 0
                                onCurrentIndexChanged: { if (bindingData) bindingData.templateMode = mycurrentIndex }
                            }
                            Item { width: 180; height: 55 }
                            Item { width: 180; height: 55 }
                        }
                        Row {
                            visible: !isSat
                            spacing: 30
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "拍摄距离"
                                labeltext: "米"
                                text: bindingData ? bindingData.distance.toFixed(2) : ""
                                onEditingFinished: { if (bindingData) bindingData.distance = parseFloat(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "拍摄方位角"
                                labeltext: "°"
                                text: bindingData ? bindingData.azimuth.toFixed(2) : ""
                                onEditingFinished: { if (bindingData) bindingData.azimuth = parseFloat(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "拍摄俯仰角"
                                labeltext: "°"
                                text: bindingData ? bindingData.pitchAngle.toFixed(2) : ""
                                onEditingFinished: { if (bindingData) bindingData.pitchAngle = parseFloat(text) || 0 }
                            }
                        }
                        Row {
                            visible: !isSat
                            spacing: 30
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "相机焦距"
                                labeltext: "mm"
                                text: bindingData ? bindingData.focalLength.toFixed(2) : ""
                                onEditingFinished: { if (bindingData) bindingData.focalLength = parseFloat(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "像元尺寸"
                                labeltext: "μm"
                                text: bindingData ? bindingData.pixelSize.toFixed(2) : ""
                                onEditingFinished: { if (bindingData) bindingData.pixelSize = parseFloat(text) || 0 }
                            }
                            Item { width: 180; height: 55 }
                        }
                        Row {
                            visible: !isSat
                            spacing: 30
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "图像宽度"
                                labeltext: "像素"
                                text: bindingData ? bindingData.imageWidth.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.imageWidth = parseInt(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "图像高度"
                                labeltext: "像素"
                                text: bindingData ? bindingData.imageHeight.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.imageHeight = parseInt(text) || 0 }
                            }
                            Item { width: 180; height: 55 }
                        }
                        Row {
                            visible: !isSat
                            spacing: 30
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "目标横坐标"
                                labeltext: "像素"
                                text: bindingData ? bindingData.targetPosX.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.targetPosX = parseInt(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "目标纵坐标"
                                labeltext: "像素"
                                text: bindingData ? bindingData.targetPosY.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.targetPosY = parseInt(text) || 0 }
                            }
                            Item { width: 180; height: 55 }
                        }
                        Row {
                            visible: !isSat
                            spacing: 30
                            height: 60
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "目标横像素数"
                                labeltext: "像素"
                                text: bindingData ? bindingData.targetPixelsX.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.targetPixelsX = parseInt(text) || 0 }
                            }
                            MyTextField {
                                mywidth: 155; myheight: 60
                                title: "目标纵像素数"
                                labeltext: "像素"
                                text: bindingData ? bindingData.targetPixelsY.toString() : ""
                                onEditingFinished: { if (bindingData) bindingData.targetPixelsY = parseInt(text) || 0 }
                            }
                            CusButton_Blue {
                                text: "确认修改"
                                width: 80
                                height: 32
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: {
                                    if (bindingData) {
                                        popup.confirmedPixelsX = bindingData.targetPixelsX
                                        popup.confirmedPixelsY = bindingData.targetPixelsY
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // ────── 分隔线 ──────
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            color: "#d0d0d0"
        }

        // ────── TXT 生成信息 ──────
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 70
            Layout.margins: 12
            color: "#ffffff"
            border.color: "#d0d0d0"
            border.width: 1
            radius: 4

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 20

                Column {
                    spacing: 4
                    Text {
                        text: bindingData
                              ? "文件名: " + (bindingData.txtFileName || "未生成")
                              : "文件名: 未生成"
                        font.pixelSize: 14
                        color: "#333333"
                    }
                    Text {
                        text: bindingData && bindingData.txtGenerated
                              ? "状态: 已生成" : "状态: 未生成"
                        font.pixelSize: 14
                        color: bindingData && bindingData.txtGenerated
                               ? "#2e7d32" : "#d93025"
                    }
                }

                Item { Layout.fillWidth: true }

                CusButton_Blue {
                    text: "生成TXT"
                    width: 100
                    height: 32
                    enabled: bindingData && bindingData.imageCount > 0
                    onClicked: {
                        if (bindingData) bindingData.generateTxt()
                    }
                }
            }
        }

        // ────── 分隔线 ──────
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            color: "#d0d0d0"
        }

        // ────── 发送状态 ──────
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 65
            Layout.margins: 12
            color: "#ffffff"
            border.color: "#d0d0d0"
            border.width: 1
            radius: 4

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 5

                // 进度条
                RowLayout {
                    spacing: 8
                    Text {
                        text: "进度:"
                        font.pixelSize: 14
                        color: "#333333"
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 18
                        color: "#e0e0e0"
                        radius: 4

                        Rectangle {
                            width: parent.width * (bindingData ? bindingData.sendProgress : 0)
                            height: parent.height
                            color: "#1976D2"
                            radius: 4
                        }

                        Text {
                            anchors.centerIn: parent
                            text: bindingData
                                  ? Math.round(bindingData.sendProgress * 100) + "%"
                                  : "0%"
                            font.pixelSize: 12
                            color: "#333333"
                        }
                    }
                }

                // 发送状态指示
                RowLayout {
                    spacing: 25
                    Indicator {
                        id: imgSentIndicator
                        label: "图片发送成功"
                        fontSize: 13
                        height: 20
                        normal: bindingData && bindingData.imageSent
                    }
                    Indicator {
                        id: txtSentIndicator
                        label: "txt发送成功"
                        fontSize: 13
                        height: 20
                        normal: bindingData && bindingData.txtSent
                    }
                    Text {
                        text: bindingData ? bindingData.statusMessage : ""
                        font.pixelSize: 12
                        color: "#666666"
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                }
            }
        }

        // ────── 底部按钮栏 ──────
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            Layout.margins: 12

            Row {
                anchors.centerIn: parent
                spacing: 15

                CusButton_Blue {
                    text: "添加图片"
                    width: 100
                    height: 36
                    onClicked: imageFileDialog.open()
                }

                CusButton_Blue {
                    text: "删除图片"
                    width: 100
                    height: 36
                    enabled: bindingData && bindingData.imageCount > 0
                    onClicked: {
                        if (bindingData && bindingData.currentImageIndex >= 0)
                            bindingData.removeImage(bindingData.currentImageIndex)
                    }
                }

                CusButton_Blue {
                    text: "生成TXT"
                    width: 100
                    height: 36
                    enabled: bindingData && bindingData.imageCount > 0
                    onClicked: {
                        if (bindingData) bindingData.generateTxt()
                    }
                }

                CusButton_Blue {
                    text: "开始装订"
                    width: 110
                    height: 36
                    enabled: bindingData && bindingData.connected
                             && bindingData.imageCount > 0
                    onClicked: {
                        if (bindingData) {
                            bindingData.generateTxt()
                            bindingData.requestSendImages()
                            bindingData.requestSendTxt("")
                        }
                    }
                }

                CusButton_Blue {
                    text: "关闭"
                    width: 80
                    height: 36
                    onClicked: popup.close()
                }
            }
        }
    }

    // ════════════════════ Toast ════════════════════
    MsgPopup2 {
        id: toastMsg
    }

    }  // Rectangle 闭合
}
