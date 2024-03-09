#include "leftscreen.h"

LeftScreen::LeftScreen(QWidget *parent) : QWidget(parent)
{

    // Создаем вертикальный макет и добавляем в него MapWidget
    MapWidget* mapWidget = new MapWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mapWidget, 1);
    setLayout(layout);
}
