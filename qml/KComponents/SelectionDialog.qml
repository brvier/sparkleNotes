import QtQuick 2.0

import 'theme.js' as Theme
import 'icons/icons.js' as Icons

Dialog {
    id: selectionDialog

    property string title: ''
    property var callback: undefined
    property var items: ([])
    property var selectedIndex: -1
    bgColor: Theme.colors.selectionDialog
    contentHeight: selectionDialogFlickable.contentHeight

    Flickable {
        id: selectionDialogFlickable

        boundsBehavior: Flickable.StopAtBounds

        anchors.fill: parent
        contentHeight: contentColumn.height

        Column {
            id: contentColumn
            width: parent.width

            PageHeader {
                id: header
                visible: title != ''
                color: Theme.colors.selectionDialogHighlight
                title: selectionDialog.title
            }

            Repeater {
                model: selectionDialog.items

                delegate: ButtonArea {
                    id: buttonArea

                    color: Theme.colors.selectionDialog
                    width: parent.width
                    height: 70 * root.scaleFactor

                    transparent: (index != selectionDialog.selectedIndex)

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            margins: 20 * root.scaleFactor
                        }

                        text: modelData
                        color: (index == selectionDialog.selectedIndex || buttonArea.pressed) ? Theme.colors.selectionDialogHighlight : Theme.colors.selectionDialogText
                        font.pixelSize: 30 * root.scaleFactor
                        elide: Text.ElideRight
                    }

                    onClicked: {
                        if (selectionDialog.callback !== undefined) {
                            selectionDialog.callback(index, modelData);
                        }
                        selectionDialog.closePage();
                    }
                }
            }
        }
    }

    ScrollDecorator { flickableItem: selectionDialogFlickable }
}
