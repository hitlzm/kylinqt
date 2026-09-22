import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
import "./"

Item{
    id:root
    anchors.fill: parent

    property var controlNames: [
        "三轴控制",
        "内框控制",
        "中框控制",
        "外框控制"
    ]

    property var myNumbers:[]

    // 数字提取函数
    function extractNumbers(str) {
        // 使用正则分割：支持英文逗号、中文逗号、以及它们周围的空白字符
        var parts = str.split(/[,，\s]+/);
        var numbers = [];
        for (var i = 0; i < parts.length; i++) {
            var trimmed = parts[i].trim();
            if (trimmed === "") continue;
            var num = parseFloat(trimmed);
            if (!isNaN(num)) {
                numbers.push(num);
            }
        }
        return numbers;  // 返回数字数组
    }

    // 角度格式化：固定两位小数，非法值按 0 处理
    function formatAngle(value) {
        var num = Number(value)
        return isFinite(num) ? num.toFixed(2) : "0.00"
    }

    // 读取转台当前角度：TurntableData 中实时反馈的 current 角度
    // （inner_angle / middle_angle / outter_angle 即各轴当前角度）
    function currentAngles() {
        var angles = { inner: 0, middle: 0, outter: 0 }
        if (typeof turntableData !== "undefined" && turntableData) {
            angles.inner = Number(turntableData.inner_angle) || 0
            angles.middle = Number(turntableData.middle_angle) || 0
            angles.outter = Number(turntableData.outter_angle) || 0
        }
        return angles
    }

    // 一键填入起点：把当前角度写进各行「起点(度)」输入框
    function fillStartAngles() {
        var cur = currentAngles()
        // 各行起点文本：三轴控制依次为 内框、中框、外框
        var texts = [
            formatAngle(cur.inner) + ", " + formatAngle(cur.middle) + ", " + formatAngle(cur.outter),
            formatAngle(cur.inner),
            formatAngle(cur.middle),
            formatAngle(cur.outter)
        ]

        for (var i = 0; i < angleRowRepeater.count && i < texts.length; ++i) {
            var row = angleRowRepeater.itemAt(i)
            if (row) {
                row.startText = texts[i]
            }
        }

        // 同步写入发送数据对象，保证「发送数据」用的起点与界面一致
        // （程序改 text 不会触发 onEditingFinished）
        if (typeof turntableSendData !== "undefined" && turntableSendData) {
            turntableSendData.inner_startangle = cur.inner
            turntableSendData.middle_startangle = cur.middle
            turntableSendData.outter_startangle = cur.outter
        }
    }

    Row {
        id: sinRow
        spacing: 10

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent. left
        anchors.leftMargin: 20 

        // 一键填入起始位置
        CusButton_Blue {
            text: "一键填入起点"
            font.pixelSize: 15
            width: 100
            height: 35
            onClicked: {
                fillStartAngles()
            }
        }
        // 正弦运动幅度
        Text {
            text: "正弦运动幅度："
            font.pixelSize: 18
            font.bold: true
            color: "#000000"
            anchors.verticalCenter: sinInput.verticalCenter
        }
        CusTextField{
                    id:sinInput
                    horizontalAlignment: TextInput.AlignHCenter
                    // 垂直居中：设置垂直居中对齐和相同的上下内边距
                    verticalAlignment: TextInput.AlignVCenter
                    height: 35
                    width: 120
                    //区分是哪种控制方式，给不同变量赋值
                    onEditingFinished: {
                        var mynumber = extractNumbers(text);    
                    }
        }
        // 正弦运动频率
        Text {
            text: "正弦运动频率："
            font.pixelSize: 18
            font.bold: true
            color: "#000000"
            anchors.verticalCenter: sinInput.verticalCenter
        }
        CusTextField{
                    id:frequencyInput
                    horizontalAlignment: TextInput.AlignHCenter
                    // 垂直居中：设置垂直居中对齐和相同的上下内边距
                    verticalAlignment: TextInput.AlignVCenter
                    height: 35
                    width: 120
                    //区分是哪种控制方式，给不同变量赋值
                    onEditingFinished: {
                        var mynumber = extractNumbers(text);    
                    }
        }
    }

    ColumnLayout{
        //anchors.fill: parent
        //anchors.width: parent.width
        anchors.left: parent.left
        anchors.leftMargin: 10  
        anchors.top: sinRow.bottom
        anchors.topMargin: 20
        spacing: 25
        RowLayout{
            spacing: 10
            Layout.alignment: Qt.AlignLeft

            // 占位：对齐下方控制名列（width: 120）
            Item { Layout.preferredWidth: 80 }

            Text {
                text: "起点(度)"
                font.pixelSize: 20
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 150
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                text: "终点(度)"
                font.pixelSize: 20
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 150
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                text: "运动时间（秒）"
                font.pixelSize: 20
                font.bold: true
                color: "#000000"
                Layout.preferredWidth: 100
                horizontalAlignment: Text.AlignHCenter
            }
            // 占位：对齐下方按钮列（Layout.fillWidth: true）
            Item { Layout.fillWidth: true }
        }

        Repeater {
            id: angleRowRepeater
            model: root.controlNames
            delegate: RowLayout {
                spacing: 10
                Layout.alignment: Qt.AlignLeft

                // 供「一键填入起点」按钮访问本行起点输入框
                property alias startText: input1.text

                Text {
                    text: controlNames[index]
                    font.pixelSize: 20
                    font.bold: true
                    color: '#000000'
                    Layout.preferredWidth: 80
                }

                CusTextField{
                    id:input1
                    // width:150
                    horizontalAlignment: TextInput.AlignHCenter
                    // 垂直居中：设置垂直居中对齐和相同的上下内边距
                    verticalAlignment: TextInput.AlignVCenter
                    Layout.preferredHeight: 40
                    Layout.preferredWidth: 150
                    //区分是哪种控制方式，给不同变量赋值
                    onEditingFinished: {
                        var mynumber = extractNumbers(text);
                        if (index === 0) {                            // 三轴控制
                            turntableSendData.inner_startangle = mynumber[0] || 0;

                            turntableSendData.middle_startangle = mynumber[1] || 0;

                            turntableSendData.outter_startangle = mynumber[2] || 0;
                            turntableSendData.index = 0;
                            
                        } else if (index === 1) {
                            turntableSendData.inner_startangle = Number(text)
                            // turntableSendData.index = 1;
                        } else if (index === 2) {
                            turntableSendData.middle_startangle = Number(text)
                            // turntableSendData.index = 2;
                        } else if (index === 3) {
                            turntableSendData.outter_startangle = Number(text)
                            // turntableSendData.index = 3;
                        } 
                    }
                }
                CusTextField{
                    id:input2
                    // width:150
                    Layout.preferredHeight: 40
                    Layout.preferredWidth: 150
                    horizontalAlignment: TextInput.AlignHCenter
                    // 垂直居中：设置垂直居中对齐和相同的上下内边距
                    verticalAlignment: TextInput.AlignVCenter

                    onEditingFinished: {
                        var mynumber = extractNumbers(text);
                        if (index === 0) {                            // 三轴控制

                            turntableSendData.inner_endangle = mynumber[0] || 0;
                            
                            turntableSendData.middle_endangle = mynumber[1] || 0;
                            
                            turntableSendData.outter_endangle = mynumber[2] || 0;

                        } else if (index === 1) {
                            turntableSendData.inner_endangle = Number(text)
                        } else if (index === 2) {
                            turntableSendData.middle_endangle = Number(text)
                        }else if (index === 3) {
                            turntableSendData.outter_endangle = Number(text)
                        }  
                    }
                }
                CusTextField{
                    id:input3
                    // width:150
                    Layout.preferredHeight: 40
                    Layout.preferredWidth: 100
                    horizontalAlignment: TextInput.AlignHCenter
                    // 垂直居中：设置垂直居中对齐和相同的上下内边距
                    verticalAlignment: TextInput.AlignVCenter
                    onEditingFinished: {
                            turntableSendData.runtime = Number(text)
                    }
                }
                CusButton_Blue {
                id: sendButton
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                text: "发送数据"

                onClicked: {
                    turntableSendData.buildFrame(index)
                }
                }

                CusButton_Blue {
                id: sinmoveButton
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                text: "正弦运动"
                onClicked: {
                    turntableSendData.buildFrame(index)
                }
                }
                
            }
        }
    }
}
