import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"
Rectangle {
    id: root
    width: 1200
    height: 560
    color: '#e9f0f9'
    //下边沿
    Rectangle {
        width: parent.width
        height: 1
        color: "black"
        anchors.bottom: parent.bottom   // 贴在主矩形下边沿
    }
    property var commandList: [

        "无动作",
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
                            imageSerial.imageSendData.m_seekerCtrlWord=0x00
                    } else if (currentCmd===1)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0x01
                    }
                    else if (currentCmd===2)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0x02
                    }else if (currentCmd===3)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0x03
                    }else if (currentCmd===4)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0x04
                    }else if (currentCmd===5)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0x06
                    }else if (currentCmd===6)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0x55
                    }else if (currentCmd===7)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0x40
                    }else if (currentCmd===8)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0xA1
                    }else if (currentCmd===9)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0xB1
                    }else if (currentCmd===10)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0xED
                    }
                    else if (currentCmd===11)
                    {
                            imageSerial.imageSendData.m_seekerCtrlWord=0xEE
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

        model: imageSerial.availablePorts
    }

    // 波特率
    CusComboBox {
        id: baudComboBox
        width: 120
        height: 32

        anchors.top: serialComboBox.top
        anchors.left: serialComboBox.right
        anchors.leftMargin: 30

        model: ["9600", "115200"]
    }
    CusPopup {
        id: tip
        backgroundWidth: 100
        backgroundHeight: 40
        barColor:"lightgray"
        contentItem: Text { text: "串口已打开！" }
        // Button { text: "确定"; onClicked: tip.hide() }
    
    }
    // 打开串口按钮
    CusButton_Blue {
        id: openButton
        width: 120
        height: 32
        text: imageSerial.portOpen ? "关闭串口" : "打开串口"

        anchors.top: serialComboBox.top
        anchors.left: baudComboBox.right
        anchors.leftMargin: 30

        onClicked: {
            if (imageSerial.portOpen) {
                imageSerial.closePort()
            } else {
                imageSerial.openPort(serialComboBox.currentText,
                                     parseInt(baudComboBox.currentText))
                                 tip.myshow();
            }
        }
    }

    // 发送数据按钮
    CusButton_Blue {
        id: sendButton
        width: 120
        height: 32
        text: "发送数据"
        enabled: imageSerial.portOpen

        anchors.top: serialComboBox.top
        anchors.left: openButton.right
        anchors.leftMargin: 30

        onClicked: {
            imageSerial.sendData(imageSerial.imageSendData.buildFrame())
            console.log("图像导引头数据已发送")
        }
    }

    CusButton_Blue {
        id: photobutton
        width: 120
        height: 32
        text: "拍摄参考图"
        anchors.top: serialComboBox.top
        anchors.left: sendButton.right
        anchors.leftMargin: 30
        onClicked: {

        }
    }

    MySwitch {
        id: mySwitch
        width: 120
        height: 32
        title: "字符叠加"
        anchors.top: photobutton.top
        anchors.left: photobutton.right
        anchors.leftMargin: 30
    }

    // 左侧输入框
    Column {
        id: leftInputColumn
        spacing: 3

        anchors.top: serialComboBox.bottom
        anchors.topMargin: 10
        anchors.left:serialComboBox.left

        Repeater {

            model: [
            { title: "弹目距离", unit: "m" },
            { title: "导弹速度",  unit: "m/s" },
            { title: "弹体俯仰角",  unit: "°" },
            { title: "弹体方位角",  unit: "°" },
            { title: "弹体滚转角",  unit: "°" },
            { title: "弹体俯仰角速度",  unit: "°/s" },
            { title: "修正图像帧序号",  unit: "" }

            ]

            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                onEditingFinished: {
                    if (index === 0) {
                        imageSerial.imageSendData.m_missileTargetDistance = parseInt(text)
                    } else if (index === 1) {
                        imageSerial.imageSendData.m_missileSpeed = Number(text)
                    } else if (index === 2) {
                        imageSerial.imageSendData.m_bodyPitchAngle = Number(text)
                    } else if (index === 3) {
                        imageSerial.imageSendData.m_bodyYawAngle = Number(text)
                    } else if (index === 4) {
                        imageSerial.imageSendData.m_bodyRollAngle = Number(text)
                    } else if (index === 5) {
                        imageSerial.imageSendData.m_bodyPitchRate = Number(text)
                    } else if (index === 6) {
                        imageSerial.imageSendData.m_correctionFrameNum = parseInt(text)
                    }
                }
            }
        }
    }

    // 右侧输入框
    Column {
        id: rightInputColumn
        spacing: 3

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        anchors.left: leftInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: [
            { title: "弹体方位角速度",  unit: "°/s" },
            { title: "弹体滚转角速度", unit: "m/s" },
            { title: "弹体X向速度",  unit: "m/s" },
            { title: "弹体Y向速度",  unit: "m/s" },
            { title: "弹体Z向速度",  unit: "m/s" },
            { title: "弹体X坐标",  unit: "m" }

            ]
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                onEditingFinished: {
                    if (index === 0) {
                        imageSerial.imageSendData.m_bodyYawRate = Number(text)
                    } else if (index === 1) {
                        imageSerial.imageSendData.m_bodyRollRate = Number(text)
                    } else if (index === 2) {
                        imageSerial.imageSendData.m_bodyVelX = Number(text)
                    } else if (index === 3) {
                        imageSerial.imageSendData.m_bodyVelY = Number(text)
                    } else if (index === 4) {
                        imageSerial.imageSendData.m_bodyVelZ = Number(text)
                    } else if (index === 5) {
                        imageSerial.imageSendData.m_bodyPosX = parseInt(text)
                    }
                }
            }
        }

    }
     MyComboBox {
            id: myCombox1
            anchors.top: rightInputColumn.bottom
            anchors.topMargin: 3
            anchors.left: rightInputColumn.left
            mywidth: 180
            myheight: 55
            title: "光学参数装订控制字"
            model: ["默认值", "非卫星图模板制作", "卫星图模板制作", "盲元校正","红外非均匀校正","模板擦除","积分时间设置"]
            onCurrentIndexChanged:{

                root.opticalParamCtrlCmd=currentIndex

                if(opticalParamCtrlCmd===0){
                    imageSerial.imageSendData.m_opticalParamCtrl=0x00
                }else if(opticalParamCtrlCmd===1){
                    imageSerial.imageSendData.m_opticalParamCtrl=0xE1
                }else if(opticalParamCtrlCmd===2){
                    imageSerial.imageSendData.m_opticalParamCtrl=0xE2
                }else if(opticalParamCtrlCmd===3){
                    imageSerial.imageSendData.m_opticalParamCtrl=0xE3
                }else if(opticalParamCtrlCmd===4){
                    imageSerial.imageSendData.m_opticalParamCtrl=0xE4
                }else if(opticalParamCtrlCmd===5){
                    imageSerial.imageSendData.m_opticalParamCtrl=0xE5
                }else if(opticalParamCtrlCmd===6){
                    imageSerial.imageSendData.m_opticalParamCtrl=0xE6
                }
            }
        }
    MyComboBox {
            id: myCombox2
            anchors.top: myCombox1.top
            anchors.left: myCombox1.right
            anchors.leftMargin: 50
            mywidth: 90
            myheight: 55
            title: "模板序号"
            model: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10"]
            onCurrentIndexChanged: {
               if (currentIndex !== -1) {
               imageSerial.imageSendData.m_templateIndex = currentIndex + 1;
             }
            }
        }
    MyComboBox {
            id: myCombox3
            anchors.top: myCombox2.top
            anchors.left: myCombox2.right
            anchors.leftMargin: 50
            mywidth: 140
            myheight: 55
            title: "预装目标类型"
            model: ["车辆", "小型建筑物", "坦克", "舰船", "靶标"]
            onCurrentIndexChanged: {
               if (currentIndex === 0) {
               imageSerial.imageSendData.m_targetBackgroundType1 = 0x00;
             }else if(currentIndex === 1){
                imageSerial.imageSendData.m_targetBackgroundType1 = 0x01;
             }else if(currentIndex === 2){
                imageSerial.imageSendData.m_targetBackgroundType1 = 0x02;
             }else if(currentIndex === 3){
                imageSerial.imageSendData.m_targetBackgroundType1 = 0x04;
             }else if(currentIndex === 4){
                imageSerial.imageSendData.m_targetBackgroundType1 = 0x07;
             }
            }
        }
    MyComboBox {
            id: myCombox4
            anchors.top: myCombox3.top
            anchors.left: myCombox3.right
            anchors.leftMargin: 50
            mywidth: 100
            myheight: 55
            title: "背景类型"
            model: ["平原","沙漠", "岛岸","山地","丛林","公路","城市","湖泊"]
            onCurrentIndexChanged: {
               if (currentIndex === 0) {
               imageSerial.imageSendData.m_targetBackgroundType4 = 0x00;
             }else if(currentIndex === 1){
                imageSerial.imageSendData.m_targetBackgroundType4 = 0x01;
             }else if(currentIndex === 2){
                imageSerial.imageSendData.m_targetBackgroundType4 = 0x02;
             }else if(currentIndex === 3){
                imageSerial.imageSendData.m_targetBackgroundType4 = 0x03;
             }else if(currentIndex === 4){
                imageSerial.imageSendData.m_targetBackgroundType4 = 0x04;
             }else if(currentIndex === 5){
                imageSerial.imageSendData.m_targetBackgroundType4 = 0x05;
             }else if(currentIndex === 6){
                imageSerial.imageSendData.m_targetBackgroundType4 = 0x06;
             }else if(currentIndex === 7){
                imageSerial.imageSendData.m_targetBackgroundType4 = 0x07;
             }
            }
        }
     MyComboBox {
            id: myCombox5
            anchors.top: leftInputColumn.bottom
            anchors.topMargin:3
            anchors.left: leftInputColumn.left
            mywidth: 120
            myheight: 55
            title: "目标灰度类型"
            model: ["亮目标", "暗目标"]
            onCurrentIndexChanged: {
               if (currentIndex !== -1) {
               imageSerial.imageSendData.m_targetBackgroundType2 = currentIndex;
               }
            }
        }
        MyComboBox {
            id: myCombox6
            anchors.top: myCombox5.top
            anchors.left: myCombox5.right
            anchors.leftMargin: 50
            mywidth: 120
            myheight: 55
            title: "目标动静类型"
            model: ["静目标","动目标" ]
            onCurrentIndexChanged: {
               if (currentIndex !== -1) {
               imageSerial.imageSendData.m_targetBackgroundType3 = currentIndex;
               }
             
            }
        }
        MyComboBox {
            id: myCombox7
              anchors.top: myCombox6.top
            anchors.left: myCombox6.right
            anchors.leftMargin: 50
            mywidth: 140
            myheight: 55
            title: "修正指令状态"
            model: ["非修正状态","修正状态" ]
             onCurrentIndexChanged: {
               if (currentIndex !== -1) {
               imageSerial.imageSendData.m_trackingCorrectionCmd = currentIndex;
               }
            }
        }
        MyComboBox {
            id: myCombox8
              anchors.top: myCombox7.top
            anchors.left: myCombox7.right
            anchors.leftMargin: 50
            mywidth: 120
            myheight: 55
            title: "波门大小"
            model: ["大","小" ]
            onCurrentIndexChanged: {
               if (currentIndex === 0) {
               imageSerial.imageSendData.m_gateSize = 0xaa;
             }else if(currentIndex === 1){
                imageSerial.imageSendData.m_gateSize = 0x55;
             }
            }
        }
    Column {
        id: thirdInputColumn
        spacing: 3

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        anchors.left: rightInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: [
            { title: "弹体Y坐标",  unit: "m" },
            { title: "弹体Z坐标",  unit: "m" },
            { title: "俯仰框架角预装", unit: "°" },
            { title: "偏航框架角预装",  unit: "°" },
            { title: "红外积分时间",  unit: "ms" },
            { title: "修正后俯仰跟踪位置",  unit: "" }
            ]

            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                onEditingFinished: {
                    if (index === 0) {
                        imageSerial.imageSendData.m_bodyPosY = parseInt(text)
                    } else if (index === 1) {
                        imageSerial.imageSendData.m_bodyPosZ = parseInt(text)
                    } else if (index === 2) {
                        imageSerial.imageSendData.m_pitchGimbalPreset = Number(text)
                    } else if (index === 3) {
                        imageSerial.imageSendData.m_yawGimbalPreset = Number(text)
                    } else if (index === 4) {
                        imageSerial.imageSendData.m_irIntegrationTime = parseInt(text)
                    } else if (index === 5) {
                        imageSerial.imageSendData.m_correctedPitchPos = parseInt(text)
                    }
                }
            }
        }

    }

    Column {
        id:fourthInputColumn
        spacing: 3

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        anchors.left: thirdInputColumn.right
        anchors.leftMargin: 30
        Repeater {

            model: [
            { title: "修正后偏航跟踪位置",  unit: "" },
            { title: "搜索俯仰角速度",  unit: "°/s" },
            { title: "搜索方位角速度",  unit: "°/s" },
            { title: "目标海拔高度",  unit: "m" },
            { title: "飞机俯仰角",  unit: "°" },
            { title: "飞机方位角",  unit: "°" },
            ]
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                onEditingFinished: {
                    if (index === 0) {
                        imageSerial.imageSendData.m_correctedYawPos = parseInt(text)
                    } else if (index === 1) {
                        imageSerial.imageSendData.m_searchPitchRate = Number(text)
                    } else if (index === 2) {
                        imageSerial.imageSendData.m_searchYawRate = Number(text)
                    } else if (index === 3) {
                        imageSerial.imageSendData.m_targetAltitude = parseInt(text)
                    } else if (index === 4) {
                        imageSerial.imageSendData.m_aircraftPitch = Number(text)
                    } else if (index === 5) {
                        imageSerial.imageSendData.m_aircraftYaw = Number(text)
                    }
                }
            }
        }
    }
    Column {
        id:mInputColumn5
        spacing: 3

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        anchors.left: fourthInputColumn.right
        anchors.leftMargin: 30
        Repeater {
            model: [
            { title: "飞机滚转角",  unit: "°" },
            { title: "焦距",  unit: "mm" },
            { title: "吊舱俯仰框架角",  unit: "°" },
            { title: "吊舱方位框架角",  unit: "°" },
            { title: "卫星图比例",  unit: "" },
            { title: "吊舱类型",  unit: "" }

            ]

            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                onEditingFinished: {
                    if (index === 0) {
                        imageSerial.imageSendData.m_aircraftRoll = Number(text)
                    } else if (index === 1) {
                        imageSerial.imageSendData.m_focalLength = parseInt(text)
                    } else if (index === 2) {
                        imageSerial.imageSendData.m_podPitchAngle = Number(text)
                    } else if (index === 3) {
                        imageSerial.imageSendData.m_podYawAngle = Number(text)
                    } else if (index === 4) {
                        imageSerial.imageSendData.m_satelliteMapScale = parseInt(text)
                    } else if (index === 5) {
                        imageSerial.imageSendData.m_podType = parseInt(text)
                    }
                }
            }
        }
    }
     Column {
        id:mInputColumn6
        spacing: 3

        anchors.top: sendButton.bottom
        anchors.topMargin: 10
        anchors.left: mInputColumn5.right
        anchors.leftMargin: 30
        Repeater {
            model: [
            { title: "目标经度",  unit: "°" },
            { title: "目标纬度",  unit: "°" },
            { title: "飞机经度",  unit: "°" },
            { title: "飞机纬度",  unit: "°" },
            { title: "飞机海拔高度",  unit: "m" },
            { title: "像元尺寸",  unit: "μm" }
            ]

            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                onEditingFinished: {
                    if (index === 0) {
                        imageSerial.imageSendData.m_targetLongitude = Number(text)
                    } else if (index === 1) {
                        imageSerial.imageSendData.m_targetLatitude = Number(text)
                    } else if (index === 2) {
                        imageSerial.imageSendData.m_aircraftLongitude = Number(text)
                    } else if (index === 3) {
                        imageSerial.imageSendData.m_aircraftLatitude = Number(text)
                    } else if (index === 4) {
                        imageSerial.imageSendData.m_aircraftAltitude = parseInt(text)
                    } else if (index === 5) {
                        imageSerial.imageSendData.m_pixelSize = parseInt(text)
                    }
                }
            }
        }
    }

    // 连接到 imageSerial 信号
    Connections {
        target: imageSerial
        function onErrorOccurred(msg) {
            console.log("Serial error:", msg)
        }
        function onConnected() {
            console.log("Serial connected")
        }
        function onDisconnected() {
            console.log("Serial disconnected")
        }

    }
}
