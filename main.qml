import QtQuick 2.12
import QtQuick.Window 2.12
import "./components"
import taoQuick 1.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
Window {
    id: root
    visible: true
    width: Screen.width
    height: Screen.height
    title: qsTr("Hello World")
    
    
    // LaserSendArea{
    //     x:0
    //     y:0
    // }
    // Test{
    //     id:sendArea 
    //     x:0
    //     y:0
    // }
    LaserRecvArea{
        // anchors.top: sendArea.bottom    
        x:0
        y:0
    }
    // Myvideo{
    //     anchors.right: parent.right
    //     anchors.top: parent.top
    // }
}