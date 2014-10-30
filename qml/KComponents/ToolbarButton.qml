import QtQuick 2.0
import 'theme.js' as Theme
import 'icons/icons.js' as Icons

Rectangle {
    id: toolbarButton

    //property alias color: iconMenuItem.color
    //property alias text: iconMenuItem.text
    property string text: ''
    property alias icon: iconMenuItem.icon

    signal clicked()

    width: iconMenuItem.width
    height: iconMenuItem.height
    color: iconMenuItem.pressed ? Theme.colors.toolbarArea : 'transparent'

    Rectangle {
        height: 5 * root.scaleFactor
        color: iconMenuItem.pressed ? Theme.colors.secondaryHighlight : 'transparent'

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
    }

    IconMenuItem {
        id: iconMenuItem
        color: Theme.colors.toolbarText
        transparent: true
        enabled: parent.enabled
        onClicked: toolbarButton.clicked()        
    }
}
