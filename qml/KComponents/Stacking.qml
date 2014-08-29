import QtQuick 2.0

Item {
    id: stacking
    property variant page: parent

    PropertyAnimation {
        id: fadeIn
        target: stacking.page
        property: 'x'
        to: 0
        duration: 500
        easing.type: Easing.OutCubic

        onStopped: {
            root.loadPageInProgress = false;
        }
    }

    PropertyAnimation {
        id: fadeOut
        target: stacking.page
        property: 'x'
        to: stacking.page.width
        duration: 500
        easing.type: Easing.OutCubic
    }

    function startFadeOut() {
        fadeOut.start();
        root.topOfStackChanged(-1);
        page.destroy(500);
    }

    function fadeInAgain() {
        fadeIn.start();
    }

    function stopAllAnimations() {
        fadeIn.stop();
    }

    Component.onCompleted: {
        if (root.loadPageInProgress) {
            page.x = page.width;
            fadeIn.start();
        }
    }
}

