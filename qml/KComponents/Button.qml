import QtQuick 2.2
import "theme.js" as Theme


Rectangle {
    id: button

    property alias text: textItem.text
    property alias font: textItem.font

    signal clicked

    width: textItem.width + 40; height: textItem.height + 10
    border.width: 1
    radius: height/5
    smooth: true

    /*gradient: Gradient {
        GradientStop { id: topGrad; position: 0.0; color: "#333" }
        GradientStop { id: bottomGrad; position: 1.0; color: "#333" }
    }*/
    color: Theme.colors.button

    Text {
        id: textItem
        x: parent.width/2 - width/2; y: parent.height/2 - height/2
        font.pixelSize: 18 * root.scaleFactor
        color: "white"
        style: Text.Raised
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: button.clicked()
    }

    states: State {
        name: "pressed"; when: mouseArea.pressed && mouseArea.containsMouse
        PropertyChanges { target: button; color: Theme.colors.buttonClicked }
        PropertyChanges { target: textItem; x: textItem.x + 1; y: textItem.y + 1; explicit: true }
    }
}
