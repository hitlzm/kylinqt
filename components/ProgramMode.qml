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


ColumnLayout{
    anchors.fill: parent
    spacing: 10
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
            Layout.preferredWidth: 150
            horizontalAlignment: Text.AlignHCenter
        }
        // 占位：对齐下方按钮列（Layout.fillWidth: true）
        Item { Layout.fillWidth: true }
    }

    Repeater {
        model: root.controlNames
        delegate: RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignLeft

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
                    // var mynumbers = [];
                    mynumber = extractNumbers(text);
                    if (index === 0) {                            // 三轴控制
                        turntableSendData.inner_startangle = mynumber[0] || 0;
                        
                        turntableSendData.middle_startangle = mynumber[1] || 0;
                        
                        turntableSendData.outter_startangle = mynumber[2] || 0;
                        turntableSendData.index = 0;
                        
                    } else if (index === 1) {
                        turntableSendData.inner_startangle = Number(text)
                        turntableSendData.index = 1;
                    } else if (index === 2) {
                        turntableSendData.middle_startangle = Number(text)
                        turntableSendData.index = 2;
                    } else if (index === 3) {
                        turntableSendData.outter_startangle = Number(text)
                        turntableSendData.index = 3;
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
                    // var mynumbers = [];
                    mynumber = extractNumbers(text);
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
                Layout.preferredWidth: 150
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
                turntableSendData.buildFrame()
            }
        }

            
        }
    }
}
}