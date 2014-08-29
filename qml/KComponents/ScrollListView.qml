
import QtQuick 2.0

ListView {
    id: pListView

    //anchors.fill: parent

    property string title
    property real pushPhase: 0
    property string titleBgColor: '#000'
    property string titleColor: '#fff'

    boundsBehavior: Flickable.StopAtBounds

    header: PageHeader {
        id: header
        title: pListView.title
        bgColor: titleBgColor
        color: titleColor
    }

    ScrollDecorator { flickableItem: pListView }
}

