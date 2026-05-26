import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    width: 1200
    height: 400
    color: '#e9f0f9'
    property int groupHeight1: 150
    Text {
        id: titleText
        text: "图像导引头接收区"
        font.pixelSize: 24
        font.bold: true
        color: "#000000"

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    // ═══ 数据显示区 ═══
    Rectangle {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: titleText.bottom
        anchors.topMargin: 16
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        color: '#eddada'

        ColumnLayout {
         anchors.fill: parent
         spacing: 1

            RowLayout {
                id:firstRow
                spacing: 4

                // ── 帧信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 1
                        radius: 2
                    }
                    title: "帧信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    // Layout.fillHeight: true
                    Layout.preferredHeight: 150
                    label: Label {
                    text: parent.title
                    font.pixelSize: 18

                    leftPadding: 12     //调整标题位置
                    topPadding: 6
                    }

                    Row {
                        spacing: 30

                        Column {
                            spacing: 6
                           
                            DataLabel { label: "B帧流水号:"; value: imageSerial.imageData.bFrameSequence }
                            DataLabel { label: "A帧流水号回告:"; value: imageSerial.imageData.aFrameSequenceReply }
                            DataLabel {
                                label: "A帧有效标志:"
                                value: imageSerial.imageData.aFrameValidFlag === 0xAA ? "有效" : "无效"
                                valueColor: imageSerial.imageData.aFrameValidFlag === 0xAA ? "#1a73e8" : "#d93025"
                            }
                        }    
                       
                    }
                }

                // ── 控制/状态信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 1
                        radius: 2
                    }
                    title: "控制/状态信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredHeight: 150
                    label: Label {
                    text: parent.title
                    font.pixelSize: 18

                    leftPadding: 12     //调整标题位置
                    topPadding: 6
                    }

                    Row {
                        spacing: 40
                        

                        Column {
                            spacing: 6
                            
                            DataLabel { label: "导引头控制字回告:"; value: "0x" + imageSerial.imageData.seekerCtrlReply.toString(16).toUpperCase() }
                            DataLabel { label: "光学参数装订回告:"; value: "0x" + imageSerial.imageData.opticalParamReply.toString(16).toUpperCase() }
                            DataLabel {
                                label: "当前工作通道:"
                                value: imageSerial.imageData.currentWorkChannel === 0x02 ? "红外" : (imageSerial.imageData.currentWorkChannel === 0x03 ? "电视" : "未知")
                            }
                            DataLabel { label: "自检标志:"; value: "0x" + imageSerial.imageData.selfCheckFlag.toString(16).toUpperCase() }
                            
                        }
                       
                        Column {
                            spacing: 6
                            DataLabel { label: "目标/背景类型:"; value: imageSerial.imageData.targetBackgroundType }
                            DataLabel { label: "光学工作状态:"; value: imageSerial.imageData.opticalWorkState }
                            DataLabel { label: "修正指令状态:"; value: "" }
                            DataLabel { label: "修正指令次数:"; value: imageSerial.imageData.correctionCmdCount }
                        }
                    }
                }

                // ── 跟踪信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 1
                        radius: 2
                    }
                    title: "跟踪信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredHeight: 150
                    label: Label {
                    text: parent.title
                    font.pixelSize: 18

                    leftPadding: 12     //调整标题位置
                    topPadding: 6
                    }

                    Row {
                        spacing: 40

                        Column {
                            spacing: 6
                            
                            DataLabel { label: "跟踪状态:"; value: imageSerial.imageData.trackingState }
                            DataLabel { label: "跟踪器状态:"; value: imageSerial.imageData.trackerState }
                            DataLabel { label: "方位偏差像素:"; value: imageSerial.imageData.azimuthDeviationPixel }
                            DataLabel { label: "俯仰偏差像素:"; value: imageSerial.imageData.pitchDeviationPixel }
                        }
                        
                    }
                }

                // ── 角度信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 1
                        radius: 2
                    }
                    title: "角度信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredHeight: 150
                    label: Label {
                    text: parent.title
                    font.pixelSize: 18

                    leftPadding: 12     //调整标题位置
                    topPadding: 6
                    }

                    Row {
                        spacing: 40

                        Column {
                            spacing: 6
                            
                            DataLabel { label: "俯仰框架角:"; value: imageSerial.imageData.pitchFrameAngle.toFixed(2) + "°" }
                            DataLabel { label: "偏航框架角:"; value: imageSerial.imageData.yawFrameAngle.toFixed(2) + "°" }
                            DataLabel { label: "方位主令:"; value: imageSerial.imageData.azimuthMasterCmd }
                            DataLabel { label: "俯仰主令:"; value: imageSerial.imageData.pitchMasterCmd }
                        }
                        
                    }
                }

                
            }
            RowLayout {

                spacing: 4
                // ── 角速度信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 1
                        radius: 2
                    }
                    title: "角速度/陀螺信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredHeight:groupHeight1
                    label: Label {
                    text: parent.title
                    font.pixelSize: 18

                    leftPadding: 12     //调整标题位置
                    topPadding: 6
                    }

                    Row {
                        spacing: 40

                        Column {
                            spacing: 6
                            DataLabel { label: "俯仰视线角速度:"; value: imageSerial.imageData.pitchLosAngVel.toFixed(2) + "°/s" }
                            DataLabel { label: "偏航视线角速度:"; value: imageSerial.imageData.yawLosAngVel.toFixed(2) + "°/s" }
                            DataLabel { label: "俯仰陀螺:"; value: imageSerial.imageData.pitchGyro.toFixed(2) + "°/s" }
                        }
                        Column {
                            spacing: 6
                            DataLabel { label: "偏航陀螺:"; value: imageSerial.imageData.yawGyro.toFixed(2) + "°/s" }
                            DataLabel { label: "方位陀螺输出:"; value: imageSerial.imageData.azimuthGyroOutput.toFixed(2) + "°/s" }
                            DataLabel { label: "俯仰陀螺输出:"; value: imageSerial.imageData.pitchGyroOutput.toFixed(2) + "°/s" }
                        }
                       
                       
                    }
                }

                // ── 伺服/平台信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 1
                        radius: 2
                    }
                    title: "伺服/平台信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredHeight:groupHeight1
                    label: Label {
                    text: parent.title
                    font.pixelSize: 18

                    leftPadding: 12     //调整标题位置
                    topPadding: 6
                    }

                    Row {
                        spacing: 40

                        Column {
                            spacing: 6
                            DataLabel { label: "平台自检结果:"; value: imageSerial.imageData.platformSelfCheck }
                            DataLabel { label: "伺服运行时间:"; value: imageSerial.imageData.servoRunningTime + " s" }
                            DataLabel { label: "伺服阶跃:"; value: imageSerial.imageData.servoStep }
                        }
                       
                    }
                }

                // ── 其他信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 1
                        radius: 2
                    }
                    title: "其他信息"
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    Layout.preferredHeight:groupHeight1
                    label: Label {
                    text: parent.title
                    font.pixelSize: 18

                    leftPadding: 12     //调整标题位置
                    topPadding: 6
                    }

                    Row {
                        spacing: 40

                        Column {
                            spacing: 6
                            DataLabel { label: "红外帧编号:"; value: imageSerial.imageData.infraredFrameNum }
                            DataLabel { label: "红外帧频:"; value: imageSerial.imageData.infraredFrameRate + " Hz" }
                            DataLabel { label: "电视帧频:"; value: imageSerial.imageData.tvFrameRate + " Hz" }
                            DataLabel { label: "Cbh_tv4405:"; value: imageSerial.imageData.cbhTv4405 }
                        }
                        Column {
                            spacing: 6
                            
                            DataLabel { label: "波门尺寸:"; value: imageSerial.imageData.gateSize }
                            DataLabel { label: "软件版本1:"; value: imageSerial.imageData.softwareVersion1 }
                             DataLabel { label: "软件版本2:"; value: imageSerial.imageData.softwareVersion2 }
                            DataLabel { label: "软件版本3:"; value: imageSerial.imageData.softwareVersion3 }
                        }
                        
                    }
                }
            
            }
    }
    }
    // 连接到 imageSerial 信号
    Connections {
        target: imageSerial
        function onErrorOccurred(msg) {
            console.log("Image serial error:", msg)
        }
        function onConnected() {
            console.log("Image serial connected")
        }
        function onDisconnected() {
            console.log("Image serial disconnected")
        }
    }

}
