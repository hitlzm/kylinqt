import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
Row {
    spacing: 2
    property string label: ""
    property string value: ""
    property color valueColor: "#1a73e8"
    property int fontSize: 18

    Label {
        text: parent.label
        font.pixelSize: parent.fontSize
        width: 150
    }
    Text {
        text: parent.value
        font.pixelSize: parent.fontSize
        font.bold: true
        color: parent.valueColor
    }
}
