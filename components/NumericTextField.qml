import QtQuick 2.12
import QtQuick.Controls 2.12
import taoQuick 1.0
import "InputRules.js" as InputRules

CusTextField {
    id: root

    property bool numericInput: true
    property bool allowNegative: true
    property bool allowDecimal: true
    property int maximumFractionDigits: 4
    property string fieldLabel
    property string lastReportedFailureState: ""

    function currentValidationResult() {
        return numericInput
            ? InputRules.validateNumber(text, allowNegative, allowDecimal,
                                        maximumFractionDigits)
            : { state: "valid", value: Number(text), message: "" }
    }

    readonly property var numericResult: currentValidationResult()
    readonly property bool inputFormatValid: numericResult.state === "valid"
    readonly property real numericValue: inputFormatValid ? numericResult.value : 0
    readonly property string numericError:
        inputFormatValid ? "" : InputRules.errorMessage(
            fieldLabel, numericResult, allowNegative, allowDecimal,
            maximumFractionDigits)

    signal validationFailed(string message)

    function reportValidationFailure(result) {
        if (result.state === "valid")
            return
        if (lastReportedFailureState === result.state)
            return
        lastReportedFailureState = result.state
        validationFailed(InputRules.errorMessage(
                             fieldLabel, result, allowNegative, allowDecimal,
                             maximumFractionDigits))
    }

    onTextChanged: {
        if (!numericInput)
            return
        var result = currentValidationResult()
        if (result.state === "invalidCharacters")
            reportValidationFailure(result)
        else
            lastReportedFailureState = ""
    }

    Connections {
        target: root
        onEditingFinished: {
            if (root.numericInput)
                root.reportValidationFailure(root.currentValidationResult())
        }
    }
}
