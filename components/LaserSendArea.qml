import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"
Rectangle {
    id: root
    width: 1200
    height: 610
    color: '#e9f0f9'

//消息提示框
    MessagePopup {
        id: msg
    }
    MsgPopup2 {
        id: toastmsg
    }

    // 输入校验标志（默认 true，未输入不算无效）
    property bool azimuthAngleValid: true
    property bool elevationAngleValid: true
    property bool searchCenterAzimuthValid: true
    property bool searchCenterElevationValid: true
    property bool azimuthSearchRangeValid: true
    property bool elevationSearchRangeValid: true
    property bool searchRadiusValid: true
//下边沿
    Rectangle {
        width: parent.width
        height: 1
        color: "black"
        anchors.bottom: parent.bottom   // 贴在主矩形下边沿
    }
    property var commandList: [

        "无动作",
        "自检",
        "锁定",
        "参数装订",
        "定轴搜索",
        "矩形搜索",
        "圆形搜索",
        "定轴位置",
        "矩形位置",
        "圆形位置"
    ]

    property int currentCmd: 0

    // 左侧命令区
    Rectangle {
        id: commandArea
        width: 160
        height: 400
        color: '#faf7f7'
        radius: 4

        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 80

        Column {
            spacing: 6
            anchors.fill: parent
            anchors.margins: 12
            Text {
                text: "激光导引头控制字"
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
                    

                    //在一种模式下只有特定的输入框可以使用，被禁用的输入框发送内容应为全0
                    //加上在不同模式下，对激光导引头发送变量的设置
                    onClicked: {root.currentCmd = index
                        // 切换模式时重置所有校验标志
                        root.azimuthAngleValid = true
                        root.elevationAngleValid = true
                        root.searchCenterAzimuthValid = true
                        root.searchCenterElevationValid = true
                        root.azimuthSearchRangeValid = true
                        root.elevationSearchRangeValid = true
                        root.searchRadiusValid = true

                         if (index === 0) {
                            firstColumn.disabledIndices = [0,1]      // 都禁用
                            secondColumn.disabledIndices = [0,1]
                            thirdColumn.disabledIndices = [0,1]
                            fourthColumn.disabledIndices = [0,1]
                            laserSendData.m_cmd= 0x00
                            } else if (index === 1) {
                            firstColumn.disabledIndices = [0,1]      // 都禁用
                            secondColumn.disabledIndices = [0,1]
                            thirdColumn.disabledIndices = [0,1]
                            fourthColumn.disabledIndices = [0,1]
                            laserSendData.m_cmd= 0x01
                            } else if (index === 2) {
                            firstColumn.disabledIndices = [0]      
                            secondColumn.disabledIndices = [1]
                            thirdColumn.disabledIndices = [0,1]
                            fourthColumn.disabledIndices = [0,1]
                            laserSendData.m_cmd= 0x02
                            } else if (index === 3) {
                            firstColumn.disabledIndices = [1]      
                            secondColumn.disabledIndices = [0,1]
                            thirdColumn.disabledIndices = [0,1]
                            fourthColumn.disabledIndices = [0,1]
                            laserSendData.m_cmd= 0x03
                            }
                            else if (index === 4 || index === 7) {
                            firstColumn.disabledIndices = [0]      
                            secondColumn.disabledIndices = [1]
                            thirdColumn.disabledIndices = [0,1]
                            fourthColumn.disabledIndices = [0,1]
                            if(index === 4){
                                laserSendData.m_cmd= 0x06
                            }else{
                                laserSendData.m_cmd= 0x09
                            }
                            }
                            else if (index === 5 || index === 8) {
                            firstColumn.disabledIndices = [0,1]      
                            secondColumn.disabledIndices = [0]
                            thirdColumn.disabledIndices = []
                            fourthColumn.disabledIndices = [1]
                            if(index === 5){
                                laserSendData.m_cmd= 0x07
                            }else{
                                laserSendData.m_cmd= 0x0A
                            }
                            }
                            else if (index === 6 || index === 9) {
                            firstColumn.disabledIndices = [0,1]      
                            secondColumn.disabledIndices = [0]
                            thirdColumn.disabledIndices = [1]
                            fourthColumn.disabledIndices = [0]
                            if(index === 6){
                                laserSendData.m_cmd= 0x08
                            }else{
                                laserSendData.m_cmd= 0x0B
                            }
                            }
                }
            }
        }
    }
    
    }

   //上部控制区，串口选择，波特率，按钮等
   // 串口选择 - 绑定到 C++ laserSerial 对象
    Text {
        id: serialLabel
        text: "串口号："
        font.pixelSize: 18
        font.bold: true
        color: "#000000"
        anchors.verticalCenter: serialComboBox.verticalCenter
        anchors.right: serialComboBox.left
        anchors.rightMargin: 6
    }
    CusComboBox {
        id: serialComboBox
        width: 220
        height: 42

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: commandArea.right
        anchors.leftMargin: -30

        model: laserData.availablePorts
    }

    // 波特率
    Text {
        id: baudLabel
        text: "波特率："
        font.pixelSize: 18
        font.bold: true
        color: "#000000"
        anchors.verticalCenter: baudComboBox.verticalCenter
        anchors.left: serialComboBox.right
        anchors.leftMargin: 6
    }
    CusComboBox {
        id: baudComboBox
        width: 220
        height: 42

        anchors.top: serialComboBox.top
        anchors.left: baudLabel.right
        anchors.leftMargin: 10

        model: ["115200"]
    }

    CusButton_Blue {
        id: scanButton
        width: 120
        height: 42
        text: "扫描串口"

        anchors.top: serialComboBox.top
        anchors.left: baudComboBox.right
        anchors.leftMargin: 30

        onClicked: {
            laserData.requestScanPorts()
        }
    }
    // 打开串口按钮
    CusButton_Blue {
        id: openButton
        width: 120
        height: 42
        text: laserData.portOpen ? "关闭串口" : "打开串口"

        anchors.top: serialComboBox.top
        anchors.left: scanButton.right
        anchors.leftMargin: 30
        Timer {
            id: delayTimer
            interval: 1  
            onTriggered: {
                // if (laserData.portOpen) tip.show()
                if (laserData.portOpen) {msg.message = "串口已打开！"; msg.open()}
                else { msg.message = "串口打开失败！"; msg.open() }
            }
        }
        onClicked: {
            if (laserData.portOpen) {
                laserData.closePort()
            } else {
                laserData.openPort(serialComboBox.currentText,
                                    parseInt(baudComboBox.currentText))
                // if (laserData.portOpen) {tip.show()} 
                // else{tip1.show()}
                delayTimer.start()
            }
        }
        // onClicked: {
        //     if (laserData.portOpen) {
        //         laserData.closePort()
        //     } else {
        //         laserData.openPort(serialComboBox.currentText,
        //                            parseInt(baudComboBox.currentText))
        //     }
        // }
    }

    // 发送数据按钮
    CusButton_Blue {
        id: sendButton
        width: 120
        height: 42
        text: "发送数据"
        enabled: laserData.portOpen
                  && azimuthAngleValid && elevationAngleValid
                  && searchCenterAzimuthValid && searchCenterElevationValid
                  && azimuthSearchRangeValid && elevationSearchRangeValid
                  && searchRadiusValid

        anchors.top: serialComboBox.top
        anchors.left: openButton.right
        anchors.leftMargin: 30

        onClicked: {
            // var data = ""      //打包数据
            // root.sendData(data)
            //直接调用对应串口里的发送函数就行，最后实现效果是固定周期的发送
            laserSendData.buildFrame()
            console.log("激光导引头数据已发送")
        }
    }

    
    // 左侧输入框
    Column {
        id: firstColumn
        spacing: 5

        anchors.top: serialComboBox.bottom
        anchors.topMargin: 30
        // anchors.left:commandArea.right
        // anchors.leftMargin: 120
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -120
        property var disabledIndices: []
        Repeater {
            model: [
                { title: "激光周期", unit: "ms" },
                { title: "方位角度", unit: "°" }
            ]
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                enabled: firstColumn.disabledIndices.indexOf(index) === -1

                // 实时校验
                onTextChanged: {
                    if (index === 1) {
                        var val = Number(text)
                        if (!isNaN(val) && (val > 20 || val < -20)) {
                            if (root.azimuthAngleValid)
                                toastmsg.showToast("方位角输入范围为-20°~ 20°,请重新输入")
                            root.azimuthAngleValid = false
                        } else {
                            root.azimuthAngleValid = true
                        }
                    }
                }

                onEditingFinished: {
                    if (index === 0) {
                        laserSendData.m_laserPeriod = Number(text)
                    } else if (index === 1) {
                        if(Number(text) >= -20 && Number(text) <= 20)
                                laserSendData.m_azimuthAngle = Number(text)
                    }
                }
            }
        }
    }
     
    // 右侧输入框
    Column {
        id: secondColumn
        spacing: 5

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: firstColumn.right
        anchors.leftMargin: 30
        property var disabledIndices: []
        Repeater {
            model: [
                { title: "俯仰角度", unit: "°" },
                { title: "搜索中心方位角度", unit: "°" }
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                enabled: secondColumn.disabledIndices.indexOf(index) === -1

                // 实时校验
                onTextChanged: {
                    if (index === 0) {
                        var val = Number(text)
                        if (!isNaN(val) && (val > 20 || val < -20)) {
                            if (root.elevationAngleValid)
                                toastmsg.showToast("俯仰角输入范围为-20°~ 20°,请重新输入")
                            root.elevationAngleValid = false
                        } else {
                            root.elevationAngleValid = true
                        }
                    } else if (index === 1) {
                        var val = Number(text)
                        if (!isNaN(val) && (val > 20 || val < -20)) {
                            if (root.searchCenterAzimuthValid)
                                toastmsg.showToast("搜索中心方位角输入范围为-20°~ 20°,请重新输入")
                            root.searchCenterAzimuthValid = false
                        } else {
                            root.searchCenterAzimuthValid = true
                        }
                    }
                }

                onEditingFinished: {
                    if (index === 0) {
                        if(Number(text) >= -20 && Number(text) <= 20)
                                laserSendData.m_elevationAngle = Number(text)
                    } else if (index === 1) {
                        if(Number(text) >= -20 && Number(text) <= 20)
                                laserSendData.m_searchCenterAzimuth = Number(text)
                    }
                }
            }
        }
    }

    Column {
        id: thirdColumn
        spacing: 5

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: secondColumn.right
        anchors.leftMargin: 30
        property var disabledIndices: []
        Repeater {
            model: [
                { title: "搜索中心俯仰角度", unit: "°" },
                { title: "方位搜索范围", unit: "°" }
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                enabled: thirdColumn.disabledIndices.indexOf(index) === -1

                // 实时校验
                onTextChanged: {
                    if (index === 0) {
                        var val = Number(text)
                        if (!isNaN(val) && (val > 20 || val < -20)) {
                            if (root.searchCenterElevationValid)
                                toastmsg.showToast("搜索中心俯仰角输入范围为-20°~ 20°,请重新输入")
                            root.searchCenterElevationValid = false
                        } else {
                            root.searchCenterElevationValid = true
                        }
                    } else if (index === 1) {
                        var val = Number(text)
                        if (!isNaN(val) && (val > 20 || val < -20)) {
                            if (root.azimuthSearchRangeValid)
                                toastmsg.showToast("方位搜索范围为-20°~ 20°,请重新输入")
                            root.azimuthSearchRangeValid = false
                        } else {
                            root.azimuthSearchRangeValid = true
                        }
                    }
                }

                onEditingFinished: {
                    if (index === 0) {
                        if(Number(text) >= -20 && Number(text) <= 20)
                                laserSendData.m_searchCenterElevation = Number(text)
                    } else if (index === 1) {
                        if(Number(text) >= -20 && Number(text) <= 20)
                                laserSendData.m_azimuthSearchRange = Number(text)
                    }
                }
            }
        }
    }

    Column {
        id: fourthColumn
        spacing: 5

        anchors.top: sendButton.bottom
        anchors.topMargin: 30
        // anchors.horizontalCenter: sendButton.horizontalCenter
        anchors.left: thirdColumn.right
        anchors.leftMargin: 30
        property var disabledIndices: []
        Repeater {
            model: [
                { title: "俯仰搜索范围", unit: "°" },
                { title: "搜索半径", unit: "m" }
            ]
            
            delegate: MyTextField {
                mywidth: 120
                myheight: 60
                title: modelData.title
                labeltext: modelData.unit
                enabled: fourthColumn.disabledIndices.indexOf(index) === -1

                // 实时校验
                onTextChanged: {
                    if (index === 0) {
                        var val = Number(text)
                        if (!isNaN(val) && (val > 20 || val < -20)) {
                            if (root.elevationSearchRangeValid)
                                toastmsg.showToast("俯仰搜索范围为-20°~ 20°,请重新输入")
                            root.elevationSearchRangeValid = false
                        } else {
                            root.elevationSearchRangeValid = true
                        }
                    } else if (index === 1) {
                        var val = Number(text)
                        if (!isNaN(val) && (val > 20 || val < -20)) {
                            if (root.searchRadiusValid)
                                toastmsg.showToast("搜索半径范围为-20°~ 20°,请重新输入")
                            root.searchRadiusValid = false
                        } else {
                            root.searchRadiusValid = true
                        }
                    }
                }

                onEditingFinished: {
                    if (index === 0) {
                        if(Number(text) >= -20 && Number(text) <= 20)
                                laserSendData.m_elevationSearchRange = Number(text)
                    } else if (index === 1) {
                        if(Number(text) >= -20 && Number(text) <= 20)
                                laserSendData.m_searchRadius = Number(text)
                    }
                }
            }
        }
    }

    LaserRecvArea{
        myheight:320
        mywidth:800
        anchors.top: firstColumn.bottom
        anchors.topMargin: 30
        anchors.left: commandArea.right
        anchors.leftMargin: 100
        groupHeight1:150
    }
    
    // // 发送数据信号
    // signal sendData(string data)

    // 连接到 laserSerial 信号
    // Connections {
    //     target: laserSerial
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
