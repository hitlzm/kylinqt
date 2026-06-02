import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import taoQuick 1.0
import "./"

Rectangle {
    id: root
    property int mywidth: 600
    property int myheight: 800
    width: mywidth
    height: myheight
    color: '#e9f0f9'
    property int groupHeight1: 150

    RowLayout {
        id: ctrlrow
        anchors.left: parent.left;    anchors.leftMargin: 10
        anchors.right: parent.right;  anchors.rightMargin: 10
        anchors.top: parent.top;      anchors.topMargin: 10
        spacing: 20

        Text { text: "转台控制区"; font.pixelSize: 24; font.bold: true; color: "#000000"; Layout.alignment: Qt.AlignVCenter }

        CusButton_Blue { text: "开机"; font.pixelSize: 18; Layout.fillWidth: true; height: 60 }
        CusButton_Blue { text: "停机"; font.pixelSize: 18; Layout.fillWidth: true; height: 60 }
        CusButton_Blue { text: "回零"; font.pixelSize: 18; Layout.fillWidth: true; height: 60 }
        CusButton_Blue { text: "复位"; font.pixelSize: 18; Layout.fillWidth: true; height: 60 }
    }

    GroupBox {
        id: statusGroup
        anchors.left: parent.left;    anchors.leftMargin: 10
        anchors.right: parent.right;  anchors.rightMargin: 10
        anchors.top: ctrlrow.bottom;  anchors.topMargin: 5

        background: Rectangle { color: "#e1d8d8"; border.color: "gray"; border.width: 4; radius: 8 }
        title: "转台状态信息"; font.pixelSize: 18
        label: Label { text: parent.title; font.pixelSize: 18; leftPadding: 12; topPadding: 6 }

        Column { spacing: 10; anchors.centerIn: parent
            Row { spacing: 6
                DataLabel { label: "秒时间:"; value: "0x" }
                DataLabel { label: "序号:";   value: "0x" }
            }
            Row { spacing: 6
                DataLabel {
                    label: "内框状态:"
                    value: { switch(laserSerial.laserData.dytStatus) {
                        case 0x01: return "伺服"; case 0x02: return "回零执行中"; case 0x03: return "位置执行中"
                        case 0x04: return "速率执行中"; case 0x05: return "速率稳定"; case 0x06: return "摇摆执行中"
                        case 0x07: return "摇摆稳定"; case 0x08: return "停车执行中"; case 0x09: return "跟踪模式1执行中"
                        case 0x0A: return "停止跟踪"; case 0x0B: return "跟踪模式2执行中"; case 0x0F: return "速度环模式执行中"
                        case 0x1F: return "驱动器报警"; case 0x20: return "伺服超差报警"; case 0x21: return "正向限位报警"
                        case 0x22: return "逆向限位报警"; case 0x23: return "时钟同步报警"; case 0x24: return "初始化信息报警"
                        case 0x25: return "限位开关同时导通"; case 0x26: return "编码器数据故障报警"
                        case 0x29: return "瞬态电流报警"; case 0x2A: return "连续电流报警"
                        default: return "未知"
                    }}
                }
                DataLabel { label: "内框角度值:"; value: "0x" }
                DataLabel { label: "内框控制偏差:"; value: "0x" }
            }
            Row { spacing: 6
                DataLabel { label: "中框状态:"; value: "未知" }
                DataLabel { label: "中框角度值:"; value: "0x" }
                DataLabel { label: "中框控制偏差:"; value: "0x" }
            }
            Row { spacing: 6
                DataLabel { label: "外框状态:"; value: "未知" }
                DataLabel { label: "外框角度值:"; value: "0x" }
                DataLabel { label: "外框控制偏差:"; value: "0x" }
            }
        }
    }

    ProgramMode {
        id: programMode
        anchors.left: parent.left;    anchors.leftMargin: 10
        anchors.right: parent.right;  anchors.rightMargin: 10
        anchors.top: statusGroup.bottom; anchors.topMargin: 5
    }
}
