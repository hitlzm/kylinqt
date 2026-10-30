.pragma library

function isDigit(character) {
    return character >= "0" && character <= "9"
}

// Empty text is valid and is interpreted as zero by validateNumber().
function isAllowedNumberPrefix(text, allowNegative, allowDecimal) {
    var value = String(text)
    if (value === "")
        return true

    var start = 0
    if (value.charAt(0) === "-") {
        if (!allowNegative)
            return false
        start = 1
        if (value.length === 1)
            return true
    }

    // A decimal point cannot be the first numeric character, so ".5" is rejected.
    if (!isDigit(value.charAt(start)))
        return false

    var dotSeen = false
    for (var i = start + 1; i < value.length; ++i) {
        var character = value.charAt(i)
        if (isDigit(character))
            continue
        if (character === "." && allowDecimal && !dotSeen) {
            dotSeen = true
            continue
        }
        return false
    }
    return true
}

function isCompleteNumber(text, allowNegative, allowDecimal) {
    var value = String(text)
    if (value === "" || !isAllowedNumberPrefix(value, allowNegative, allowDecimal))
        return false

    var normalized = value.replace(/^-/, "")
    if (normalized.charAt(normalized.length - 1) === ".")
        return false
    if (!allowDecimal)
        return normalized.indexOf(".") === -1
    return normalized.indexOf(".") !== -1 || /^\d+$/.test(normalized)
}

function fractionalDigits(text) {
    var value = String(text)
    var dotIndex = value.indexOf(".")
    return dotIndex < 0 ? 0 : value.length - dotIndex - 1
}

function validateNumber(text, allowNegative, allowDecimal, maximumFractionDigits) {
    var value = String(text)
    if (value === "")
        return { state: "valid", value: 0, message: "" }

    if (!isAllowedNumberPrefix(value, allowNegative, allowDecimal))
        return { state: "invalidCharacters", value: NaN, message: "" }

    if (!isCompleteNumber(value, allowNegative, allowDecimal))
        return { state: "incomplete", value: NaN, message: "" }

    if (maximumFractionDigits >= 0
            && fractionalDigits(value) > maximumFractionDigits)
        return { state: "tooManyDecimals", value: NaN, message: "" }

    return { state: "valid", value: Number(value), message: "" }
}

function errorMessage(fieldLabel, result, allowNegative, allowDecimal,
                      maximumFractionDigits) {
    var characters = allowDecimal ? "半角数字、一个小数点" : "半角整数"
    if (allowNegative)
        characters += "和位于开头的一个负号"

    if (result.state === "invalidCharacters")
        return fieldLabel + "格式不正确：只能输入" + characters

    if (result.state === "incomplete")
        return fieldLabel + "不是完整数值：数字需以 0 开头，不支持 +、.5、科学计数法或单位"

    if (result.state === "tooManyDecimals")
        return fieldLabel + "最多允许 " + maximumFractionDigits + " 位小数"

    return ""
}
