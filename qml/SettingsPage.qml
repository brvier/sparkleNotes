import QtQuick 2.2
import QtQuick.Window 2.1
import "KComponents"
import "KComponents/theme.js" as Theme
import 'KComponents/icons/icons.js' as Icons

Page {
    //anchors.fill: parent
    width: parent.width
    height: parent.height

    PageHeader {
        id: header
        title: 'sparkleNotes'
        bgColor: Theme.colors.fresh
        color: '#fff'
        anchors.left: parent.left
        anchors.right: parent.right

        MouseArea {
            anchors.fill: parent

            onClicked: {
                Qt.inputMethod.hide();
            }
        }
    }




    Flickable {
        id:flicker
        anchors {
            top: header.bottom
            bottom: toolbar.top
            left: parent.left
            right: parent.right
        }
        clip: true

        Column {
            width: flicker.width

            Label {
                text: 'Address'
            }

            TextField {
                id: addressField
                width: parent.width - (10 * root.scaleFactor)
                 x: 5 *  root.scaleFactor

            }

            Label {
                text: 'Remote Path'
            }

            TextField {
                id: remotePathField
                width: parent.width - (10 * root.scaleFactor)
                                x: 5 *  root.scaleFactor
            }

            Label {
                text: 'SSH Key'
            }

            TextField {
                id: sshKeyField
                width: parent.width - (10 * root.scaleFactor)
                x: 5 *  root.scaleFactor
            }

            Label {
                text: 'Font Size'
            }

            Slider {
                min:1
                max:128
                value:9
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                onValueChangeRequested: {
                    value = newValue
                }

            }

        }



    }
    ScrollDecorator { flickableItem: flicker }

    Toolbar {
        id:toolbar

        ToolbarButton {
            icon: Icons.arrow_left
            onClicked: {
                closePage();
            }
        }



    }


}
