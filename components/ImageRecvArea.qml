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

    // ═══ 自检状态行（右侧）═══
    // 数据来源：imageSerial.imageData.selfCheckFlag1~5，0=正常, 非0=故障
    Flow {
        anchors.left: titleText.right
        anchors.leftMargin: 120
        anchors.verticalCenter: titleText.verticalCenter
        spacing: 20

        Indicator { label: "红外视频接收"; normal: imageData.selfCheckFlag2 === 1 }
        Indicator { label: "电视视频接收"; normal: imageData.selfCheckFlag3 === 1 }
        Indicator { label: "视频输出";     normal: imageData.selfCheckFlag4 === 1 }
        Indicator { label: "通讯";         normal: imageData.selfCheckFlag5 === 1 }
        Indicator { label: "伺服自检";     normal: imageData.selfCheckFlag6 === 1 }
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
        color: '#e1d8d8'
        // #eddada
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
                        border.width: 4
                        radius: 8
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
                           
                            DataLabel { label: "B帧流水号:"; value: imageData.bFrameSequence }
                            DataLabel { label: "A帧流水号回告:"; value: imageData.aFrameSequenceReply }
                            DataLabel {
                                label: "A帧有效标志:"
                                value: imageData.aFrameValidFlag === 0xAA ? "有效" : "无效"
                                valueColor: imageData.aFrameValidFlag === 0xAA ? "#1a73e8" : "#d93025"
                            }
                        }    
                       
                    }
                }

                // ── 控制/状态信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
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
                        spacing: 10
                        

                        Column {
                            spacing: 6
                            
                            DataLabel {
                                label: "导引头控制字:"
                                value: {
                                    var v = imageData.seekerCtrlReply
                                    var desc = "默认值"
                                    switch(v) {
                                        // case 0x00: return "默认值"
                                        case 0x01: desc= "自检通过"; break
                                        case 0x02: desc= "射检通过"; break
                                        case 0x04: desc= "搜索回告"; break
                                        case 0x06: desc= "发射指令回告"; break
                                        case 0x41: desc= "解锁回告"; break
                                        case 0x42: desc= "软件升级成功回告"; break
                                        case 0x43: desc= "软件升级失败回告"; break
                                        case 0x44: desc= "软件升级中"; break
                                        case 0x55: desc= "通讯检查通过"; break
                                        case 0xE1: desc= "自检中"; break
                                        case 0xE2: desc= "射检中"; break
                                        case 0xF1: desc= "自检不通过"; break
                                        case 0xF2: desc= "射检不通过"; break
                                        case 0xF4: desc= "通讯检查不通过"; break
                                        default: break
                                    }
                                    return desc;
                                }
                                valueColor: {
                                    var v = imageData.seekerCtrlReply
                                    if (v === 0x44 || v === 0xE1 || v === 0xE2) return "#e68a00"
                                    if (v >= 0xF0 || v === 0x43) return "#d93025"
                                    return "#1a73e8"
                                }
                            }
                            DataLabel {
                                label: "光学参数装订:"
                                value: {
                                    var v = imageData.opticalParamReply
                                    var data="默认值"
                                    switch(v) {
                            
                                        case 0xE1: data= "非卫星图模板装订成功"; break
                                        case 0xE2: data= "卫星图模板装订成功"; break
                                        case 0xE3: data= "模板装订中"; break
                                        case 0xE4: data= "红外非均匀校正成功"; break
                                        case 0xE5: data= "模板正在擦除"; break
                                        case 0xE6: data= "模板擦除成功"; break
                                        case 0xE7: data= "非卫星图模板装订失败"; break
                                        case 0xE8: data= "卫星图模板装订失败"; break
                                        case 0xE9: data= "盲元校正成功"; break
                                        default: break
                                    }
                                    return data;
                                }
                                valueColor: {
                                    var v = imageData.opticalParamReply
                                    if (v === 0xE3 || v === 0xE5) return "#e68a00"
                                    if (v === 0xE7 || v === 0xE8) return "#d93025"
                                    return "#1a73e8"
                                }
                            }
                            DataLabel {
                                label: "当前工作通道:"
                                value: imageData.currentWorkChannel === 0x02 ? "红外" : (imageData.currentWorkChannel === 0x03 ? "电视" : "未知")
                            }
                            DataLabel { label: "自检标志:"; value: "0x" + imageData.selfCheckFlag.toString(16).toUpperCase() }
                            
                        }
                       
                        Column {
                            spacing: 6
                            DataLabel { label: "目标类型:"; value: switch(imageData.m_targetBackgroundType1) {
                                        case 0x00: return "车辆"
                                        case 0x01: return "小型建筑物"
                                        case 0x02: return "坦克"
                                        case 0x04: return "舰船"
                                        case 0x07: return "靶标"
                                        default: return "未知"
                                    } }
                            DataLabel { label: "目标灰度类型:"; value: switch(imageData.m_targetBackgroundType2) {
                                        case 0x00: return "亮目标"
                                        case 0x01: return "暗目标"
                                        default: return "未知"
                                    } }
                            DataLabel { label: "目标动/静:"; value: switch(imageData.m_targetBackgroundType3) {
                                        case 0x00: return "静目标"
                                        case 0x01: return "动目标"
                                        default: return "未知"
                                    } }
                            DataLabel { label: "背景类型:"; value: switch(imageData.m_targetBackgroundType4) {
                                        case 0x00: return "平原"
                                        case 0x01: return "沙漠"
                                        case 0x02: return "岛岸"
                                        case 0x03: return "山地"
                                        case 0x04: return "丛林"
                                        case 0x05: return "公路"
                                        case 0x06: return "城市"
                                        case 0x07: return "湖泊"
                                        default: return "未知"
                                    }  }
                           
                        }
                        Column {
                            spacing: 6
                             DataLabel {
                                label: "光学工作状态:"
                                value: {
                                    switch(imageData.opticalWorkState) {
                                        case 0x02: return "搜索状态"
                                        case 0x03: return "跟踪状态"
                                        case 0x04: return "框架角电锁零位状态"
                                        case 0x05: return "记忆状态"
                                        case 0x06: return "解锁状态"
                                        default: return "未知状态"
                                    }
                                }
                            }
                            DataLabel { label: "修正指令状态:"; value: imageData.correctionCmdStatus === 1 ? "修正状态" : "非修正状态" }
                            DataLabel { label: "修正指令次数:"; value: imageData.correctionCmdCount }
                        }
                    }
                }

                // ── 跟踪信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
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
                            
                            DataLabel {
                                label: "跟踪状态:"
                                value: {
                                    switch(imageData.trackingState) {
                                        case 0x00: return "默认"
                                        case 0x11: return "搜索中"
                                        case 0x22: return "目标丢失"
                                        case 0x33: return "目标锁定"
                                        case 0x44: return "记忆状态"
                                        default: return "未知状态"
                                    }
                                }
                                valueColor: imageData.trackingState === 0x22 ? "#d93025" : "#1a73e8"
                            }
                            DataLabel {
                                label: "跟踪器状态:"
                                value: {
                                    switch(imageData.trackerState) {
                                        case 0x00: return "空闲状态"
                                        case 0x01: return "跟踪状态"
                                        case 0x02: return "识别状态"
                                        case 0x03: return "匹配状态"
                                        default: return "未知状态" 
                                    }
                                }
                            }
                            DataLabel { label: "方位偏差像素:"; value: imageData.azimuthDeviationPixel }
                            DataLabel { label: "俯仰偏差像素:"; value: imageData.pitchDeviationPixel }
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
                            
                            DataLabel { label: "俯仰框架角:"; value: imageData.pitchFrameAngle.toFixed(2) + "°" }
                            DataLabel { label: "偏航框架角:"; value: imageData.yawFrameAngle.toFixed(2) + "°" }
                            DataLabel { label: "方位主令:"; value: imageData.azimuthMasterCmd }
                            DataLabel { label: "俯仰主令:"; value: imageData.pitchMasterCmd }
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
                        border.width: 4
                        radius: 8
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
                            DataLabel { label: "俯仰视线角速度:"; value: imageData.pitchLosAngVel.toFixed(2) + "°/s" }
                            DataLabel { label: "偏航视线角速度:"; value: imageData.yawLosAngVel.toFixed(2) + "°/s" }
                            DataLabel { label: "俯仰陀螺:"; value: imageData.pitchGyro.toFixed(2) + "°/s" }
                        }
                        Column {
                            spacing: 6
                            DataLabel { label: "偏航陀螺:"; value: imageData.yawGyro.toFixed(2) + "°/s" }
                            DataLabel { label: "方位陀螺输出:"; value: imageData.azimuthGyroOutput.toFixed(2) + "°/s" }
                            DataLabel { label: "俯仰陀螺输出:"; value: imageData.pitchGyroOutput.toFixed(2) + "°/s" }
                        }
                       
                       
                    }
                }

                // ── 伺服/平台信息 ──
                GroupBox {
                    background: Rectangle {
                        color: "transparent"
                        border.color: "gray"
                        border.width: 4
                        radius: 8
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
                            DataLabel { label: "平台自检结果:"; value: imageData.platformSelfCheck }
                            DataLabel { label: "伺服运行时间:"; value: imageData.servoRunningTime + " s" }
                            DataLabel { label: "伺服阶跃:"; value: imageData.servoStep }
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
                            DataLabel { label: "红外帧编号:"; value: imageData.infraredFrameNum }
                            DataLabel { label: "红外帧频:"; value: imageData.infraredFrameRate + " Hz" }
                            DataLabel { label: "电视帧频:"; value: imageData.tvFrameRate + " Hz" }
                            DataLabel { label: "Cbh_tv4405:"; value: imageData.cbhTv4405 }
                        }
                        Column {
                            spacing: 6
                            
                            DataLabel { label: "波门尺寸:"; value: imageData.gateSize }
                            DataLabel { label: "软件版本1:"; value: imageData.softwareVersion1 }
                             DataLabel { label: "软件版本2:"; value: imageData.softwareVersion2 }
                            DataLabel { label: "软件版本3:"; value: imageData.softwareVersion3 }
                        }
                        
                    }
                }
            
            }
    }
    }
    // 连接到 imageSerial 信号
    // Connections {
    //     target: imageSerial
    //     function onErrorOccurred(msg) {
    //         console.log("Image serial error:", msg)
    //     }
    //     function onConnected() {
    //         console.log("Image serial connected")
    //     }
    //     function onDisconnected() {
    //         console.log("Image serial disconnected")
    //     }
    // }

}
