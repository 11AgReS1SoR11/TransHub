#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>

class MapWidget : public QWidget {
public:
    MapWidget(QWidget *parent = nullptr);

    Q_INVOKABLE void plotRoute(double startLat, double startLng, double endLat, double endLng);

};

#endif // MAPWIDGET_H
