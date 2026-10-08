import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0

/*!
    启动登录门禁页。

    校验通过前主界面（含视频播放器、串口相关界面）完全不实例化，由 main.qml 里的
    Loader 控制；校验逻辑在 C++ 的 AuthManager 中，本页只负责输入与提示。
    连续失败次数用尽后会进入锁定倒计时，期间无法提交。
*/
Rectangle {
    id: root
    color: "#0f2233"

    // 提交一次校验。失败原因由 AuthManager::verifyFailed 回填到 hint，
    // 这里不直接判断返回值，避免与信号两条路径重复处理。
    function submit() {
        if (auth.lockSecondsLeft > 0)
            return
        hint.text = ""
        auth.verify(keyField.text)
        keyField.text = ""
        keyField.forceActiveFocus()
    }

    Component.onCompleted: keyField.forceActiveFocus()

    Rectangle {
        id: card
        width: 480
        height: 340
        anchors.centerIn: parent
        radius: 12
        color: "#ffffff"
        border.width: 1
        border.color: "#c8d0d8"

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 40
            anchors.rightMargin: 40
            spacing: 14

            Text {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("kylin-qt")
                font.pixelSize: 26
                font.bold: true
                color: "#10233a"
            }

            Text {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("请输入密钥后进入使用界面")
                font.pixelSize: 15
                color: "#5a6673"
            }

            CusTextField {
                id: keyField
                width: parent.width
                height: 42
                font.pixelSize: 18
                // 覆盖 CusConfig.maximumLength(64)，避免长口令被静默截断
                maximumLength: 128
                echoMode: TextInput.Password
                placeholderText: qsTr("密钥")
                onAccepted: root.submit()
            }

            Text {
                id: hint
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 14
                color: auth.lockSecondsLeft > 0 ? "#e07b00" : "#d32f2f"
                elide: Text.ElideRight
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 16

                CusButton_Blue {
                    width: 130
                    height: 40
                    font.pixelSize: 16
                    text: qsTr("进入")
                    enabled: auth.lockSecondsLeft === 0
                    onClicked: root.submit()
                }

                CusButton_White {
                    width: 110
                    height: 40
                    font.pixelSize: 16
                    text: qsTr("退出")
                    onClicked: auth.giveUp()
                }
            }
        }
    }

    Connections {
        target: auth
        onVerifyFailed: hint.text = reason
        onLockSecondsLeftChanged: {
            if (auth.lockSecondsLeft > 0) {
                hint.text = qsTr("尝试次数已用尽，请等待 %1 秒").arg(auth.lockSecondsLeft)
            } else {
                hint.text = qsTr("可以重新输入密钥（还可尝试 %1 次）").arg(auth.attemptsLeft)
            }
        }
    }
}
