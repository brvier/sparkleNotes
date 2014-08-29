
import QtQuick 2.2

import 'theme.js' as Theme

ButtonArea {
    id: confirmationButton

    property alias text: label.text
    property alias icon: icon.icon
    property color contentColor: Theme.colors.highlight

    Row {
        anchors.centerIn: parent
        spacing: 5 * root.scaleFactor

        Icon {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            color: confirmationButton.contentColor
        }

        Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            color: confirmationButton.contentColor
        }
    }
}
