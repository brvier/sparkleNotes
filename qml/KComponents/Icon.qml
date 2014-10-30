

import QtQuick 2.2

import 'theme.js' as Theme
import 'icons/icons.js' as Icons


Label {
    id: picon

    property int size: 48
    property string icon

    text: icon
    font.pixelSize: size * root.scaleFactor
    font.family: Icons.font
}

