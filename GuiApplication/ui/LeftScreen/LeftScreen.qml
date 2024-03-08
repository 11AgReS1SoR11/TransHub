import QtQuick 2.15
import QtQuick.Controls 2.15
import QtWebEngine 1.8

Rectangle {
    id: leftScreen

    anchors {
        top: parent.top
        bottom: bottomBar.top
        left: parent.left
    }

    WebEngineView {
        id: webView
        anchors.fill: parent
        url: "qrc:/../osm.html"
     }

    width: parent.width * 25/26
}
