import QtQuick 2.12
import QtQuick.Window 2.12
import "./components"

Window {
    id: root
    visible: true
    width: Screen.width
    height: Screen.height
    title: qsTr("Hello World")
    
    LaserRecvArea{
        x:100
        y:100
    }
}