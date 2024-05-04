#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebChannel/QWebChannel>
#include <QToolBar>
#include "PointCoordinates.h"
#include "planningmanager.h"


namespace Ui {
    class MapWidget;
}

class MapWidget : public QWidget {

    Q_OBJECT

public:

    struct Message{
        double x;
        double y;
        QString markerType;
        QString opType;
    };

    enum class State
    {
      WAITING = 0
     ,PROCESSING
    };

    enum class Type {
        STORAGE = 0,
        USER,
        TRUCK
    };

    explicit MapWidget(QWidget *parent = nullptr);

    Q_INVOKABLE bool plotRoute(double startLat, double startLng, double endLat, double endLng);

protected:
    //! Интерфейс.
    Ui::MapWidget *ui {nullptr};

    void createToolBar();


private:

    QWebEngineView *webView { nullptr };
    QWebChannel *channel { nullptr };
    QToolBar* _toolBar { nullptr };
    QHash<QString, Type> markerTypes;

    PointCoordinates* pointCoordinates;
    State appState = State::WAITING;

signals:
    void markerSignal(Message mes);
    void strartProcessing();
    void stopProcessing();


};

#endif // MAPWIDGET_H
