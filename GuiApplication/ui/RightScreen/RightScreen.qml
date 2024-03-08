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

   Image {
       id: exit
       source: "qrc:/ui/pictures/exit.png" // Указываем путь к файлу иконки
       anchors.top: parent.top // Привязываем к верху родителя
       anchors.margins: 10 // Задаем отступ, чтобы иконка не прилипала к краям
       width: parent.width * .9 // Задаем ширину иконки
       height: parent.height * .08 // Задаем высоту иконки
       fillMode: Image.PreserveAspectFit

       MouseArea {
               anchors.fill: parent
               onClicked: {
                   exitAnimation.start();
               }
           }

       SequentialAnimation {
           id: exitAnimation
           PropertyAnimation {
               target: exit
               property: "scale"
               to: 0.75
               duration: 80
           }
           ScriptAction {
               script: Qt.quit()
           }
       }
   }
}
