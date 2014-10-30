import QtQuick 2.0

import 'theme.js' as Theme

Item {
    id: slider

    property real value
    property real min: 0.0
    property real max: 1.0
    property color color: Theme.colors.highlight

    property real displayedValue: mouseArea.pressed ? temporaryValue : value
    property real temporaryValue

    signal valueChangeRequested(real newValue)

    clip: true

    height: 50 * root.scaleFactor

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            slider.valueChangeRequested(min + (max - min) * (mouse.x / width))
            //parent.height = 30 * root.scaleFactor
        }
        onPressed: {
            //parent.height = 50 * root.scaleFactor
            slider.temporaryValue = (min + (max - min) * (mouse.x / width));
        }
        onPositionChanged: {
            slider.temporaryValue = (min + (max - min) * (mouse.x / width));
        }
        preventStealing: true
    }

    Rectangle {
        anchors.fill: parent
        color: slider.color
        opacity: .3
    }
    Rectangle {
        id: fillBackground
        color: slider.color
        height: parent.height
        width: parent.width * (parent.displayedValue - parent.min) / (parent.max - parent.min)

        anchors {
            verticalCenter: parent.verticalCenter
        }
    }


    Label {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: Math.round(displayedValue)
        color: (displayedValue > ((max + min) / 2)) ? Theme.colors.textondark : Theme.colors.highlight
    }

}
