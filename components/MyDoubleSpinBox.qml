import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0

/*!
    \qmltype MyDoubleSpinBox
    \brief 基于 TaoQuick 的 CusSpinBox 封装的小数微调框（支持最小 0.01 的步进）

    对外接口与 DoubleSpinBox 保持一致：label / value / from / to / stepSize / decimals / fontSize

    \qml
    MyDoubleSpinBox {
        label: "步进角度 (°):"
        value: 1.0
        from: 0.1
        to: 5
        stepSize: 0.1
        decimals: 1
        fontSize: 20
        onValueModified: console.log("value =", value)
    }
    \endqml

    实现说明：
    Qt5 的 SpinBox.value / stepSize 是 int（写 stepSize: 0.01 会直接报
    “Invalid property assignment: int expected”），无法直接表示小数。
    因此内部把数值按 10^decimals 放大成整数交给 CusSpinBox 运算，
    对外仍然是 double，这样既能支持 0.01（乃至 0.001）的步进，
    又不会出现 0.1 + 0.01 = 0.11000000000000001 之类的浮点误差。
*/
Item {
    id: root

    // ───────────────────────── 对外属性 ─────────────────────────
    property string label: ""
    property double value: 0.0
    property double from: 0.0
    property double to: 100.0
    //! 步进值，默认 0.01（decimals 为 2 时即为最小步进）
    property double stepSize: 0.01
    //! 小数位数（同时决定显示精度与步进精度）
    property int decimals: 2
    //! 标签与数值的字体大小
    property int fontSize: 16
    property bool labelBold: true
    property bool editable: true
    property bool wrap: false
    //! 标签与输入框之间的间距
    property int labelSpacing: 2
    //! 微调框宽度，0 表示按 fontSize 自适应
    property int controlWidth: 0
    //! 微调框高度，0 表示按 fontSize 自适应
    property int controlHeight: 0
    property color labelColor: "#333333"

    //! 当前显示文本（已按精度格式化）
    readonly property string displayText: spinBox.textFromValue(spinBox.value, Qt.locale())

    signal valueModified()
    signal editingFinished()

    // ───────────────────── 内部整数放大比例 ─────────────────────
    //! stepSize 自身的小数位数（例如 0.05 → 2）
    readonly property int _stepDecimals: _decimalsOf(stepSize)
    //! 实际精度取 decimals 与 stepSize 小数位数的较大者，避免 0.05 被当成 0.1
    readonly property int _precision: Math.max(0, Math.min(6, Math.max(decimals, _stepDecimals)))
    readonly property int _factor: Math.round(Math.pow(10, _precision))
    readonly property int _rawFrom: _toRaw(from)
    readonly property int _rawTo: _toRaw(to)
    //! 精度（factor）切换过程中为 true，用来屏蔽 Qt 侧因上下限瞬时变化产生的夹取
    property bool _settling: false
    //! true 表示当前内部值变化来自“外部 value 同步”，不算用户修改
    property bool _downSync: false

    // ───────────────────────── 内部函数 ─────────────────────────
    //! 取小数位数：String(0.01) → "0.01" → 2
    function _decimalsOf(v) {
        var s = String(v)
        var i = s.indexOf(".")
        return i < 0 ? 0 : s.length - i - 1
    }

    //! 小数 → 内部整数
    function _toRaw(v) {
        return Math.round(v * _factor)
    }

    //! 内部整数 → 小数
    function _toValue(raw) {
        return raw / _factor
    }

    //! 限幅到 [from, to]
    //! 注意：这里即时换算上下限，不直接读 _rawFrom / _rawTo 绑定，
    //! 否则在 decimals 变化的瞬间可能拿到尚未重新求值的旧边界
    function _clampRaw(raw) {
        var lo = _toRaw(from)
        var hi = _toRaw(to)
        return Math.max(Math.min(lo, hi), Math.min(Math.max(lo, hi), raw))
    }

    //! 解析用户输入的文本（空串 / 非法输入返回 NaN）
    function _parse(text) {
        var s = String(text).trim().replace(",", ".")
        if (s === "")
            return NaN
        var n = Number(s)
        return isNaN(n) ? NaN : n
    }

    //! 把内部数值刷新到输入框；用户正在输入等价写法（如 0.5 / 0.50）时不打断
    function _refreshDisplay(force) {
        if (!input)
            return
        var text = spinBox.textFromValue(spinBox.value, Qt.locale())
        if (input.text === text)
            return
        if (!force && input.activeFocus && _toRaw(_parse(input.text)) === spinBox.value)
            return
        input.text = text
    }

    //! 提交用户输入的文本
    function _commitText(text) {
        var n = _parse(text)
        if (isNaN(n))
            return          // 空串 / 非法输入：保持原值，失焦时再归一化显示
        var raw = _clampRaw(_toRaw(n))
        if (spinBox.value !== raw)
            spinBox.value = raw
        else
            _refreshDisplay(false)
    }

    //! 对外 value → 内部整数
    function _applyValue() {
        if (!spinBox || _settling)
            return
        var raw = _toRaw(value)
        var clamped = _clampRaw(raw)
        _downSync = true
        if (spinBox.value !== clamped)
            spinBox.value = clamped
        _downSync = false
        // 只有超出 [from, to] 时才把 value 纠正回范围内；
        // 单纯的四舍五入误差不回写，避免打断外部（如 slider）对 value 的绑定
        if (clamped !== raw) {
            var actual = _toValue(spinBox.value)
            if (Math.abs(value - actual) > 1e-9)
                value = actual
        }
    }

    //! 精度变化后按新精度重新换算内部值并刷新显示
    function _rescale() {
        _settling = false
        if (!spinBox)
            return
        var raw = _clampRaw(_toRaw(value))
        _downSync = true
        if (spinBox.value !== raw)
            spinBox.value = raw
        _downSync = false
        // 精度变化是用户主动行为，这里把 value 按新精度归一化
        value = _toValue(spinBox.value)
        _refreshDisplay(true)
    }

    function increase() {
        spinBox.increase()
    }
    function decrease() {
        spinBox.decrease()
    }

    readonly property real _controlWidth: controlWidth > 0 ? controlWidth : Math.max(80, Math.round(fontSize * 4.2))
    readonly property real _controlHeight: controlHeight > 0 ? controlHeight : Math.max(CusConfig.fixedHeight, fontSize + 14)

    onValueChanged: _applyValue()
    // 精度（decimals / stepSize）变化后需要重新换算内部整数：
    // 先屏蔽切换过程中的瞬时变化，等 from / to 等绑定求值完再换算
    on_FactorChanged: {
        _settling = true
        Qt.callLater(_rescale)
    }
    Component.onCompleted: {
        _applyValue()
        _refreshDisplay(true)
    }

    // ───────────────────────── 布局 ─────────────────────────
    // 标签在上、微调框在下；implicitWidth / implicitHeight 覆盖整个组件，
    // 这样才能在 ColumnLayout 里直接使用
    implicitWidth: Math.max(_controlWidth, labelText.implicitWidth)
    implicitHeight: (label !== "" ? labelText.height + labelSpacing : 0) + _controlHeight

    Text {
        id: labelText
        text: root.label
        visible: root.label !== ""
        anchors.left: parent.left
        anchors.top: parent.top
        font.pixelSize: root.fontSize
        font.bold: root.labelBold
        color: root.enabled ? root.labelColor : "#BBBBBB"
    }

    CusSpinBox {
        id: spinBox
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: root.label !== "" ? labelText.bottom : root.top
        anchors.topMargin: root.label !== "" ? root.labelSpacing : 0
        height: root._controlHeight

        // 内部一律使用“放大后的整数”，从根本上避开浮点误差
        from: root._rawFrom
        to: root._rawTo
        stepSize: Math.max(1, root._toRaw(root.stepSize))
        // 注意：不要覆盖 CusSpinBox 的 contentItem！
        // 覆盖会让 CusSpinBox.qml 内部那个 contentItem 在同一次对象创建过程中
        // 先被创建再被销毁，而 CusSpinBox.qml 里的 Binding 仍引用着它，
        // 一旦该对象是在异步加载（Loader.asynchronous）的孵化过程中创建，
        // Qt 就会中止孵化并报 “Object or context destroyed during incubation”。
        // 所以这里保留它自带的输入框（只读 + 隐藏），数值只由本组件驱动。
        editable: false
        wrap: root.wrap

        Component.onCompleted: {
            if (contentItem)
                contentItem.visible = false
        }

        // 内部整数 → 显示文本
        textFromValue: function (value, locale) {
            return (value / root._factor).toFixed(root._precision)
        }
        // 显示文本 → 内部整数
        valueFromText: function (text, locale) {
            var n = root._parse(text)
            return isNaN(n) ? spinBox.value : root._clampRaw(root._toRaw(n))
        }
        validator: DoubleValidator {
            locale: "C"
            bottom: Math.min(root.from, root.to)
            top: Math.max(root.from, root.to)
            decimals: root._precision
            notation: DoubleValidator.StandardNotation
        }

        onValueChanged: {
            if (root._settling)
                return          // 精度切换中，等 _rescale 统一处理
            // 内部数值变化后刷新显示（用户正在输入时不会打断）
            root._refreshDisplay(false)
            if (root._downSync)
                return          // 由外部 value 同步进来的，不算用户修改
            // 用户点击上下箭头 / 输入文本 → 同步到对外 value
            var v = root._toValue(value)
            if (Math.abs(root.value - v) > 1e-9)
                root.value = v
            root.valueModified()
        }
    }

    // 自己叠在 CusSpinBox 上面的输入框：负责小数文本的显示与编辑
    CusTextInput {
        id: input
        z: 2
        anchors.left: spinBox.left
        anchors.leftMargin: spinBox.leftPadding
        anchors.verticalCenter: spinBox.verticalCenter
        width: spinBox.width - spinBox.up.indicator.width - spinBox.leftPadding - spinBox.rightPadding
        height: spinBox.height
        color: CusConfig.textColor
        font.pixelSize: root.fontSize
        selectionColor: CusConfig.controlColor_pressed
        selectedTextColor: CusConfig.textColor_pressed
        horizontalAlignment: Qt.AlignLeft
        verticalAlignment: Qt.AlignVCenter
        readOnly: !root.editable
        selectByMouse: true
        validator: spinBox.validator
        inputMethodHints: Qt.ImhFormattedNumbersOnly

        // 边输入边提交数值（显示文本保持不变，不打断输入）
        onTextEdited: root._commitText(text)
        // 回车或失焦后按精度归一化显示
        onEditingFinished: {
            root._commitText(text)
            root._refreshDisplay(true)
            root.editingFinished()
        }
    }
}
