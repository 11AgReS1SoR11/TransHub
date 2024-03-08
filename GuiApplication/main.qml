import QtQuick 2.15
import QtQuick.Window 2.15
import "ui/BottomBar"
import "ui/RightScreen"
import "ui/LeftScreen"

Window {
    width: 1500
    height: 1000
    visible: true
    title: qsTr("TransHubMap")


    RightScreen {
        id: rightScreen
    }

    LeftScreen {
        id: leftScreen
    }

    BottomBar {
        id: bottomBar
    }
}


