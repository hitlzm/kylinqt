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

    // Text {
    //     id: titleText
    //     text: "激光导引头接收区"
    //     font.pixelSize: 24
    //     font.bold: true
    //     color: "#000000"

    //     anchors.left: parent.left
    //     anchors.leftMargin: 10
    //     anchors.top: parent.top
    //     anchors.topMargin: 10
    // }

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
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "状态信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredHeight: groupHeight1
                    label: Label {
                        text: parent.title
                        font.pixelSize: 18
                        leftPadding: 12
                        topPadding: 6
                    }

                    Row {
                        spacing: 30
                        anchors.centerIn: parent

                        Column {
                            spacing: 6

                            DataLabel {
                                label: "DYT状态:"
                                value: {
                                    switch(laserSerial.laserData.dytStatus) {
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
                                        default: return "未知(0x" + laserSerial.laserData.dytStatus.toString(16) + ")"
                                    }
                                }
                            }
                            DataLabel { label: "探测器状态:"; value: "0x" + laserSerial.laserData.detectorStatus.toString(16).toUpperCase() }
                            DataLabel {
                                label: "故障信息:"
                                value: laserSerial.laserData.faultInfo === 0 ? "无故障" : "故障码: 0x" + laserSerial.laserData.faultInfo.toString(16).toUpperCase()
                                valueColor: laserSerial.laserData.faultInfo === 0 ? "#1a73e8" : "#d93025"
                            }
                        }
                    }
                }

                // ── 角度信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "角度信息"
                    font.pixelSize: 18
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

                        Column {
                            spacing: 6
                            DataLabel { label: "光轴方位角:"; value: laserSerial.laserData.opticalAzimuth.toFixed(2) + "°" }
                            DataLabel { label: "光轴俯仰角:"; value: laserSerial.laserData.opticalPitch.toFixed(2) + "°" }
                            DataLabel { label: "方位偏差角:"; value: laserSerial.laserData.deviationAzimuth.toFixed(3) + "°" }
                            DataLabel { label: "俯仰偏差角:"; value: laserSerial.laserData.deviationPitch.toFixed(3) + "°" }
                        }
                        
                    }
                }

                // ── 角速度信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "角速度信息"
                    font.pixelSize: 18
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

                        Column {
                            spacing: 6
                            DataLabel { label: "方位陀螺:"; value: laserSerial.laserData.gyroAzimuthRate.toFixed(2) + "°/s" }
                            DataLabel { label: "俯仰陀螺:"; value: laserSerial.laserData.gyroPitchRate.toFixed(2) + "°/s" }
                            DataLabel { label: "方位视线:"; value: laserSerial.laserData.losAzimuthRate.toFixed(2) + "°/s" }
                            DataLabel { label: "俯仰视线:"; value: laserSerial.laserData.losPitchRate.toFixed(2) + "°/s" }
                        }

                    }
                }
            }

            RowLayout {
                spacing: 4

                // ── 象限能量 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
                    }
                    title: "象限能量"
                    font.pixelSize: 18
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

                        Column {
                            spacing: 6
                            DataLabel { label: "第一象限:"; value: laserSerial.laserData.quadrant1Energy.toFixed(2) }
                            DataLabel { label: "第二象限:"; value: laserSerial.laserData.quadrant2Energy.toFixed(2) }
                            DataLabel { label: "第三象限:"; value: laserSerial.laserData.quadrant3Energy.toFixed(2) }
                            DataLabel { label: "第四象限:"; value: laserSerial.laserData.quadrant4Energy.toFixed(2) }
                        }
                    }
                }

                // ── 其他信息 ──
                GroupBox {
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

                    Row {
                        spacing: 40
                        anchors.centerIn: parent

                        Column {
                            spacing: 6
                            DataLabel { label: "激光周期:"; value: (laserSerial.laserData.laserPeriod * 2).toFixed(0) + " us" }
                            DataLabel { label: "增益状态:"; value: "0x" + laserSerial.laserData.gainStatus.toString(16).toUpperCase() }
                            DataLabel { label: "软件版本1:"; value: laserSerial.laserData.softwareVersion1.toFixed(2) }
                            DataLabel { label: "软件版本2:"; value: laserSerial.laserData.softwareVersion2.toFixed(2) }
                        }
                    
                    }
                }
            }
        }
    }
    
    // 连接到 laserSerial 信号
    Connections {
        target: laserSerial
        function onErrorOccurred(msg) {
            console.log("Laser serial error:", msg)
        }
        function onConnected() {
            console.log("Laser serial connected")
        }
        function onDisconnected() {
            console.log("Laser serial disconnected")
        }
    }
}
