import QtQuick 2.2
import QtQuick.Window 2.1
import "KComponents"
import "KComponents/theme.js" as Theme
import 'KComponents/icons/icons.js' as Icons

Page {
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
        contentHeight: childrenRect.height + (20 * root.scaleFactor)

        Column {
            spacing: 5* root.scaleFactor

            onHeightChanged: {
                flicker.contentHeight = height + (20 * root.scaleFactor)
            }

            width: flicker.width

            Label {
                text: 'Git Remote URL/Path'
            }

            TextField {
                id: addressField
                width: parent.width - (10 * root.scaleFactor)
                x: 5 *  root.scaleFactor
                Component.onCompleted: {
                    text = (settings.get("gitRemoteUrl") ?  settings.get("gitRemoteUrl") : "")
                }
                onTextChanged: {
                    settings.set("gitRemoteUrl", text)
                }
            }


            Item {
                height:15* root.scaleFactor
                width: parent.width - (20 * root.scaleFactor)
            }

            Button {
                text: 'Generate New SSH Key'
                x: 5 *  root.scaleFactor
                onClicked: {
                    console.log('Generating new key')
                    showConfirmation('Generate New Key',
                                     'Yes',
                                     'No',
                                     'Are you sure you want to generate a new SSH Key ?',
                                     Icons.warning,
                                     function() {
                                         console.log('Generate New SSH Key');
                                         settings.keygen();
                                         pubKey.text = settings.readPubKey();}
                                     )
                }
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - (20 * root.scaleFactor)
                height: 60 * root.scaleFactor
            }

            Item {
                height:15* root.scaleFactor
                width: parent.width - (20 * root.scaleFactor)
            }

            Label {
                text: 'Client ID'
            }

            TextInput {
                id: pubKey
                text: settings.readPubKey()
                x: 5 *  root.scaleFactor
                width: parent.width - (20 * root.scaleFactor)
                wrapMode: TextInput.WrapAnywhere
                readOnly: true
            }

            Item {
                height:20* root.scaleFactor
                width: parent.width - (20 * root.scaleFactor)
            }

            Button {
                text: 'Copy to clipboard'
                onClicked: {
                    pubKey.selectAll();
                    pubKey.copy();
                    pubKey.select(0,0);
                }
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - (20 * root.scaleFactor)
                height: 60 * root.scaleFactor
            }

            Item {
                height:15* root.scaleFactor
                width: parent.width - (20 * root.scaleFactor)
            }

            Label {
                text: 'Font Size'
            }

            Slider {
                min:1
                max:36
                value:9
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                onValueChangeRequested: {
                    value = newValue
                    settings.set("fontSize", value)
                }
                Component.onCompleted: {
                    value = (settings.get("fontSize") ?  settings.get("fontSize") : 9.0)
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



