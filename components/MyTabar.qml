import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
Item{
    property int mywidth: 1200
    width: mywidth
TabBar {
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("图像导引头")
        }
        TabButton {
            text: qsTr("激光导引头")
        }
    }

    StackLayout {
        width: parent.width
        currentIndex: bar.currentIndex
        Item {
            id: laser
        }
        Item {
            id: image
        }
    }
}