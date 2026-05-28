import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
Item {
    id: root
    property alias title: titleText.text
    property int mywidth
    property int myheight
    property alias model: myCombox.model
    width:mywidth
    height:myheight
    // property alias labeltext: mylabel.text
    Text {
    id: titleText
    text: title 
    anchors.left: parent.left
    anchors.top: parent.top   // 替代 anchors.top
    anchors.topMargin: 2
    font.pixelSize: 14
    font.bold: true
    color: "black"
    }
    
    CusComboBox{
        id: myCombox
        anchors.left: parent.left
        anchors.top: titleText.bottom
        width: mywidth
        height: myheight-titleText.height
    }
    
}