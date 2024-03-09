#include "objects/exiticon.h"


ExitButton::ExitButton(QWidget *parent):QPushButton(parent) {
    QIcon icon = QIcon(":/ui/pictures/exit.png");
    setIcon(icon);
    setIconSize(QSize(40, 40));
    setFixedSize(50, 50);
    // Подключение сигнала нажатия к слоту завершения приложения
    connect(this, &ExitButton::clicked, qApp, &QApplication::quit);
}

