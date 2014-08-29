import QtQuick 2.2

Item {
    id: stacking
    property variant page: parent

    PropertyAnimation {
        id: fadeIn
        target: stacking.page
        property: 'opacity'
        to: 1
        duration: 500
        easing.type: Easing.OutCubic

        onStopped: {
            root.loadPageInProgress = false;
        }
    }

    PropertyAnimation {
        id: fadeOut
        target: stacking.page
        property: 'opacity'
        to: 0
        duration: 500
        easing.type: Easing.OutCubic
    }

    function startFadeOut() {
        fadeOut.start();
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
            page.x = 0;
            page.opacity = 0;
            fadeIn.start();
        }
    }
}

