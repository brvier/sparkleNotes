import QtQuick 2.0
import 'theme.js' as Theme
import 'icons/icons.js' as Icons

Rectangle {
    id: toolbar
    property bool showing: true

    color: Theme.colors.toolbar

    height: 80 * root.scaleFactor
    MouseArea {
        // Capture all touch events
        anchors.fill: parent
    }

    anchors {
        left: parent.left
        right: parent.right
        bottom: parent.bottom
        bottomMargin: toolbar.showing ? 0 : -height
    }

    Behavior on anchors.bottomMargin { PropertyAnimation { duration: 100 } }
}
