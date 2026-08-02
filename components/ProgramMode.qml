import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
import "./"

Item{
    id:root
    anchors.fill: parent

    // 限位提示弹窗（需确认按钮）
    MessagePopup { id: limitmsg }

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
                    var mynumber = extractNumbers(text);
                    if (index === 0) {                            // 三轴控制
                        var innerVal  = mynumber[0]
                        var middleVal = mynumber[1]
                        var outerVal  = mynumber[2]

                        // 收集所有超限的轴
                        var errors = []
                        if (innerVal !== undefined && text.trim() !== "" && (isNaN(innerVal) || innerVal < -200 || innerVal > 200))
                            errors.push("内框(滚转): -200°~200°")
                        if (middleVal !== undefined && text.trim() !== "" && (isNaN(middleVal) || middleVal < -10 || middleVal > 70))
                            errors.push("中框(俯仰): -10°~70°")
                        if (outerVal !== undefined && text.trim() !== "" && (isNaN(outerVal) || outerVal < -100 || outerVal > 100))
                            errors.push("外框(方位): -100°~100°")

                        if (errors.length > 0) {
                            limitmsg.message = "输入超出范围！请按以下范围重新输入：\n" + errors.join("\n")
                            limitmsg.open()
                            text = ""
                            return
                        }

                        turntableSendData.inner_startangle = innerVal || 0;
                        turntableSendData.middle_startangle = middleVal || 0;
                        turntableSendData.outter_startangle = outerVal || 0;
                        turntableSendData.index = 0;

                    } else if (index === 1) {                     // 内框控制（滚转）
                        var val = Number(text)
                        if (text.trim() !== "" && (isNaN(val) || val < -200 || val > 200)) {
                            limitmsg.message = "内框(滚转)角度输入范围为-200°~200°，请重新输入"
                            limitmsg.open()
                            text = ""
                            return
                        }
                        turntableSendData.inner_startangle = val
                    } else if (index === 2) {                     // 中框控制（俯仰）
                        var val = Number(text)
                        if (text.trim() !== "" && (isNaN(val) || val < -10 || val > 70)) {
                            limitmsg.message = "中框(俯仰)角度输入范围为-10°~70°，请重新输入"
                            limitmsg.open()
                            text = ""
                            return
                        }
                        turntableSendData.middle_startangle = val
                    } else if (index === 3) {                     // 外框控制（方位）
                        var val = Number(text)
                        if (text.trim() !== "" && (isNaN(val) || val < -100 || val > 100)) {
                            limitmsg.message = "外框(方位)角度输入范围为-100°~100°，请重新输入"
                            limitmsg.open()
                            text = ""
                            return
                        }
                        turntableSendData.outter_startangle = val
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
                        var innerVal  = mynumber[0]
                        var middleVal = mynumber[1]
                        var outerVal  = mynumber[2]

                        // 收集所有超限的轴
                        var errors = []
                        if (innerVal !== undefined && text.trim() !== "" && (isNaN(innerVal) || innerVal < -200 || innerVal > 200))
                            errors.push("内框(滚转): -200°~200°")
                        if (middleVal !== undefined && text.trim() !== "" && (isNaN(middleVal) || middleVal < -10 || middleVal > 70))
                            errors.push("中框(俯仰): -10°~70°")
                        if (outerVal !== undefined && text.trim() !== "" && (isNaN(outerVal) || outerVal < -100 || outerVal > 100))
                            errors.push("外框(方位): -100°~100°")

                        if (errors.length > 0) {
                            limitmsg.message = "输入超出范围！请按以下范围重新输入：\n" + errors.join("\n")
                            limitmsg.open()
                            text = ""
                            return
                        }

                        turntableSendData.inner_endangle = innerVal || 0;
                        turntableSendData.middle_endangle = middleVal || 0;
                        turntableSendData.outter_endangle = outerVal || 0;

                    } else if (index === 1) {                     // 内框控制（滚转）
                        var val = Number(text)
                        if (text.trim() !== "" && (isNaN(val) || val < -200 || val > 200)) {
                            limitmsg.message = "内框(滚转)角度输入范围为-200°~200°，请重新输入"
                            limitmsg.open()
                            text = ""
                            return
                        }
                        turntableSendData.inner_endangle = val
                    } else if (index === 2) {                     // 中框控制（俯仰）
                        var val = Number(text)
                        if (text.trim() !== "" && (isNaN(val) || val < -10 || val > 70)) {
                            limitmsg.message = "中框(俯仰)角度输入范围为-10°~70°，请重新输入"
                            limitmsg.open()
                            text = ""
                            return
                        }
                        turntableSendData.middle_endangle = val
                    } else if (index === 3) {                     // 外框控制（方位）
                        var val = Number(text)
                        if (text.trim() !== "" && (isNaN(val) || val < -100 || val > 100)) {
                            limitmsg.message = "外框(方位)角度输入范围为-100°~100°，请重新输入"
                            limitmsg.open()
                            text = ""
                            return
                        }
                        turntableSendData.outter_endangle = val
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
                turntableSendData.buildFrame(index)
            }
        }

            
        }
    }
}
}