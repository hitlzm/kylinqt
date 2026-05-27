import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
Item {
    id: root
    property int mywidth
    property int myheight
    width: mywidth
    height: myheight

    property alias title: titleText.text
    property alias text: textField.text
    property alias placeholderText: textField.placeholderText
    // property alias labeltext: mylabel.text
    Text {
        id: titleText
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: 14
        font.bold: true
        color: "#333333"
    }
    

    CusTextField {
        id: textField
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleText.bottom
        anchors.topMargin: 4
        height: 40
        font.pixelSize: 16
        // 水平居中
        horizontalAlignment: TextInput.AlignHCenter
    
        // 垂直居中：设置垂直居中对齐和相同的上下内边距
        verticalAlignment: TextInput.AlignVCenter
    }
    
}
