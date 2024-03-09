#include "rightscreen.h"

RightScreen::RightScreen(QWidget *parent)
    : QWidget{ parent }
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    ExitButton* exit_icon = new ExitButton();
    layout->addStretch(1); // Добавление пружины для размещения в верхней части окна
    layout->addWidget(exit_icon, 0, Qt::AlignHCenter); // 0 для горизонтального выравнивания и Qt::AlignHCenter для центрирования
    layout->addWidget(exit_icon);
    setLayout(layout);
}
