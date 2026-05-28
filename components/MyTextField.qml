import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
Item {
    id: root
    property int mywidth
    property int myheight
    property int textfieldheight
    width: mywidth
    height: myheight

    property alias title: titleText.text
    property alias text: textField.text
    property alias placeholderText: textField.placeholderText
    property alias labeltext: mylabel.text
    Text {
        id: titleText
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: 14
        font.bold: true
        color: "#333333"
        }
   RowLayout {
    id: rowLayout
    width: parent.width
    spacing: 8
    anchors.top: titleText.bottom
    anchors.topMargin: 2
    
    CusTextField {
        id: textField
        // Layout.fillWidth: true
        Layout.preferredHeight: myheight - titleText.height - rowLayout.spacing
        Layout.preferredWidth: mywidth - mylabel.width - rowLayout.spacing
        font.pixelSize: 16
        // 水平居中
        horizontalAlignment: TextInput.AlignHCenter
        // 垂直居中：设置垂直居中对齐和相同的上下内边距
        verticalAlignment: TextInput.AlignVCenter
    }
    Label {
        id: mylabel
        Layout.preferredWidth: 10
        text: labeltext
        font.bold: true
        font.pixelSize: textField.font.pixelSize
        color: '#060000'
        Layout.alignment: Qt.AlignVCenter
    }

   }
}
