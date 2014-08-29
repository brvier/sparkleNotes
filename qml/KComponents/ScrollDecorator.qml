import QtQuick 2.0

import 'theme.js' as Theme

Rectangle {
    property var flickableItem

    x: flickableItem.width - width
    y: flickableItem.visibleArea.yPosition * flickableItem.height
    width: 5 * root.scaleFactor
    height: flickableItem.visibleArea.heightRatio * flickableItem.height
    //height:  (flickable.visibleArea.heightRatio > 1) ? (parent.height) :(flickable.visibleArea.heightRatio*parent.height)
    visible: flickableItem.visibleArea.heightRatio < 1
    color: Theme.colors.background
    opacity: flickableItem.moving ? 1 : 0
    Behavior on opacity { PropertyAnimation { duration: 100 } }
}
