import QtQuick 2.2

import 'theme.js' as Theme
import 'icons/icons.js' as Icons

Dialog {
    id: confirmation

    property alias title: header.title
    property alias description: description.text
    property alias affirmativeAction: affirmativeButton.text
    property alias negativeAction: negativeButton.text

    property string icon
    property alias color: header.color
    property var callback: undefined

    contentHeight: contentColumn.height


    Column {
        id: contentColumn
        width: parent.width

        PageHeader {
            id: header
            bgColor: Theme.colors.dialogHeader
            color: Theme.colors.dialogHeaderText
            title: 'Confirmation'
        }

        Item { width: parent.width; height: 10 * root.scaleFactor }

        Label {
            id: description
            width: parent.width - 30 * root.scaleFactor
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: Text.WordWrap
            color: Theme.colors.dialogText
            visible: text
        }

        Item { width: parent.width; height: 10 * root.scaleFactor }

        Row {
            width: parent.width - 30 * root.scaleFactor
            height: 80 * root.scaleFactor
            spacing: 30 * root.scaleFactor
            anchors.horizontalCenter: parent.horizontalCenter

            ConfirmationButton {
                id: affirmativeButton

                width: (parent.width - parent.spacing) * 2 / 3
                height: parent.height

                text: 'Yes'
                icon: confirmation.icon
                contentColor: header.color

                onClicked: {
                    if (confirmation.callback !== undefined) {
                        confirmation.callback();
                        confirmation.closePage();
                    }
                }
            }

            ConfirmationButton {
                id: negativeButton

                width: (parent.width - parent.spacing) * 1 / 3
                height: parent.height

                text: 'Cancel'
                contentColor: Theme.colors.placeholder

                onClicked: {
                    confirmation.closePage();
                }
            }
        }

        Item { width: parent.width; height: 20 * root.scaleFactor }
    }
}
