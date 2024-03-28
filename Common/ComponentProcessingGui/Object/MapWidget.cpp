#include "MapWidget.h"
#include "ui_MapWidget.h"
#include "StyleHelper.h"


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

    _toolBar->addAction (QIcon (":/icons/show_32x32.png"), tr ("Show"));
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/edit_32x32.png"), tr ("Edit"));
    _toolBar->addAction (QIcon (":/icons/cancel_32x32.png"), tr ("Cancel"));
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/print_32x32.png"), tr ("Print\njournal"));
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/export_32x32.png"), tr ("Export"));
    _toolBar->addAction (QIcon (":/icons/import_32x32.png"), tr ("Import"));
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/help_32x32.png"), tr ("Help"));
}

void MapWidget::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    setWindowFlags(Qt::FramelessWindowHint);
}


bool MapWidget::plotRoute(double startLat, double startLng, double endLat, double endLng) {
    // Здесь можно добавить код для прокладки маршрута на карте
    (void)startLat;
    (void)startLng;
    (void)endLat;
    (void)endLng;
    return true;
}

