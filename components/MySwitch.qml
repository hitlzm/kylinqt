import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import QtQuick.Layouts 1.12
Item {
    id: root
    property alias title: titleText.text
    // property alias labeltext: mylabel.text
    Text {
    id: titleText
    text: title + ":"
    anchors.left: parent.left
    anchors.verticalCenter: parent.verticalCenter   // 替代 anchors.top
    font.pixelSize: 16
    font.bold: true
    color: 'black'
    }
    
    CusSwitch{
        id: mySwitch
        anchors.left: titleText.right
        anchors.verticalCenter: root.verticalCenter
        width: 60
        height: 30
    }
    
}
