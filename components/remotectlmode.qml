import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import "./"
import QtQuick.Layouts 1.12
import QtGamepad 1.0

Item {
    id: root
    anchors.fill: parent
   
    Gamepad {
        id: myGamepad
        deviceId: GamepadManager.connectedGamepads.length > 0 ? GamepadManager.connectedGamepads[0] : -1
        // onAxisLeftXChanged: gamepadBridge.axisLeftXChange(axisLeftX)
        // onAxisLeftYChanged: gamepadBridge.axisLeftYChange(axisLeftY)
        // onAxisRightXChanged: gamepadBridge.axisRightXChange(axisRightX)
        // onButtonL2Changed: gamepadBridge.buttonL2Change(buttonL2)
        // onButtonR2Changed: gamepadBridge.buttonR2Change(buttonR2)
        onButtonAChanged: gamepadBridge.buttonAChange(buttonA)   // 或者 = myGamepad.buttonA
        onButtonBChanged: gamepadBridge.buttonBChange(buttonB)
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

        // ──────────────── 运行模式指示 ──────────────── ，当没有扳机按下时为速度模式，有扳机按下时为位置模式
        DataLabel {
            label: "当前运行模式:"
            value: myGamepad.buttonL2 > 0.5 || myGamepad.buttonR2 > 0.5 ? "位置模式" : "速度模式"
            fontSize: 20
        }
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
            value: (myGamepad.axisRightX * 100).toFixed(0) + " %"
            fontSize: 20
        }
        DataLabel {
            id: y
            label: "中框速度输出:"
            value: (myGamepad.axisLeftY * 100).toFixed(0) + " %"
            fontSize: 20
        }
        DataLabel {
            id: z
            label: "外框速度输出:"
            value: (myGamepad.axisLeftX * 100).toFixed(0) + " %"
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
        //分界线
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
        Rectangle {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 2
            color: "#cccccc"
        }
        // 步进角与速度模式速度倍率设置，加两个DoubleSpinBox
        DoubleSpinBox {
            id: stepAngleSpinBox
            label: "步进角度 (°):"
            value: 1.0
            from: 0.1
            to: 5
            stepSize: 0.1
            decimals: 1
            fontSize: 20
        }
        DoubleSpinBox {
            id: speedMultiplierSpinBox
            label: "速度倍率:"
            value: 1.0
            from: 0
            to: 1
            stepSize: 0.05
            decimals: 1
            fontSize: 20
        }
    }

//以固定周期更新手柄数据
Timer {
    interval: 100        // 50Hz
    repeat: true
    running: true

    onTriggered: {

        gamepadBridge.updateGamepad(
                    myGamepad.axisLeftX,
                    myGamepad.axisLeftY,
                    myGamepad.axisRightX,
                    myGamepad.buttonL2,
                    myGamepad.buttonR2)
    }
}
}
