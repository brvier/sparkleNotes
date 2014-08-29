
import QtQuick 2.2

import 'theme.js' as Theme

MouseArea {
    id: mouseArea
    property bool transparent: false
    property bool canHighlight: true
    property alias color: background.color

    Rectangle {
        id: background
        anchors.fill: parent
        color: Theme.colors.area
        opacity: (mouseArea.pressed && mouseArea.canHighlight) ? 1 : .5
        visible: parent.enabled && ((mouseArea.canHighlight && mouseArea.pressed) || !mouseArea.transparent)
    }
}
