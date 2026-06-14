import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import "./"
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    color: '#def0ea'

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 10

        Indicator {
            label: "手柄连接状态"
            normal: false
            // 设置normal变量来显示手柄是否连接
        }
        Row{
            spacing: 80
            DataLabel {
            id: x
            label: "内框角度输出:"
            value: "0"
            fontSize: 20
            }
            DataLabel {
            id: x1
            label: "内框速度输出:"
            value: "0"
            fontSize: 20
            }
        }
        Row{
            spacing: 80
             DataLabel {
            id: y
            label: "中轴角度输出:"
            value: "0"
            fontSize: 20
            }
             DataLabel {
            id: y1
            label: "中轴速度输出:"
            value: "0"
            fontSize: 20
            }
        }
        Row{

                spacing: 80
                DataLabel {
                id: z
                label: "外框角度输出:"
                value: "0"
                fontSize: 20
                }
                DataLabel {
                id: z1
                label: "外框速度输出:"
                value: "0"
                fontSize: 20
                }
        }
        
    }
}
