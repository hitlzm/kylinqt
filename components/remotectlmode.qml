import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import "./"
import QtQuick.Layouts 1.12

Item {
    id: root

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 10

        Indicator {
            label: "手柄连接状态"
            normal: false
            // 设置normal变量来显示手柄是否连接
        }

        DataLabel {
            id: x
            label: "内框角度输出:"
            value: "远程控制模式"
            fontSize: 20
        }
        DataLabel {
            id: y
            label: "中轴角度输出:"
            value: "远程控制模式"
            fontSize: 20
        }
        DataLabel {
            id: z
            label: "外框角度输出:"
            value: "远程控制模式"
            fontSize: 20
        }
    }
}
