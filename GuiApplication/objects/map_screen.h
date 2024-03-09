#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include <QWidget>
#include <QHBoxLayout>
#include "mapwidget.h"

class MapScreen : public QWidget {
public:
    MapScreen(QWidget *parent = nullptr);
};

#endif // MAPSCREEN_H
