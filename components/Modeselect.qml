import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Item{
    id: root
    property int myindex: 1
    property bool syncingModeSelection: false
    property bool pendingExguideSelection: false

    function syncModeSelection() {
        syncingModeSelection = true
        radio1.checked = modeController.currentMode === 0
        radio2.checked = modeController.currentMode === 1
        radio3.checked = modeController.currentMode === 2
        syncingModeSelection = false

        if (modeController.currentMode >= 0 && modeController.currentMode <= 2)
            myindex = modeController.currentMode
    }

    function selectMode(mode) {
        if (mode !== 0)
            pendingExguideSelection = false

        // 允许先打开外引导配置页完成零位修正，但后端仍保持原模式。
        if (mode === 0 && !modeController.innerAngleZeroCorrected) {
            pendingExguideSelection = true
            myindex = 0
            modeController.setCurrentMode(0)
            return
        }

        if (modeController.setCurrentMode(mode)) {
            pendingExguideSelection = false
            myindex = mode
        } else {
            syncModeSelection()
        }
    }

    Connections {
        target: modeController
        onCurrentModeChanged: {
            if (modeController.currentMode !== 0)
                root.pendingExguideSelection = false
            root.syncModeSelection()
        }
        onInnerAngleZeroCorrectedChanged: {
            if (root.pendingExguideSelection
                    && modeController.currentMode !== 0) {
                root.pendingExguideSelection = false
                root.selectMode(0)
            }
        }
    }

    Component.onCompleted: root.syncModeSelection()

    GroupBox {
        background: Rectangle {
        color: "transparent"
        border.color: "gray"
        border.width: 4
        radius: 8
    }
    Row {
        spacing: 10   // 统一控制间距
        Text {
            id: text1
            text: qsTr("控制模式选择")
            font.pixelSize: 20
            font.bold: true
            color: 'black'
            anchors.verticalCenter: parent.verticalCenter
        }
    ButtonGroup { id: modeGroup }
    CusRadioButton {
        id: radio1
        width: 160
        text: qsTr("外引导模式")
        font.pixelSize: 20
        checked: false
        ButtonGroup.group: modeGroup
        onCheckedChanged: {
            if (checked && !root.syncingModeSelection)
                root.selectMode(0)
        }
    }
    CusRadioButton {
        id: radio2
        width: 160
        text: qsTr("程控模式")
        font.pixelSize: 20
        checked: false
        ButtonGroup.group: modeGroup
        onCheckedChanged: {
            if (checked && !root.syncingModeSelection)
                root.selectMode(1)
        }
    }
    CusRadioButton {
        id: radio3
        width: 160
        text: qsTr("遥控模式")
        font.pixelSize: 20
        checked: false
        ButtonGroup.group: modeGroup
        onCheckedChanged: {
            if (checked && !root.syncingModeSelection)
                root.selectMode(2)
        }
    }
}

}
}

