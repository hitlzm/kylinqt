import QtQuick 2.12

// 图像导引头接收区（数据显示已全部移至发送区，仅保留背景色方框）
Rectangle {
    id: root
    width: 1200
    height: 100
    color: '#e9f0f9'

    // 背景色方框
    Rectangle {
        anchors.fill: parent
        anchors.margins: 10
        color: '#e1d8d8'
    }
}
