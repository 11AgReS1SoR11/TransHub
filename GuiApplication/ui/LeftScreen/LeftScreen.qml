import QtQuick 2.0

Rectangle {
   id: leftScreen
   anchors {
       left: parent.left
       right: rightScreen.left
       bottom: bottomBar.top
       top: parent.top
   }

   color: "blue"
}
