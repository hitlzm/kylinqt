import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import "./"
import QtQuick.Layouts 1.12
import QtGamepad 1.0

Item {
    id: root
    anchors.centerIn:parent
   
    Gamepad {
        id: myGamepad
        deviceId: 0
        onAxisLeftXChanged: handle.axisLeftX = axisLeftX
        onAxisLeftYChanged: handle.axisLeftY = axisLeftY
        onAxisRightXChanged: handle.axisRightX = axisRightX
        onButtonL2Changed: handle.buttonL2 = buttonL2
        onButtonR2Changed: handle.buttonR2 = buttonR2
        onButtonAChanged: handle.buttonA = buttonA   // 或者 = myGamepad.buttonA
        // onButtonAReleased: handle.buttonA = false // 松开时复位
        onButtonBChanged: handle.buttonB = buttonB
        // onButtonBReleased: handle.buttonB = false
    }
    
    ColumnLayout {
        id:firstcolumn
        anchors.left:root.left
        anchors.leftMargin: 20
        spacing: 12

        // ──────────────── 连接状态 ────────────────
        Indicator {
            label: "手柄连接状态"
            normal: myGamepad.connected
        }

        // ──────────────── 左摇杆 ────────────────
        Rectangle {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 2
            color: "#cccccc"
        }

        Text {
            text: "左摇杆 (Left Stick)"
            font.pixelSize: 16
            font.bold: true
            color: "#555555"
        }

        DataLabel {
            label: "X 轴:"
            value: myGamepad.axisLeftX.toFixed(3)
            fontSize: 20
        }

        DataLabel {
            label: "Y 轴:"
            value: myGamepad.axisLeftY.toFixed(3)
            fontSize: 20
        }

        // ──────────────── 右摇杆 ────────────────
        Rectangle {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 2
            color: "#cccccc"
        }

        Text {
            text: "右摇杆 (Right Stick)"
            font.pixelSize: 16
            font.bold: true
            color: "#555555"
        }

        DataLabel {
            label: "X 轴:"
            value: myGamepad.axisRightX.toFixed(3)
            fontSize: 20
        }

        DataLabel {
            label: "Y 轴:"
            value: myGamepad.axisRightY.toFixed(3)
            fontSize: 20
        }
}
ColumnLayout {
        id:secondcolumn
        anchors.left:firstcolumn.right
        anchors.leftMargin: 20
        anchors.top: firstcolumn.top
        anchors.topMargin:50
        spacing: 12
        // ──────────────── 扳机 ────────────────
        Rectangle {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 2
            color: "#cccccc"
        }

        Text {
            text: "扳机 (Triggers)"
            font.pixelSize: 16
            font.bold: true
            color: "#555555"
        }

        DataLabel {
            label: "L2:"
            value: myGamepad.buttonL2.toFixed(3)
            fontSize: 20
        }

        DataLabel {
            label: "R2:"
            value: myGamepad.buttonR2.toFixed(3)
            fontSize: 20
        }

        // ──────────────── 速度输出映射 ────────────────
        Rectangle {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 2
            color: "#cccccc"
        }

        Text {
            text: "速度输出映射"
            font.pixelSize: 16
            font.bold: true
            color: "#e65100"
        }

        DataLabel {
            id: x
            label: "内框速度输出:"
            value: (myGamepad.axisLeftY * 100).toFixed(0) + " %"
            fontSize: 20
        }
        DataLabel {
            id: y
            label: "中框速度输出:"
            value: (myGamepad.axisLeftX * 100).toFixed(0) + " %"
            fontSize: 20
        }
        DataLabel {
            id: z
            label: "外框速度输出:"
            value: (myGamepad.axisRightX * 100).toFixed(0) + " %"
            fontSize: 20
        }
    }
    ColumnLayout{
        id:thirdcolumn
        anchors.left:secondcolumn.right
        anchors.leftMargin: 20
        anchors.top: firstcolumn.top
        anchors.topMargin:50
        spacing: 12
        Rectangle {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 2
            color: "#cccccc"
        }
        Text {
            text: "按键"
            font.pixelSize: 16
            font.bold: true
            color: "#555555"
        }
        DataLabel {
            label: "A:"
            value: myGamepad.buttonA
            fontSize: 20
        }

        DataLabel {
            label: "B:"
            value: myGamepad.buttonB
            fontSize: 20
        }
    }
}
