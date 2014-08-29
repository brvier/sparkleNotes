import QtQuick 2.0

import 'theme.js' as Theme

Page {
    id: aboutPage
    //anchors.fill: parent
    width: parent.width
    height: parent.height


    Flickable {
        id: flickable
        anchors.fill: parent
        boundsBehavior: Flickable.StopAtBounds

        contentWidth: detailColumn.width
        contentHeight: detailColumn.height + detailColumn.spacing

        Rectangle {
            anchors.fill: parent
            color:'black'
            opacity: 0.0

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    closePage();
                }
            }
        }

        Column {
            id: detailColumn

            width: aboutPage.width
            spacing: 15 * root.scaleFactor

            PageHeader {
                title: 'About SparkleNotes'
                bgColor: Theme.colors.fresh
                color: '#fff'
                anchors.left: parent.left
                anchors.right: parent.right

            }

            Column {
                width: parent.width               
                spacing: 10 * root.scaleFactor

                Label {
                    font.pixelSize: 30 * root.scaleFactor
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.WordWrap
                    text: 'SparkleNotes ' + '0.0.0' /// __VERSION__
                    color: Theme.colors.highlight
                }

                Label {
                    font.pixelSize: 20 * root.scaleFactor
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.WordWrap
                    text: 'http://khertan.net/'
                    color: Theme.colors.placeholder
                }
            }

            Label {
                width: parent.width * .95
                font.pixelSize: 30 * root.scaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text: [
                    '© 2012-2014 Benoit HERVIER',
                    'License: GPLv3 or later',
                    'Website: http://khertan.net/',
                    '',
                    'Many QML components are derivated from gpodder touch GUI from Thomas Perl : http://github.com/gpodder/gpodder-ui-qml',
                    '',

                    ].join('\n')
            }
        }
    }

    ScrollDecorator { flickableItem: flickable }
}
