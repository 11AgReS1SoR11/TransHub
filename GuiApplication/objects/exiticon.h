#ifndef EXITICON_H
#define EXITICON_H

#include <QtWidgets>
#include <QPushButton>

class ExitButton : public QPushButton {
public:
    ExitButton(QWidget *parent = nullptr) : QPushButton(parent) {
        QIcon icon = QIcon(":/ui/pictures/exit.png");
        setIcon(icon);
        setIconSize(QSize(40, 40));
        setFixedSize(50, 50);
        // Подключение сигнала нажатия к слоту завершения приложения
        connect(this, &ExitButton::clicked, qApp, &QApplication::quit);
    }
};



#endif // EXITICON_H
