import QtQuick 2.2

import 'theme.js' as Theme


Label {
    anchors.centerIn: parent
    font.pixelSize: 40 * root.scaleFactor
    color: Theme.colors.placeholder
}
