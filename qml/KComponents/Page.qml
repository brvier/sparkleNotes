
import QtQuick 2.2

import 'theme.js' as Theme
import 'icons/icons.js' as Icons

Rectangle {
    id: page
    color: Theme.colors.page

    Component.onCompleted: root.topOfStackChanged();
    default property alias children: dragging.children

    property bool isDialog: false
    signal closing;

    function closePage() {
        closing();
        stacking.startFadeOut();
    }    

    onXChanged: root.update(page, x)

    width: parent.width
    height: parent.height - parent.bottomSpacing

    Stacking { id: stacking }

    Dragging {
        id: dragging
        stacking: stacking
    }

    Image {
        anchors {
            right: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: 10 * root.scaleFactor
        source: Qt.resolvedUrl('images/pageshadow.png')
        opacity: .1
    }
}
