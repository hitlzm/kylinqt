import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    property int mywidth:1200
    property int myheight:320
    width: mywidth
    height: myheight
    color: '#e9f0f9'
    property int groupHeight1: 150

    function statusLabel(index) {
        if (index === 0) return "DYT状态:"
        if (index === 1) return "激光周期:"
        if (index === 2) return "自检信息:"
        if (index === 3) return "探测器状态:"
        if (index === 4) return "故障信息:"
        return "故障码:"
    }

    function statusValue(index) {
        if (index === 0) {
            switch (laserData.dytStatus) {
                case 0x00: return "待机"
                case 0x01: return "自检中"
                case 0x04: return "定轴搜索"
                case 0x05: return "矩形搜索"
                case 0x06: return "圆形搜索"
                case 0x07: return "捕获"
                case 0x08: return "稳定跟踪"
                case 0x09: return "记忆跟踪"
                case 0x0A: return "定轴搜索(位置)"
                case 0x0B: return "矩形搜索(位置)"
                case 0x0C: return "圆形搜索(位置)"
                default: return "未知(0x" + laserData.dytStatus.toString(16) + ")"
            }
        }
        // bits4：激光周期设定标志
        if (index === 1)
            return (laserData.detectorStatus & 0x10) ? "已设定" : "未设定"
        // bits3~2：自检信息
        if (index === 2) {
            switch ((laserData.detectorStatus & 0x0C) >> 2) {
                case 0x00: return "NA"
                case 0x01: return "自检中"
                case 0x02: return "自检正常"
                case 0x03: return "自检故障"
                default: return "NA"
            }
        }
        // bits1~0：探测器状态
        if (index === 3) {
            switch (laserData.detectorStatus & 0x03) {
                case 0x00: return "正常"
                case 0x01: return "丢光"
                case 0x02: return "不同步"
                case 0x03: return "能量饱和"
                default: return "未知"
            }
        }
        if (index === 4) {
            switch (laserData.faultInfo1) {
                case 0x00: return "NA"
                case 0x01: return "无故障"
                case 0x02: return "有故障"
                default: return "NA"
            }
        }
        switch (laserData.faultInfo1) {
            case 0x00: return "NA"
            case 0x01: return "NA"
            case 0x02: return "0x" + laserData.faultInfo2.toString(16).toUpperCase()
            default: return "NA"
        }
    }

    function statusValueColor(index) {
        // 探测器状态非正常（丢光/不同步/能量饱和）红色
        if (index === 3) return (laserData.detectorStatus & 0x03) !== 0 ? "#d93025" : "#1a73e8"
        // 自检故障红色
        if (index === 2) return (laserData.detectorStatus & 0x0C) === 0x0C ? "#d93025" : "#1a73e8"
        if (index === 4) return laserData.faultInfo1 === 1 ? "#1a73e8" : "#d93025"
        if (index === 5) return "#d93025"
        return "#1a73e8"
    }

    MsgPopup2 {
        id: faultmsg
    }
    MsgPopup2 {
        id: detectorMsg
    }

    // 捕捉故障信息变化，出现故障时弹窗
    property int faultStatus: laserData.faultInfo1
    onFaultStatusChanged: {
        if (faultStatus === 0x02) {
            faultmsg.showToast("激光导引头故障", 1500)
        }
    }

    // 探测器状态（第三部分 bits1~0）非正常时弹窗：1=丢光 2=不同步 3=能量饱和
    property int detectorState: (laserData.detectorStatus & 0x03)
    property int detectorLossCount: 0
    onDetectorStateChanged: {
        if (detectorState === 0) return
        var txt = ""
        if (detectorState === 1) {
            detectorLossCount++
            txt = "探测器丢光（累计 " + detectorLossCount + " 次）"
        }
        else if (detectorState === 2) txt = "探测器不同步"
        else if (detectorState === 3) txt = "探测器能量饱和"
        detectorMsg.showToast(txt, 1500)
    }
   

    // ═══ 数据显示区 ═══
    Rectangle {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 16
        // anchors.bottom: parent.bottom
        // anchors.bottomMargin: 10
        color: '#e1d8d8'

        ColumnLayout {
            anchors.fill: parent
            spacing: 1

            RowLayout {
                id: firstRow
                spacing: 4

                // ── 状态信息 ──
                GroupBox {
                    topPadding: 20
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "状态信息"
                    font.pixelSize: 18
                    Layout.fillWidth: false
                    Layout.preferredWidth: 600
                    Layout.minimumWidth: 600
                    Layout.preferredHeight: groupHeight1
                    label: Label {
                        text: parent.title
                        font.pixelSize: 18
                        leftPadding: 12
                        topPadding: 6
                    }

                    Row {
                        spacing: 28
                        anchors.centerIn: parent

                        Repeater {
                            model: 2

                            delegate: Column {
                                id: statusColumn
                                property int columnIndex: index
                                spacing: 6

                                Repeater {
                                    model: 3

                                    delegate: DataLabel {
                                        property int dataIndex: statusColumn.columnIndex * 3 + index
                                        labelWidth: 110
                                        valueWidth: 130
                                        label: root.statusLabel(dataIndex)
                                        value: root.statusValue(dataIndex)
                                        valueColor: root.statusValueColor(dataIndex)
                                    }
                                }
                            }
                        }
                    }
                }

                // ── 角度信息 ──
                GroupBox {
                    topPadding: 20
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "角度信息"
                    font.pixelSize: 18
                    Layout.preferredWidth: 240
                    Layout.minimumWidth: 240
                    Layout.fillWidth: true
                    Layout.preferredHeight: groupHeight1
                    label: Label {
                        text: parent.title
                        font.pixelSize: 18
                        leftPadding: 12
                        topPadding: 6
                    }

                    Row {
                        spacing: 40
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: 10

                        Column {
                            spacing: 6
                            DataLabel { label: "光轴方位角:"; value: laserData.opticalAzimuth.toFixed(2) + "°" }
                            DataLabel { label: "光轴俯仰角:"; value: laserData.opticalPitch.toFixed(2) + "°" }
                            DataLabel { label: "方位偏差角:"; value: laserData.deviationAzimuth.toFixed(3) + "°" }
                            DataLabel { label: "俯仰偏差角:"; value: laserData.deviationPitch.toFixed(3) + "°" }
                        }
                        
                    }
                }

            }

            RowLayout {
                spacing: 4

                // ── 角速度信息 ──
                GroupBox {
                    topPadding: 20
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "角速度信息"
                    font.pixelSize: 18
                    Layout.preferredWidth: 280
                    Layout.minimumWidth: 270
                    Layout.fillWidth: false
                    Layout.preferredHeight: groupHeight1
                    label: Label {
                        text: parent.title
                        font.pixelSize: 18
                        leftPadding: 12
                        topPadding: 6
                    }

                    Row {
                        spacing: 40
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: 10

                        Column {
                            spacing: 6
                            DataLabel { label: "方位陀螺:"; value: laserData.gyroAzimuthRate.toFixed(2) + "°/s" }
                            DataLabel { label: "俯仰陀螺:"; value: laserData.gyroPitchRate.toFixed(2) + "°/s" }
                            DataLabel { label: "方位视线:"; value: laserData.losAzimuthRate.toFixed(2) + "°/s" }
                            DataLabel { label: "俯仰视线:"; value: laserData.losPitchRate.toFixed(2) + "°/s" }
                        }

                    }
                }

                // ── 象限能量 ──
                GroupBox {
                    topPadding: 20
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "象限能量"
                    font.pixelSize: 18
                    Layout.preferredWidth: 230
                    Layout.minimumWidth: 210
                    Layout.fillWidth: false
                    Layout.preferredHeight: groupHeight1
                    label: Label {
                        text: parent.title
                        font.pixelSize: 18
                        leftPadding: 12
                        topPadding: 6
                    }

                    Row {
                        spacing: 40
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: 10

                        Column {
                            spacing: 6
                            DataLabel { label: "第一象限:"; value: laserData.quadrant1Energy.toFixed(2) }
                            DataLabel { label: "第二象限:"; value: laserData.quadrant2Energy.toFixed(2) }
                            DataLabel { label: "第三象限:"; value: laserData.quadrant3Energy.toFixed(2) }
                            DataLabel { label: "第四象限:"; value: laserData.quadrant4Energy.toFixed(2) }
                        }
                    }
                }

                // ── 其他信息 ──
                GroupBox {
                    topPadding: 20
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "其他信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredHeight: groupHeight1
                    label: Label {
                        text: parent.title
                        font.pixelSize: 18
                        leftPadding: 12
                        topPadding: 6
                    }

                    CusListView {
                        anchors.top: parent.top
                        anchors.topMargin: 14
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.right: parent.right
                        anchors.rightMargin: 8
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 0
                        spacing: 4
                        model: 6

                        delegate: DataLabel {
                            fontSize: 18
                            labelWidth: 110
                            valueWidth: 120
                            label: {
                                if (index === 0) return "丢光次数:"
                                if (index === 1) return "激光周期:"
                                if (index === 2) return "增益状态:"
                                if (index === 3) return "返回时间戳:"
                                if (index === 4) return "软件版本1:"
                                return "软件版本2:"
                            }
                            value: {
                                if (index === 0) return root.detectorLossCount + " 次"
                                if (index === 1) return (laserData.laserPeriod * 2).toFixed(0) + " us"
                                if (index === 2) return "0x" + laserData.gainStatus.toString(16).toUpperCase()
                                if (index === 3) return ("00" + laserData.usTime).slice(-3) + " us"
                                if (index === 4) return "V" + (laserData.softwareVersion1 / 100).toFixed(2)
                                return "V" + (laserData.softwareVersion2 / 100).toFixed(2)
                            }
                        }
                    }
                }
            }
        }
    }
    
    // 连接到 laserSerial 信号
    // Connections {
    //     target: laserSerial
    //     function onErrorOccurred(msg) {
    //         console.log("Laser serial error:", msg)
    //     }
    //     function onConnected() {
    //         console.log("Laser serial connected")
    //     }
    //     function onDisconnected() {
    //         console.log("Laser serial disconnected")
    //     }
    // }
}
