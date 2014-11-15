import QtQuick 2.0

import 'theme.js' as Theme
import 'icons/icons.js' as Icons

Item {
    id: textField

    property alias text: textInput.text
    property string placeholderText: ''
    signal accepted

    function paste() {
        textInput.paste();
    }

    height: 50 * root.scaleFactor

    Rectangle {
        anchors.fill: parent
        color: '#ddd'
    }

    TextInput {
        id: textInput
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: clipboardIcon.left
            margins: 5 * root.scaleFactor
        }
        //color: Theme.colors.text
        //selectionColor: Theme.colors.background

        font.pixelSize: parent.height * 0.6
        font.family: placeholder.font.family
        focus: true
        onAccepted: textField.accepted()
        inputMethodHints: TextInput.Normal
        onTextChanged: {
            console.log(textInput.text)
        }

    }

    Label {
        id: placeholder
        anchors.fill: textInput
        visible: (textInput.text == '')
        text: textField.placeholderText
        color: Theme.colors.placeholder
        font.pixelSize: textInput.font.pixelSize
    }

    Icon {
        id: clipboardIcon

        anchors {
            right: parent.right
            margins: 5 * root.scaleFactor
            verticalCenter: parent.verticalCenter
        }
        color: Theme.colors.secondaryHighlight
        icon: Icons.paperclip
        size: textInput.font.pixelSize - (10 * root.scaleFactor)

        MouseArea {
            anchors.fill: parent

        onClicked: {
            root.showSelection([
                {
                    label: 'Copy',
                    callback: function () {
                        textInput.copy();
                    }
                },
                {
                    label: 'Paste',
                    callback: function() {
                        textInput.paste();
                    }
                },
                {
                    label: 'Cut',
                    callback: function() {
                        textInput.cut();
                    }
                },
                {
                    label: 'Clear',
                    callback: function() {
                        textInput.text = '';
                    }
                },
                {
                    label: 'Select all',
                    callback: function() {
                        textInput.selectAll();
                    }
                }
            ]);
        }

        }
    }
}
