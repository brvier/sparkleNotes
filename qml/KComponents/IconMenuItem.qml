import QtQuick 2.0

import 'theme.js' as Theme

ButtonArea {
    id: iconMenuItem

    property alias text: label.text
    property color color: Theme.colors.secondaryHighlight
    property color _real_color: enabled ? color : Theme.colors.placeholder
    property alias icon: icon.icon
    property alias size: icon.size
    property bool alwaysShowText: false

    Behavior on _real_color { ColorAnimation { duration: 100 } }

    transparent: true
    canHighlight: false

    height: 80 * root.scaleFactor
    width: height

    Icon {
        id: icon
        anchors.centerIn: parent
        opacity: iconMenuItem.enabled ? 1 : .2
        color: label.color
    }

    Label {
        id: label
        font.pixelSize: 15 * root.scaleFactor
        visible: parent.pressed || parent.alwaysShowText
        color: parent.pressed ? Qt.darker(iconMenuItem._real_color, 1.1) : iconMenuItem._real_color

        anchors {
            bottom: icon.top
            horizontalCenter: icon.horizontalCenter
            margins: 5 * root.scaleFactor
        }
    }
}
