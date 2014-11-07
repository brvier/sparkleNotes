import QtQuick 2.0

import 'theme.js' as Theme

Dialog {
    id: textInputDialog

    property string buttonText
    property string placeholderText
    property string text
    property bool pasteOnLoad: false
    property var callback
    property var callbackOptions

    contentHeight: contentColumn.height

    Component.onCompleted: {
        if (pasteOnLoad) {
            input.paste();
        }
    }

    function accept() {
        console.log('TextInputDialo:'+input.text)
        textInputDialog.callback(input.text, callbackOptions);
        textInputDialog.closePage();
    }

    Column {
        id: contentColumn

        anchors.centerIn: parent
        spacing: 20 * root.scaleFactor

        TextField {
            id: input
            width: textInputDialog.width *.8
            placeholderText: textInputDialog.placeholderText
            text: textInputDialog.text
            onAccepted: textInputDialog.accept();
        }

        ButtonArea {
            id: button
            width: input.width
            height: input.height

            Label {
                anchors.centerIn: parent
                text: textInputDialog.buttonText
            }

            onClicked: textInputDialog.accept();
        }
    }
}
