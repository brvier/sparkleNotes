
import QtQuick 2.2

Row {
    id: buttonRow
    property var model

    height: 100 * pgst.scalef

    Repeater {
        id: repeater
        model: buttonRow.model

        delegate: ButtonArea {
            height: buttonRow.height
            width: buttonRow.width / repeater.count
            onClicked: buttonRow.model[index].clicked()

            Label {
                anchors.centerIn: parent
                text: modelData.label
            }
        }
    }
}

