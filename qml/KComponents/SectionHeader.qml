
import QtQuick 2.2

import 'theme.js' as Theme

Rectangle {
    property alias text: pLabel.text

    height: 40 * root.scaleFactor
    width: root.width


    color: Theme.colors.select


    Label {
        id: pLabel
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 10 * root.scaleFactor
            verticalCenter: parent.verticalCenter
        }
        font.pixelSize: 16 * root.scaleFactor
        color: '#fff'
    }
}

