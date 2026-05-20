import QtQuick 2.12
import QtQuick.Window 2.12
import "./components"
import taoQuick 1.0

Window {
    id: root
    visible: true
    width: Screen.width
    height: Screen.height
    title: qsTr("Hello World")
    
    // LaserRecvArea{
    //     x:100
    //     y:100
    // }
    LaserSendArea{
        x:100
        y:100
    }
    CusButton_Blue {
        x:800
        y:800
        width: 120
        height: 36
        text: "Hello"
        onClicked: {
            console.log("hello TaoQuick")
        }
    }
    
}