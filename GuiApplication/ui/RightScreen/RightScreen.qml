import QtQuick 2.15
import QtQuick.Controls 2.15
import QtWebEngine 1.8

Rectangle {
    id: rightScreen

    anchors {
        top: parent.top
        bottom: bottomBar.top
        right: parent.right
    }

    WebEngineView {
        id: webView
        anchors.fill: parent
        url: "qrc:/../osm.html"
     }


    width: parent.width * 2/3
}
