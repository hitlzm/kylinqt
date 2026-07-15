import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"
Rectangle {
    id: root
    width: 1200
    height: 750
    color: '#e9f0f9'

    MessagePopup {
        id: azimuthmsg
    }
    MessagePopup {
        id: pitchmsg
    }
    MsgPopup2{
        id:testmsg
    }
    //下边沿
    Rectangle {
        width: parent.width
        height: 1
        color: "black"
        anchors.bottom: parent.bottom   // 贴在主矩形下边沿
    }
    property var commandList: [

        "默认值",
        //以下指令发送三拍，三拍后恢复默认值
        "自检",
        "射检",
        "搜索",
        "预置",
        "发射指令",
        "通讯检查",
        "解锁",
        "软件升级",
        "软件升级结束",
        "打开波门叠加",
        "关闭波门叠加"

    ]

    property int currentCmd: 0
    property int opticalParamCtrlCmd: 0

    // 输入校验标志（默认 true，未输入不算无效）
    property bool pitchInputValid: true
    property bool yawInputValid: true

    // 接收回告描述缓存（无效值时保持上一次的描述）
    property string lastValidDesc1: "默认值"
    property string lastValidDesc2: "默认值"
    Text {
        id: titleText
        text: "图像导引头发送区"
        font.pixelSize: 24
        font.bold: true
        color: "#000000"

        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    // 左侧命令区
    Rectangle {
        id: commandArea
        width: 160
        height: 460
        color: '#faf7f7'
        radius: 4

        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: titleText.bottom
        anchors.topMargin: 20

        Column {
            spacing: 6
            anchors.fill: parent
            anchors.margins: 12

            Text {
                text: "图像导引头控制字"
                font.pixelSize: 16
                font.bold: true
                color: "#000000"
            }
            Repeater {
                model: root.commandList

                delegate: CusRadioButton {
                    text: modelData
                    checked: index === root.currentCmd
                    font.pixelSize: 16

                    indicator.width: 18
                    indicator.height: 18

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 16
                        color: "#000000"
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 30
                    }

                    onClicked: {
                    root.currentCmd = index
                    if(currentCmd===0)
                    {
                            imageSendData.m_seekerCtrlWord=0x00
                    } else if (currentCmd===1)
                    {
                            imageSendData.m_seekerCtrlWord=0x01
                    }
                    else if (currentCmd===2)
                    {
                            imageSendData.m_seekerCtrlWord=0x02
                    }else if (currentCmd===3)
                    {
                            imageSendData.m_seekerCtrlWord=0x03
                    }else if (currentCmd===4)
                    {
                            imageSendData.m_seekerCtrlWord=0x04
                    }else if (currentCmd===5)
                    {
                           imageSendData.m_seekerCtrlWord=0x06
                    }else if (currentCmd===6)
                    {
                            imageSendData.m_seekerCtrlWord=0x55
                    }else if (currentCmd===7)
                    {
                            imageSendData.m_seekerCtrlWord=0x40
                    }else if (currentCmd===8)
                    {
                            imageSendData.m_seekerCtrlWord=0xA1
                    }else if (currentCmd===9)
                    {
                            imageSendData.m_seekerCtrlWord=0xB1
                    }else if (currentCmd===10)
                    {
                            imageSendData.m_seekerCtrlWord=0xED
                    }
                    else if (currentCmd===11)
                    {
                            imageSendData.m_seekerCtrlWord=0xEE
                    }

                    }
                }
            }
        }
    }

    // 串口选择 - 绑定到 C++ imageSerial 对象
    CusComboBox {
        id: serialComboBox
        width: 120
        height: 32

        anchors.top: titleText.top
        anchors.left: titleText.right
        anchors.leftMargin: 30

        model: imageData.availablePorts
    }

    // 波特率
    CusComboBox {
        id: baudComboBox
        width: 120
        height: 32

        anchors.top: serialComboBox.top
        anchors.left: serialComboBox.right
        anchors.leftMargin: 30

        model: ["460800"]
    }
    CusPopup {
            id: tip
            backgroundWidth: 200
            backgroundHeight: 80
            barColor:"lightgray"
            contentItem:Column {
            spacing: 10
            Text { text: "串口已打开！" }
            CusButton_Blue { text: "确定"; onClicked: tip.hide() }
            }
        }
    MessagePopup {
        id: msg
    }
    CusPopup {
        id: tip1
        backgroundWidth: 200
        backgroundHeight: 80
        barColor:"lightgray"
        contentItem:Column {
        spacing: 10
        Text { text: "串口打开失败！" }
        CusButton_Blue { text: "确定"; onClicked: tip1.hide() }
        }
    }
    //扫描串口按钮
    CusButton_Blue {
        id: scanButton
        width: 100
        height: 32
        text: "扫描串口"

        anchors.top: serialComboBox.top
        anchors.left: baudComboBox.right
        anchors.leftMargin: 20

        onClicked: {
            imageData.requestScanPorts()
        }
    }
    // 打开串口按钮
    CusButton_Blue {
        id: openButton
        width: 100
        height: 32
        text: imageData.portOpen ? "关闭串口" : "打开串口"

        anchors.top: serialComboBox.top
        anchors.left: scanButton.right
        anchors.leftMargin: 20
        Timer {
            id: delayTimer
            interval: 1
            onTriggered: {
                // if (imageData.portOpen) tip.show()
                if (imageData.portOpen) {
                    // msg.message = "串口已打开！"; msg.open()
                    testmsg.showToast("串口已打开")
                    }
                else { msg.message = "串口打开失败！"; msg.open() }
            }
        }
        onClicked: {
            if (imageData.portOpen) {
                imageData.closePort()
            } else {
                imageData.openPort(serialComboBox.currentText,
                                    parseInt(baudComboBox.currentText))
                delayTimer.start()
            }
        }
    }

    // 发送数据按钮
    CusButton_Blue {
        id: sendButton
        width: 100
        height: 32
        text: "发送数据"
        enabled: imageData.portOpen && pitchInputValid && yawInputValid

        anchors.top: serialComboBox.top
        anchors.left: openButton.right
        anchors.leftMargin: 30

        onClicked: {
            imageSendData.buildFrame()     //构建帧成功后释放信号，由串口工作线程计算校验位并发送
            console.log("图像导引头数据已发送")
        }
    }

    CusButton_Blue {
        id: photobutton
        width: 100
        height: 32
        text: "拍摄参考图"
        anchors.top: serialComboBox.top
        anchors.left: sendButton.right
        anchors.leftMargin: 20
        onClicked: {
                imageSendData.m_captureRefImgCmd=0xaa;
                imageSendData.buildFrame()
        }
    }

    MySwitch {
        id: mySwitch
        width: 120
        height: 32
        title: "字符叠加"
        anchors.top: photobutton.top
        anchors.left: photobutton.right
        anchors.leftMargin: 20
    }

    // 框架角预装输入行
    Row {
        id: presetInputRow
        spacing: 30

        anchors.top: serialComboBox.bottom
        anchors.topMargin: 10
        anchors.left: serialComboBox.left

        MyTextField {
            id: pitchPresetField
            mywidth: 120
            myheight: 60
            title: "俯仰框架角预装"
            labeltext: "°"

            // 实时校验：每次按键即刻更新发送按钮状态，超出范围即刻弹窗
            onTextChanged: {
                var val = Number(text)
                if (!isNaN(val) && (val > 18 || val < -18)) {
                    if (root.pitchInputValid)  // 仅在由合法变非法时弹一次
                        testmsg.showToast("俯仰角输入范围为-18°~ 18°,请重新输入")
                    root.pitchInputValid = false
                } else {
                    root.pitchInputValid = true
                }
            }

            onEditingFinished: {
                if (Number(text) >= -18 && Number(text) <= 18)
                    imageSendData.m_pitchGimbalPreset = Number(text)
            }
        }

        MyTextField {
            id: yawPresetField
            mywidth: 120
            myheight: 60
            title: "偏航框架角预装"
            labeltext: "°"

            onTextChanged: {
                var val = Number(text)
                if (!isNaN(val) && (val > 18 || val < -18)) {
                    if (root.yawInputValid)
                        testmsg.showToast("偏航角输入范围为-18°~ 18°,请重新输入")
                    root.yawInputValid = false
                } else {
                    root.yawInputValid = true
                }
            }

            onEditingFinished: {
                if (Number(text) >= -18 && Number(text) <= 18)
                    imageSendData.m_yawGimbalPreset = Number(text)
            }
        }
    }

    // ═══ 自检状态指示灯（输入框右侧）═══
    // 数据来源：imageData.selfCheckFlag2~6，1=正常, 其他=故障
    Row {
        id: indicatorRow
        spacing: 20

        anchors.left: presetInputRow.right
        anchors.leftMargin: 30
        anchors.verticalCenter: presetInputRow.verticalCenter

        Indicator { id: indIrVideo;   label: "红外视频接收"; normal: imageData.selfCheckFlag2 === 1; onFaultTriggered: testmsg.showToast(label + "故障") }
        Indicator { id: indTvVideo;   label: "电视视频接收"; normal: imageData.selfCheckFlag3 === 1; onFaultTriggered: testmsg.showToast(label + "故障") }
        Indicator { id: indVideoOut;  label: "视频输出";     normal: imageData.selfCheckFlag4 === 1; onFaultTriggered: testmsg.showToast(label + "故障") }
        Indicator { id: indComm;      label: "通讯";         normal: imageData.selfCheckFlag5 === 1; onFaultTriggered: testmsg.showToast(label + "故障") }
        Indicator { id: indServo;     label: "伺服自检";     normal: imageData.selfCheckFlag6 === 1; onFaultTriggered: testmsg.showToast(label + "故障") }
    }

    // 下拉选择框第一行
    Row {
        id: comboRow1
        spacing: 50

        anchors.top: presetInputRow.bottom
        anchors.topMargin: 10
        anchors.left: presetInputRow.left

        MyComboBox {
            id: myCombox1
            mywidth: 180
            myheight: 55
            title: "光学参数装订控制字"
            model: ["默认值", "非卫星图模板制作", "卫星图模板制作", "盲元校正","红外非均匀校正","模板擦除","积分时间设置"]
            onCurrentIndexChanged:{

                root.opticalParamCtrlCmd=mycurrentIndex

                if(opticalParamCtrlCmd===0){
                    imageSendData.m_opticalParamCtrl=0x00
                }else if(opticalParamCtrlCmd===1){
                    imageSendData.m_opticalParamCtrl=0xE1
                }else if(opticalParamCtrlCmd===2){
                    imageSendData.m_opticalParamCtrl=0xE2
                }else if(opticalParamCtrlCmd===3){
                    imageSendData.m_opticalParamCtrl=0xE3
                }else if(opticalParamCtrlCmd===4){
                    imageSendData.m_opticalParamCtrl=0xE4
                }else if(opticalParamCtrlCmd===5){
                    imageSendData.m_opticalParamCtrl=0xE5
                }else if(opticalParamCtrlCmd===6){
                    imageSendData.m_opticalParamCtrl=0xE6
                }
            }
        }
        MyComboBox {
            id: myCombox2
            mywidth: 90
            myheight: 55
            title: "模板序号"
            model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10"]
            onCurrentIndexChanged: {
               if (mycurrentIndex !== -1) {
               imageSendData.m_templateIndex = mycurrentIndex + 1;
             }
            }
        }
        MyComboBox {
            id: myCombox3
            mywidth: 140
            myheight: 55
            title: "预装目标类型"
            model: ["车辆", "小型建筑物", "坦克", "舰船", "靶标"]
            onCurrentIndexChanged: {
               if (mycurrentIndex === 0) {
               imageSendData.m_targetBackgroundType1 = 0x00;
             }else if(mycurrentIndex === 1){
                imageSendData.m_targetBackgroundType1 = 0x01;
             }else if(mycurrentIndex === 2){
                imageSendData.m_targetBackgroundType1 = 0x02;
             }else if(mycurrentIndex === 3){
                imageSendData.m_targetBackgroundType1 = 0x04;
             }else if(mycurrentIndex === 4){
                imageSendData.m_targetBackgroundType1 = 0x07;
             }
            }
        }
        MyComboBox {
            id: myCombox4
            mywidth: 100
            myheight: 55
            title: "背景类型"
            model: ["平原","沙漠", "岛岸","山地","丛林","公路","城市","湖泊"]
            onCurrentIndexChanged: {
               if (mycurrentIndex === 0) {
               imageSendData.m_targetBackgroundType4 = 0x00;
             }else if(mycurrentIndex === 1){
                imageSendData.m_targetBackgroundType4 = 0x01;
             }else if(mycurrentIndex === 2){
                imageSendData.m_targetBackgroundType4 = 0x02;
             }else if(mycurrentIndex === 3){
                imageSendData.m_targetBackgroundType4 = 0x03;
             }else if(mycurrentIndex === 4){
                imageSendData.m_targetBackgroundType4 = 0x04;
             }else if(mycurrentIndex === 5){
                imageSendData.m_targetBackgroundType4 = 0x05;
             }else if(mycurrentIndex === 6){
                imageSendData.m_targetBackgroundType4 = 0x06;
             }else if(mycurrentIndex === 7){
                imageSendData.m_targetBackgroundType4 = 0x07;
             }
            }
        }
    }

    // 下拉选择框第二行
    Row {
        id: comboRow2
        spacing: 50

        anchors.top: comboRow1.bottom
        anchors.topMargin: 10
        anchors.left: comboRow1.left

        MyComboBox {
            id: myCombox5
            mywidth: 120
            myheight: 55
            title: "目标灰度类型"
            model: ["亮目标", "暗目标"]
            onCurrentIndexChanged: {
               if (mycurrentIndex !== -1) {
               imageSendData.m_targetBackgroundType2 = mycurrentIndex;
               }
            }
        }
        MyComboBox {
            id: myCombox6
            mywidth: 120
            myheight: 55
            title: "目标动静类型"
            model: ["静目标","动目标" ]
            onCurrentIndexChanged: {
               if (mycurrentIndex !== -1) {
               imageSendData.m_targetBackgroundType3 = mycurrentIndex;
               }

            }
        }
        MyComboBox {
            id: myCombox7
            mywidth: 140
            myheight: 55
            title: "修正指令状态"
            model: ["非修正状态","修正状态" ]
             onCurrentIndexChanged: {
               if (mycurrentIndex !== -1) {
               imageSendData.m_trackingCorrectionCmd = mycurrentIndex;
               }
            }
        }
        MyComboBox {
            id: myCombox8
            mywidth: 120
            myheight: 55
            title: "波门大小"
            model: ["大","小" ]
            onCurrentIndexChanged: {
               if (mycurrentIndex === 0) {
               imageSendData.m_gateSize = 0xaa;
             }else if(mycurrentIndex === 1){
                imageSendData.m_gateSize = 0x55;
             }
            }
        }
    }

    // ═══ 帧信息 / 控制状态信息（来自接收区）═══
    Row {
        id: recvInfoRow
        spacing: 8

        anchors.top: comboRow2.bottom
        anchors.topMargin: 10
        anchors.left: commandArea.right
        anchors.leftMargin: 10

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
            label: Label {
                text: parent.title
                font.pixelSize: 18

                leftPadding: 12     //调整标题位置
                topPadding: 6
            }

            Column {
                spacing: 6

                DataLabel { fontSize: 16; labelWidth: 120; valueWidth: 60; label: "B帧流水号:"; value: imageData.bFrameSequence }
                DataLabel { fontSize: 16; labelWidth: 120; valueWidth: 60; label: "A帧流水号回告:"; value: imageData.aFrameSequenceReply }
                DataLabel {
                    fontSize: 16
                    labelWidth: 120
                    valueWidth: 60
                    label: "A帧有效标志:"
                    value: imageData.aFrameValidFlag === 0xAA ? "有效" : "无效"
                    valueColor: imageData.aFrameValidFlag === 0xAA ? "#1a73e8" : "#d93025"
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
            label: Label {
                text: parent.title
                font.pixelSize: 18

                leftPadding: 12     //调整标题位置
                topPadding: 6
            }

            Row {
                spacing: 8

                Column {
                    spacing: 6

                    DataLabel {
                        fontSize: 16
                        labelWidth: 105
                        valueWidth: 165
                        label: "导引头控制字:"

                        value: {
                            var v = imageData.seekerCtrlReply;
                            var currentDesc = null;  // null 代表“此次不更新缓存”
                            switch(v) {
                                case 0x01: currentDesc = "自检通过"; break;
                                case 0x02: currentDesc = "射检通过"; break;
                                case 0x04: currentDesc = "搜索回告"; break;
                                case 0x06: currentDesc = "发射指令回告"; break;
                                case 0x41: currentDesc = "解锁回告"; break;
                                case 0x42: currentDesc = "软件升级成功回告"; break;
                                case 0x43: currentDesc = "软件升级失败回告"; break;
                                case 0x44: currentDesc = "软件升级中"; break;
                                case 0x55: currentDesc = "通讯检查通过"; break;
                                case 0xE1: currentDesc = "自检中"; break;
                                case 0xE2: currentDesc = "射检中"; break;
                                case 0xF1: currentDesc = "自检不通过"; break;
                                case 0xF2: currentDesc = "射检不通过"; break;
                                case 0xF4: currentDesc = "通讯检查不通过"; break;
                                default: break;
                            }
                            if (currentDesc !== null) {
                                root.lastValidDesc1 = currentDesc;
                            }
                            return root.lastValidDesc1;
                        }
                        valueColor: {
                            var v = imageData.seekerCtrlReply
                            if (v === 0x44 || v === 0xE1 || v === 0xE2) return "#e68a00"
                            if (v >= 0xF0 || v === 0x43) return "#d93025"
                            return "#1a73e8"
                        }
                    }
                    DataLabel {
                        fontSize: 16
                        labelWidth: 105
                        valueWidth: 165
                        label: "光学参数装订:"

                        value: {
                            var v = imageData.opticalParamReply
                            var currentDesc = null;
                            switch(v) {

                                case 0xE1: currentDesc = "非卫星图模板装订成功"; break
                                case 0xE2: currentDesc = "卫星图模板装订成功"; break
                                case 0xE3: currentDesc = "模板装订中"; break
                                case 0xE4: currentDesc = "红外非均匀校正成功"; break
                                case 0xE5: currentDesc = "模板正在擦除"; break
                                case 0xE6: currentDesc = "模板擦除成功"; break
                                case 0xE7: currentDesc = "非卫星图模板装订失败"; break
                                case 0xE8: currentDesc = "卫星图模板装订失败"; break
                                case 0xE9: currentDesc = "盲元校正成功"; break
                                default: break
                            }
                            if (currentDesc !== null) {
                                root.lastValidDesc2 = currentDesc;
                            }
                            return root.lastValidDesc2;
                        }
                        valueColor: {
                            var v = imageData.opticalParamReply
                            if (v === 0xE3 || v === 0xE5) return "#e68a00"
                            if (v === 0xE7 || v === 0xE8) return "#d93025"
                            return "#1a73e8"
                        }
                    }
                    DataLabel {
                        fontSize: 16
                        labelWidth: 105
                        valueWidth: 165
                        label: "当前工作通道:"
                        value: imageData.currentWorkChannel === 0x02 ? "红外" : (imageData.currentWorkChannel === 0x03 ? "电视" : "未知")
                    }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 165; label: "自检标志:"; value: "0x" + imageData.selfCheckFlag.toString(16).toUpperCase() }
                }

                Column {
                    spacing: 6
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 85; label: "目标类型:"; value: switch(imageData.m_targetBackgroundType1) {
                                case 0x00: return "车辆"
                                case 0x01: return "小型建筑物"
                                case 0x02: return "坦克"
                                case 0x04: return "舰船"
                                case 0x07: return "靶标"
                                default: return "未知"
                            } }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 85; label: "目标灰度类型:"; value: switch(imageData.m_targetBackgroundType2) {
                                case 0x00: return "亮目标"
                                case 0x01: return "暗目标"
                                default: return "未知"
                            } }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 85; label: "目标动/静:"; value: switch(imageData.m_targetBackgroundType3) {
                                case 0x00: return "静目标"
                                case 0x01: return "动目标"
                                default: return "未知"
                            } }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 85; label: "背景类型:"; value: switch(imageData.m_targetBackgroundType4) {
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
                        fontSize: 16
                        labelWidth: 105
                        valueWidth: 150
                        label: "光学工作状态:"
                        value: {
                            switch(imageData.opticalWorkState) {
                                case 0x02: return "搜索状态"
                                //跟踪到目标时弹窗提示2-3秒
                                case 0x03: return "跟踪状态"
                                case 0x04: return "框架角电锁零位状态"
                                case 0x05: return "记忆状态"
                                case 0x06: return "解锁状态"
                                default: return "未知状态"
                            }
                        }
                    }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 150; label: "修正指令状态:"; value: imageData.correctionCmdStatus === 1 ? "修正状态" : "非修正状态" }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 150; label: "修正指令次数:"; value: imageData.correctionCmdCount }
                }
            }
        }
    }

    // ═══ 跟踪 / 角度 / 伺服平台信息（来自接收区）═══
    Row {
        id: recvInfoRow2
        spacing: 8

        anchors.top: recvInfoRow.bottom
        anchors.topMargin: 8
        anchors.left: recvInfoRow.left

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
            label: Label {
                text: parent.title
                font.pixelSize: 18

                leftPadding: 12     //调整标题位置
                topPadding: 6
            }

            Column {
                spacing: 6

                DataLabel {
                    fontSize: 16
                    labelWidth: 105
                    valueWidth: 80
                    label: "跟踪状态:"
                    property int currentState: imageData.trackingState
                    onCurrentStateChanged: {
                        if (currentState === 0x22) testmsg.showToast("目标已丢失")
                        else if (currentState === 0x33) testmsg.showToast("已锁定目标")
                    }
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
                    fontSize: 16
                    labelWidth: 105
                    valueWidth: 80
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
                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "方位偏差像素:"; value: imageData.azimuthDeviationPixel }
                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "俯仰偏差像素:"; value: imageData.pitchDeviationPixel }
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
            label: Label {
                text: parent.title
                font.pixelSize: 18

                leftPadding: 12     //调整标题位置
                topPadding: 6
            }

            Column {
                spacing: 6

                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "俯仰框架角:"; value: imageData.pitchFrameAngle.toFixed(2) + "°" }
                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "偏航框架角:"; value: imageData.yawFrameAngle.toFixed(2) + "°" }
                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "方位主令:"; value: imageData.azimuthMasterCmd }
                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "俯仰主令:"; value: imageData.pitchMasterCmd }
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
            label: Label {
                text: parent.title
                font.pixelSize: 18

                leftPadding: 12     //调整标题位置
                topPadding: 6
            }

            Column {
                spacing: 6

                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "平台自检结果:"; value: imageData.platformSelfCheck }
                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "伺服运行时间:"; value: imageData.servoRunningTime + " s" }
                DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "伺服阶跃:"; value: imageData.servoStep }
            }
        }
    }

    // ═══ 角速度陀螺 / 其他信息（来自接收区）═══
    Row {
        id: recvInfoRow3
        spacing: 8

        anchors.top: recvInfoRow2.bottom
        anchors.topMargin: 8
        anchors.left: recvInfoRow2.left

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
            label: Label {
                text: parent.title
                font.pixelSize: 18

                leftPadding: 12     //调整标题位置
                topPadding: 6
            }

            Row {
                spacing: 8

                Column {
                    spacing: 6
                    DataLabel { fontSize: 16; labelWidth: 120; valueWidth: 80; label: "俯仰视线角速度:"; value: imageData.pitchLosAngVel.toFixed(2) + "°/s" }
                    DataLabel { fontSize: 16; labelWidth: 120; valueWidth: 80; label: "偏航视线角速度:"; value: imageData.yawLosAngVel.toFixed(2) + "°/s" }
                    DataLabel { fontSize: 16; labelWidth: 120; valueWidth: 80; label: "俯仰陀螺:"; value: imageData.pitchGyro.toFixed(2) + "°/s" }
                }
                Column {
                    spacing: 6
                    DataLabel { fontSize: 16; labelWidth: 120; valueWidth: 80; label: "偏航陀螺:"; value: imageData.yawGyro.toFixed(2) + "°/s" }
                    DataLabel { fontSize: 16; labelWidth: 120; valueWidth: 80; label: "方位陀螺输出:"; value: imageData.azimuthGyroOutput.toFixed(2) + "°/s" }
                    DataLabel { fontSize: 16; labelWidth: 120; valueWidth: 80; label: "俯仰陀螺输出:"; value: imageData.pitchGyroOutput.toFixed(2) + "°/s" }
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
            label: Label {
                text: parent.title
                font.pixelSize: 18

                leftPadding: 12     //调整标题位置
                topPadding: 6
            }

            Row {
                spacing: 8

                Column {
                    spacing: 6
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "红外帧编号:"; value: imageData.infraredFrameNum }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "红外帧频:"; value: imageData.infraredFrameRate + " Hz" }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "电视帧频:"; value: imageData.tvFrameRate + " Hz" }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "Cbh_tv4405:"; value: imageData.cbhTv4405 }
                }
                Column {
                    spacing: 6

                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "波门尺寸:"; value: imageData.gateSize }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "软件版本1:"; value: imageData.softwareVersion1 }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "软件版本2:"; value: imageData.softwareVersion2 }
                    DataLabel { fontSize: 16; labelWidth: 105; valueWidth: 80; label: "软件版本3:"; value: imageData.softwareVersion3 }
                }
            }
        }
    }

    // 连接到 imageSerial 信号
    // Connections {
    //     target: imageSerial
    //     function onErrorOccurred(msg) {
    //         console.log("Serial error:", msg)
    //     }
    //     function onConnected() {
    //         console.log("Serial connected")
    //     }
    //     function onDisconnected() {
    //         console.log("Serial disconnected")
    //     }

    // }
}
