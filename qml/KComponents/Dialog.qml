import QtQuick 2.2

import "theme.js" as Theme

Rectangle {
    id: page
    z: 200

    color: Theme.colors.dialogBackground
    property alias bgColor : contents.color

    Component.onCompleted: root.dialogsVisible = root.dialogsVisible + 1;
    Component.onDestruction: root.dialogsVisible = root.dialogsVisible - 1;

    default property alias children: contents.children
    property bool isDialog: true
    property int contentHeight: -1
    property bool fullWidth: false

    function closePage() {
        stacking.startFadeOut();
    }

    onXChanged: root.update(page, x)

    width: parent.width
    height: parent.height

    DialogStacking { id: stacking }

    MouseArea {
        anchors.fill: parent
        onClicked: page.closePage();
    }

    MouseArea {
        // Tapping on the dialog contents should do nothing
        anchors.fill: contents
    }

    Rectangle {
        id: contents
        property int border: parent.width * 0.1
        width: parent.fullWidth ? parent.width : (parent.width - 2 * border)
        property int maxHeight: parent.height - toolbar.height
        height: ((page.contentHeight > 0 && page.contentHeight < maxHeight) ? page.contentHeight : maxHeight) * parent.opacity
        anchors {
            horizontalCenter: parent.horizontalCenter
            //top: parent.top
            //topMargin: 20 * root.scaleFactor
            verticalCenter: parent.verticalCenter
        }
        color: Theme.colors.dialog
        clip: true
    }
}
