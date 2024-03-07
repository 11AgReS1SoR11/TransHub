import QtQuick 2.0
import QtQuick.Window 2.0
import QtLocation 5.12
import QtPositioning 5.12

Rectangle {
    id: rightScreen

    anchors {
        top: parent.top
        bottom: bottomBar.top
        right: parent.right
    }
    Plugin {
       id: mapPlugin
       PluginParameter {
                   name: "osm.mapping.providersrepository.disabled"
                   value: "true"
               }
               PluginParameter {
                   name: "osm.mapping.providersrepository.address"
                   value: "https://www.mapbox.com/"
               }
       name: "osm"
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(59.9386, 30.3141)
        zoomLevel: 16
    }
    width: parent.width * 2/3
}
