#include "transhubwingow.h"

TransHubWindow::TransHubWindow(QWidget *parent) : QMainWindow(parent)
{
    // Создаем вертикальный макет
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Создаем QSplitter для разделения экрана на две части (горизонтальное разделение)
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setContentsMargins(0, 0, 0, 0);

    // Создаем и добавляем виджеты LeftScreen и RightScreen в QSplitter
    LeftScreen* left_screen = new LeftScreen();
    RightScreen* right_screen = new RightScreen();
    splitter->addWidget(left_screen);
    splitter->addWidget(right_screen);

    // Устанавливаем пропорции для размеров виджетов
    splitter->setStretchFactor(0, 20); // Левый виджет будет занимать 2/3 ширины
    splitter->setStretchFactor(1, 1); // Правый виджет будет занимать 1/3 ширины

    // Добавляем QSplitter в вертикальный макет
    layout->addWidget(splitter);

    // Устанавливаем макет для главного окна
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

