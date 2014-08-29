
import QtQuick 2.2

import 'theme.js' as Theme
import 'icons/icons.js' as Icons

Item {
    id: slidePageHeader
    property alias title: label.text
    property alias color: label.color
    property alias bgColor: background.color

    visible: title !== ''
    z:200

    height: 60 * root.scaleFactor * visible
    //color: Theme.colors.dialogHeader

    Rectangle {
        id: background
        anchors.fill: parent
        //anchors.bottomMargin: 10
        //anchors.topMargin: 10
        color: Theme.colors.dialogHeader
    }

    Label {
        id: label
        anchors {
            left: parent.left
            right: parent.right
            rightMargin: 20 * root.scaleFactor
            leftMargin: 20 * root.scaleFactor
            verticalCenter: parent.verticalCenter
        }

        color: Theme.colors.highlight
        font.pixelSize: parent.height * .6
        elide: Text.ElideRight
    }
}
