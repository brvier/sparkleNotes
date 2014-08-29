import QtQuick 2.2
import QtQuick.Window 2.1
import "KComponents"
import "KComponents/theme.js" as Theme
import 'KComponents/icons/icons.js' as Icons
import net.khertan.documenthandler 1.0


Page {
    //anchors.fill: parent
    width: parent.width
    height: parent.height

    property alias path: editor.path

    onClosing: {
        notesModel.refresh();
    }

    PageHeader {
        id: header
        title: 'SparkleNotes'
        bgColor: Theme.colors.fresh
        color: '#fff'
        anchors.left: parent.left
        anchors.right: parent.right

        MouseArea {
            anchors {
                top: parent.top
                left: parent.left
                right: clipboardIcon.left
                rightMargin: 20 * root.scaleFactor
                bottom: parent.bottom
            }

            onClicked: {
                Qt.inputMethod.hide();
            }
        }

        IconMenuItem {
            id: clipboardIcon

            anchors {
                right: parent.right
                margins: 5 * root.scaleFactor
                verticalCenter: parent.verticalCenter
            }
            color: '#fff'
            icon: Icons.paperclip
            size: 24
            onClicked: {
                root.showSelection([
                                       {
                                           label: 'Undo',
                                           callback: function () {
                                               editor.undo();
                                           }
                                       },
                                       {
                                           label: 'Copy',
                                           callback: function () {
                                               editor.copy();
                                           }
                                       },
                                       {
                                           label: 'Paste',
                                           callback: function() {
                                               editor.paste();
                                           }
                                       },
                                       {
                                           label: 'Cut',
                                           callback: function() {
                                               editor.cut();
                                           }
                                       },
                                       {
                                           label: editor.selectByMouse === false ? 'Select by mouse' : 'Scroll by mouse',
                                           callback: function() {
                                               editor.selectByMouse = ! editor.selectByMouse;
                                           }
                                       },
                                       {
                                           label: 'Insert Date',
                                           callback: function() {
                                               editor.insert(editor.cursorPosition, new Date().toLocaleDateString()) ;
                                           }
                                       },
                                       {
                                           label: 'Select all',
                                           callback: function() {
                                               editor.selectAll();
                                           }
                                       }
                                   ]);
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
        contentHeight: editor.height
        contentWidth: width
        //contentY: slider.y
        function ensureVisible(r)
        {
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY+height <= r.y+r.height)
                contentY = r.y+r.height-height;
        }


        TextEdit {
            property string path: ''

            id: editor
            width: flicker.width
            height: Math.max(flicker.height, implicitHeight)
            selectionColor: Theme.colors.select
            onCursorRectangleChanged: flicker.ensureVisible(cursorRectangle)
            font.pointSize: 16 //* root.scaleFactor//TODO SETTINGS
            focus: true

            DocumentHandler {
                id: document
                target: editor
                cursorPosition: editor.cursorPosition
                selectionStart: editor.selectionStart
                selectionEnd: editor.selectionEnd
                Component.onCompleted: {
                    setStyle('#000', Theme.colors.fresh, Theme.colors.highlight, Theme.colors.select, 16);
                }
            }
            Component.onCompleted: {
                document.fileUrl = path;
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

        ToolbarButton {
            icon: Icons.menu
            anchors.right: parent.right
            onClicked: {
                root.showSelection([
                                       {
                                           label: 'Push',
                                           callback: function () {
                                               console.log('push');
                                           }
                                       },
                                   ])
            }
        }


    }


}
