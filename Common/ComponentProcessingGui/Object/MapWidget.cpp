#include "MapWidget.h"
#include "ui_MapWidget.h"
#include "StyleHelper.h"
#include <QHideEvent>


MapWidget::MapWidget(QWidget *parent)
    : QWidget { parent }
    , ui { new Ui::MapWidget }
{

//    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setAttribute(Qt::WA_TranslucentBackground);
//    this->setStyleSheet(StyleHelper::getWindowStyleSheet());
    this->setMouseTracking(true);
    this->setMinimumSize(1200, 900);
    this->setWindowTitle(tr("Map"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    webView = new QWebEngineView(this);
    webView->setUrl(QUrl("qrc:/osm.html"));

    channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("mapWidget"), this);
    webView->page()->setWebChannel(channel);

    createToolBar();

    layout->addWidget(webView);
    layout->addWidget(_toolBar);
    setLayout(layout);
}


void MapWidget::createToolBar ()
{
    _toolBar = new QToolBar (this);
    _toolBar->setIconSize (QSize (32, 32));
    _toolBar->setOrientation (Qt::Vertical);
    _toolBar->setToolButtonStyle (Qt::ToolButtonTextUnderIcon);

    _toolBar->addAction (QIcon (":/man.png"), tr ("Client"), [this](){webView->page()->runJavaScript("flag = 'user';");});
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/storage.png"), tr ("Storage"), [this](){webView->page()->runJavaScript("flag = 'storage';");});
    _toolBar->addAction (QIcon (":/truck_128px.png"), tr ("Track"), [this](){webView->page()->runJavaScript("flag = 'track'");});
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/eraser.png"), tr ("Erase"), [this](){webView->page()->runJavaScript("map.eachLayer(function (layer) {if (layer instanceof L.Marker) {map.removeLayer(layer);}});");});
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/export_32x32.png"), tr ("Export"));
    _toolBar->addAction (QIcon (":/icons/import_32x32.png"), tr ("Import"));
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/help_32x32.png"), tr ("Help"));

    //connect(action, &QAction::triggered, this, &MainWindow::changeVariable("user"));
}

bool MapWidget::plotRoute(double startLat, double startLng, double endLat, double endLng) {
    // Здесь можно добавить код для прокладки маршрута на карте
    (void)startLat;
    (void)startLng;
    (void)endLat;
    (void)endLng;
    return true;
}

