#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebChannel/QWebChannel>
#include <QToolBar>


namespace Ui {
    class MapWidget;
}

class MapWidget : public QWidget {

    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);

    ~MapWidget() {};

    Q_INVOKABLE bool plotRoute(double startLat, double startLng, double endLat, double endLng);

protected:
    //! Интерфейс.
    Ui::MapWidget *ui {nullptr};

    void createToolBar();


private:

    QWebEngineView *webView { nullptr };
    QWebChannel *channel { nullptr };
    QToolBar* _toolBar { nullptr };

};

#endif // MAPWIDGET_H
