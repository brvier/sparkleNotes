import QtQuick 2.0

MouseArea {
    id: dragging

    property Item stacking
    property bool canClose: true

    anchors.fill: parent

    drag {
        target: parent
        axis: Drag.XAxis
        minimumX: 0
        maximumX: canClose ? parent.width : 0
        filterChildren: true
    }

    onPressedChanged: {
        if (root.loadPageInProgress) {
            return;
        }

        if (pressed) {
            dragging.stacking.stopAllAnimations();
        } else {
            if (parent.x > parent.width / 3) {
                dragging.stacking.startFadeOut();
            } else {
                dragging.stacking.fadeInAgain();
            }
        }
    }
}

