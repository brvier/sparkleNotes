import QtQuick 2.2
import QtQuick.Window 2.1
import "KComponents"
import "KComponents/theme.js" as Theme
import 'KComponents/icons/icons.js' as Icons

Window {
    id: root
    property bool loadPageInProgress: false
    property real scaleFactor: (width < height) ? (width / 480) : (height / 480)
    property int dialogsVisible: 0
    default property alias children: listPage.children

    visible: true
    width: 768
    height: 1280
    color: '#000000'
    Connections {
        target: notesModel
        onError: showError('Oups', text)
    }

    FontLoader {
        id: fontLoader
        source: Qt.resolvedUrl("KComponents/icons/open-iconic.ttf")
    }

    function topOfStackChanged(offset) {
        if (offset === undefined) {
            offset = 0;
        }
        var page = children[children.length+offset-1];
    }

    function update(page, x) {
        var index = -1;
        for (var i=0; i<children.length; i++) {
            if (children[i] === page) {
                index = i;
                break;
            }
        }

        if (page.isDialog) {
            children[index-1].opacity = 1;
        } else if (children != undefined) {

        } else {
            children[index-1].opacity = x / width;
        }

    }

    //showConfirmation("Are you sure ?", "Yes", "No", "Click on yes will close that dialog, and confirm that you are sure.")
    function showConfirmation(title, affirmative, negative, description, icon, callback) {
        Qt.inputMethod.hide();
        loadPage('KComponents/Confirmation.qml', {
                     title: title,
                     affirmativeAction: affirmative,
                     negativeAction: negative,
                     description: description,
                     icon: icon,
                     callback: callback,
                 });
    }

    //showError("title", "Error message") //Disapear automaticaly after a 20s delay
    function showError(title, message) {
        loadPage('KComponents/Confirmation.qml', {
                     title: title,
                     affirmativeAction: 'Close',
                     negativeAction: 'Negatif',
                     description: message,
                     icon: Icons.warning,
                     callback: undefined,
                 });
    }

    function editNote(path) {
        loadPage('EditPage.qml', { path: path, } );
    }

    function showSelection(items, title, selectedIndex) {
        Qt.inputMethod.hide();
        loadPage('KComponents/SelectionDialog.qml', {
                     title: title,
                     callback: function (index, item) {
                         items[index].callback();
                     },
                     items: function() {
                         var result = [];
                         for (var i in items) {
                             result.push(items[i].label);
                         }
                         return result;
                     }(),
                     selectedIndex: selectedIndex,
                 });
    }

    function setCategoryDialog(path, category) {
        Qt.inputMethod.hide();
        loadPage('KComponents/TextInputDialog.qml', {
                     title: 'Set category',
                     placeholderText: category,
                     text: category,
                     buttonText: 'Set category',
                     callbackOptions: [path],
                     callback: function (new_category, options) {
                         console.log('QML2:'+new_category+', ' + options[0]);
                         notesModel.setCategory(options[0], new_category);
                     },
                     path:path
                 });
    }

    function filterByCategory(cat) {
        notesModel.setFilter(cat);
        notesModel.refresh(false);
    }

    function showCategoriesList() {
        var categories = notesModel.categoryList();
        loadPage('KComponents/SelectionDialog.qml', {
                     title: 'Categories',
                     callback: function (idx, item) {
                         console.log(idx, item);
                         filterByCategory(item); },
                     items: function() {
                         var result = [];
                         for (var i in categories) {
                             result.push(categories[i]);
                         }
                         return result;
                     }(),
                     selectedIndex: 0, //selectedIndex,
                 });

    }

    function loadPage(filename, properties) {
        if (root.loadPageInProgress) {
            console.log('ignoring loadPage request while load in progress');
            return;
        }

        var component = Qt.createComponent(filename);
        if (component.status != Component.Ready) {
            console.log('Error loading ' + filename + ':' +
                        component.errorString());
        }

        if (properties === undefined) {
            properties = {};
        }

        root.loadPageInProgress = true;
        component.createObject(root, properties);
        Qt.inputMethod.hide();

    }

    function milisecondsToString(miliseconds) {
        if (miliseconds > 0) {
            try {
                //get different date time initials.
                var myDate = new Date();
                var difference_ms = myDate.getTime()/1000.0 - miliseconds;
                //take out milliseconds
                difference_ms = difference_ms / 1000;
                var seconds = Math.floor(difference_ms % 60);
                difference_ms = difference_ms / 60;
                var minutes = Math.floor(difference_ms % 60);
                difference_ms = difference_ms / 60;
                var hours = Math.floor(difference_ms % 24);
                difference_ms = difference_ms / 24;
                var days = Math.floor(difference_ms % 7);
                difference_ms = difference_ms / 7;
                var weeks = Math.floor(difference_ms);

                //remove weeks if it exceeds the month limit ie. 4weeks+2days.
                var months = 0;
                if ((weeks == 4 && days >= 2) || (weeks > 4)) {
                    difference_ms = difference_ms * 7;
                    days = Math.floor(difference_ms % 30);
                    difference_ms = difference_ms / 30;
                    months = Math.floor(difference_ms);
                    weeks = 0;
                }
                //check and return the largest value of date time initialized.
                if (months >= 1) {
                    return months + "M ago";
                } else if (weeks != 0) {
                    return weeks + "W ago";
                } else if (days != 0) {
                    return days + "d ago";
                } else if (hours != 0) {
                    return hours + "h ago";
                } else if (minutes != 0) {
                    return minutes + "m ago";
                } else if (seconds != 0) {
                    return seconds + "s ago";
                }
            } catch (e) {
                console.debug(e);
                return 'Now';
            }
            return 'Now';
        }
    }


    Page {
        id: listPage
        anchors.fill: parent
        width: parent.width

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
                    right: parent.right
                    rightMargin: 20 * root.scaleFactor
                    bottom: parent.bottom
                }

                onClicked: {
                    Qt.inputMethod.hide();
                }
            }

        }

        ScrollListView {
            id: notesList

            anchors {
                top: header.bottom
                bottom: toolbar.top
                right: parent.right
                left: parent.left
            }


            section.property: 'category'
            section.delegate: SectionHeader { text: section }
            Placeholder {
                text: 'No notes'
                visible: notesList.count === 0
            }
            model: notesModel
            delegate: NoteItem {
                onClicked: {
                    editNote(path);
                }
                onPressAndHold: {
                    root.showSelection([
                                           {
                                               label: 'Delete',
                                               callback: function () {
                                                   showConfirmation('Delete',
                                                                    'Yes',
                                                                    'No',
                                                                    'Are you sure you want to delete \'' + title +'\'',
                                                                    Icons.warning,
                                                                    function() {
                                                                        console.log('Delete :'+path);
                                                                        notesModel.deleteNote(path);}
                                                                    )}

                                           },
                                           {
                                               label: 'Category',
                                               callback: function () {
                                                   console.log('Set category :'+path);
                                                   setCategoryDialog(path, category);
                                               }
                                           },
                                       ], title);
                }
            }
        }


        Toolbar {
            id:toolbar

            ToolbarButton {
                id:catTool
                icon: Icons.list
                onClicked: {
                    showCategoriesList();
                }
            }

            ToolbarButton {
                icon: Icons.plus
                iconColor: '#0092CC'
                //anchors.right: menuTool.left
                anchors.left: catTool.right
                //anchors.rightMargin: parent.width/2 - (catTool.width*2)
                anchors.leftMargin:(parent.width - (menuTool.width*3)) / 2
                onClicked: {
                    notesModel.create();
                }
            }

            ToolbarButton {
                id: menuTool
                icon: Icons.menu
                anchors.right: parent.right
                onClicked: {
                    root.showSelection([
                                           {
                                               label: 'About',
                                               callback: function () {
                                                   console.log('show about');
                                                   loadPage('KComponents/AboutPage.qml', { } )
                                               }
                                           },
                                           {
                                               label: 'Settings',
                                               callback: function () {
                                                   loadPage('SettingsPage.qml', { } );
                                               }
                                           },

                                       ])
                }
            }


        }
    }
}
