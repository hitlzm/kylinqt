import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    height: 24
    width: rowLayout.implicitWidth

    property bool normal: true
    property string label: ""
    property int fontSize: 18

    Row {
        id: rowLayout
        spacing: 6
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            width: 10; height: 10; radius: 5
            color: root.normal ? "#4CAF50" : "#F44336"
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            text: root.label
            font.pixelSize: root.fontSize
            color: root.normal ? "#333333" : "#d93025"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
