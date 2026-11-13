import QtQuick 2.9
import QtQuick.Controls 2.2

CusButton {
    // 蓝底白字原主题（保留供切换和查阅）
    // backgroundColorNormal: "#38a9e4"
    // backgroundColorHovered: "#47c0ff"
    // backgroundColorPressed: "#108bcd"
    // backgroundColorDisable: "#b6bdc5"

    // borderColor: "#579ee5"
    // textColor: "#ffffff"

    // 主题开关：false 为黑底白字，true 为蓝底白字
    readonly property bool useBlueTheme: false

    backgroundColorNormal: useBlueTheme ? "#38a9e4" : "#000000"
    backgroundColorHovered: useBlueTheme ? "#47c0ff" : "#333333"
    backgroundColorPressed: useBlueTheme ? "#108bcd" : "#111111"
    backgroundColorDisable: useBlueTheme ? "#b6bdc5" : "#666666"

    borderColor: useBlueTheme ? "#579ee5" : backgroundColor
    textColor: "#ffffff"
}
