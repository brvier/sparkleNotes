
import QtQuick 2.2

import 'theme.js' as Theme

ButtonArea {
    id: noteItem

    transparent: true

    height: 80 * root.scaleFactor
    anchors {
        left: parent.left
        right: parent.right
    }

    /*Rectangle {
        width: 5
        Behavior on width { PropertyAnimation { } }

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }

        color: Theme.colors.fresh
    }*/

    Column {
        anchors {
            left: parent.left
            leftMargin: 5 * root.scaleFactor
            rightMargin: 5 * root.scaleFactor
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        Label {
            id: titleLabel

            elide: Text.ElideRight
            text: title
            color: Theme.colors.fresh
        }

        Label {
            id: dateLabel
            color: Theme.colors.text
            text: (datetime === undefined) ? "Now": milisecondsToString(datetime)
        }
    }


}
