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
    //增加对编辑完成信号的支持
    signal editingFinished()
    property alias title: titleText.text
    property alias text: textField.text
    property alias placeholderText: textField.placeholderText
    property alias labeltext: mylabel.text

    // 禁用时：文本框背景变灰，清空内容
    onEnabledChanged: {
        if (!enabled) {
            textField.text = ""
        }
    }

    Text {
        id: titleText
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: 14
        font.bold: true
        color: root.enabled ? "#c0c0c0" : "#606060"
        }
   RowLayout {
    id: rowLayout
    width: parent.width
    spacing: 8
    anchors.top: titleText.bottom
    anchors.topMargin: 2

    Item {
        Layout.preferredWidth: mywidth - mylabel.width - rowLayout.spacing
        Layout.preferredHeight: myheight - titleText.height - rowLayout.spacing

        CusTextField {
            id: textField
            anchors.fill: parent
            font.pixelSize: 16
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            onEditingFinished: root.editingFinished()
        }

        // 灰色覆盖层，现在锚定到父项（即 Item）
        Rectangle {
            anchors.fill: parent
            color: "#3a3f4a"
            visible: !root.enabled
            z: 10
        }
    }
    Label {
        id: mylabel
        Layout.preferredWidth: 10
        text: labeltext
        font.bold: true
        font.pixelSize: textField.font.pixelSize
        color: root.enabled ? '#c0c0c0' : "#606060"
        Layout.alignment: Qt.AlignVCenter
    }

   }

    
}
