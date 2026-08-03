import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
Row {
    spacing: 5
    property string label: ""
    property string value: ""
    property color valueColor: "#1a73e8"
    property int fontSize: 18
    property int labelWidth: 0   // 0 = 自适应宽度，>0 = 固定宽度
    property int valueWidth: 0   // 0 = 自适应宽度，>0 = 固定宽度
    property bool labelBold: false

    // 固定行高 = fontSize + 3（18px → 21px）。
    // 避免因系统字体行高不同（Win 宋体 21 / 雅黑 24 vs 麒麟 Noto ~27）导致列表行距不一致
    height: fontSize + 3

    Label {
        text: parent.label
        font.pixelSize: parent.fontSize
        font.bold: parent.labelBold
        width: parent.labelWidth > 0 ? parent.labelWidth : implicitWidth
        horizontalAlignment: parent.labelWidth > 0 ? Text.AlignRight : Text.AlignLeft
        height: parent.height
        verticalAlignment: Text.AlignVCenter
    }
    Text {
        text: parent.value
        font.pixelSize: parent.fontSize
        font.bold: true
        color: parent.valueColor
        width: parent.valueWidth > 0 ? parent.valueWidth : implicitWidth
        height: parent.height
        verticalAlignment: Text.AlignVCenter
    }
}
