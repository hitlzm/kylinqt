import QtQuick 2.12
import QtTest 1.2
import "../components" as Components

TestCase {
    name: "NumericInput"

    Components.NumericTextField {
        id: field
        fieldLabel: "测试值"
    }

    Components.MyTextField {
        id: wrappedField
        mywidth: 120
        myheight: 60
        title: "包装测试值"
        labeltext: "°"
        numericInput: true
        fieldLabel: "包装测试值"
    }

    SignalSpy {
        id: validationSpy
        target: wrappedField
        signalName: "validationFailed"
    }

    SignalSpy {
        id: numericValidationSpy
        target: field
        signalName: "validationFailed"
    }

    function test_number_states() {
        field.text = ""
        verify(field.inputFormatValid)
        compare(field.numericValue, 0)

        field.text = "-12.5"
        verify(field.inputFormatValid)
        compare(field.numericValue, -12.5)

        field.text = "1."
        compare(field.numericResult.state, "incomplete")
        verify(!field.inputFormatValid)

        field.text = ".5"
        compare(field.numericResult.state, "invalidCharacters")
        compare(numericValidationSpy.count, 1)

        field.text = "+1"
        compare(field.numericResult.state, "invalidCharacters")

        field.text = "1e2"
        compare(field.numericResult.state, "invalidCharacters")

        field.text = "0x10"
        compare(field.numericResult.state, "invalidCharacters")

        field.text = "1.12345"
        compare(field.numericResult.state, "tooManyDecimals")

        field.allowNegative = false
        field.text = "-1"
        compare(field.numericResult.state, "invalidCharacters")
    }

    function test_wrapped_text_property() {
        wrappedField.text = ""
        verify(wrappedField.inputFormatValid)
        compare(wrappedField.numericValue, 0)

        wrappedField.text = "20.5"
        verify(wrappedField.inputFormatValid)
        compare(wrappedField.numericValue, 20.5)

        wrappedField.text = "20°"
        verify(!wrappedField.inputFormatValid)
        compare(validationSpy.count, 1)

        wrappedField.text = "abc"
        compare(validationSpy.count, 1)
    }
}
