import QtQuick 2.15
import QtQuick.Controls 2.15
import QtWebEngine 1.8

Rectangle {
   id: rightScreen
   anchors {
       right: parent.right
       left: leftScreen.right
       bottom: bottomBar.top
       top: parent.top
   }

   color: "white"
}
