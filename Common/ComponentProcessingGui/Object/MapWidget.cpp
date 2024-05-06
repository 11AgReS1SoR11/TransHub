#include "MapWidget.h"
#include "ui_MapWidget.h"
#include "StyleHelper.h"
#include <QHideEvent>
#include <QThread>


MapWidget::MapWidget(QWidget *parent)
    : ISystemGuiCoreStatusBarTabWindow { parent }
    , ui { new Ui::MapWidget }
{
    setMouseTracking(true);
    setMinimumSize(1200, 900);
    setWindowTitle(tr("Map"));

    markerTypes.insert("storage", Type::STORAGE);
    markerTypes.insert("track", Type::TRUCK);
    markerTypes.insert("user", Type::USER);

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

    pointCoordinates = new PointCoordinates(this);

    channel = new QWebChannel();
    webView->page()->setWebChannel(channel);

    connect(pointCoordinates, &PointCoordinates::pointClicked, [=](double latitude, double longitude, QString markerType, QString opType) {
        qDebug() << "Received coordinates: Latitude:" << latitude
                 << ", Longitude:" << longitude << ", marker type:" << markerType << ", operation type:" << opType;


        if (opType == "adding")
            markers.push_back({latitude, longitude, markerType});
        else
        {
            for (int i = 0; i < markers.size(); ++i)
            {
                if (markers[i].latitude == latitude && markers[i].longitude == longitude && markers[i].markerType == markerType)
                    markers.removeAt(i);
            }
        }         


        auto get_object = [](Type value, double latitude, double longitude) -> std::unique_ptr<QObject> {
            switch(value) {
                case Type::STORAGE:
                    return std::make_unique<Planning::Storage>(nullptr, latitude, longitude);
                case Type::TRUCK:
                    return std::make_unique<Planning::Truck>(nullptr, latitude, longitude);
                case Type::USER:
                    return std::make_unique<Planning::User>(nullptr, latitude, longitude);
                default:
                    return nullptr;
            }
        };

        auto get_object2 = [](Type value, double latitude, double longitude) -> QObject* {
            switch(value) {
                case Type::STORAGE:
                    return new Planning::Storage(nullptr, latitude, longitude);
                case Type::TRUCK:
                    return new Planning::Truck(nullptr, latitude, longitude);
                case Type::USER:
                    return new Planning::User(nullptr, latitude, longitude);
                default:
                    return nullptr;
            }
        };

        if(opType == QString("adding")) {

            if (markerTypes.contains(markerType)) {

                Type value = markerTypes.value(markerType);
                QObject* Object = get_object2(value, latitude, longitude);
                if(Object != nullptr) {

                    if(Planning::PlanningManager::addObject(Object) == true) {
                        emit Planning::PlanningManager::instance()->aboutAddedObject();
                    }

                }
                else {
                    qInfo () << qPrintable (QString ("[MapWidget] Object has nullpointer value"));
                }
            }
        }
        else if(opType == QString("deleting")) {

            if (markerTypes.contains(markerType)) {

                Type value = markerTypes.value(markerType);
                QObject* Object = get_object2(value, latitude, longitude);
                if(Object != nullptr) {

                    if(Planning::PlanningManager::removeObject(Object) == true) {
                        emit Planning::PlanningManager::instance()->aboutRemovedObject();
                    }
                    delete Object;
                }
                else {
                    qInfo () << qPrintable (QString ("[MapWidget] Object has nullpointer value"));
                }
            }
        }
    });

     channel->registerObject("pointCoordinates", pointCoordinates);

}

QPixmap MapWidget::GetWindowIcon()
{
    return QPixmap( ":/icons/icons/map.png" );
}


void MapWidget::createToolBar ()
{
    _toolBar = new QToolBar (this);
    _toolBar->setIconSize (QSize (32, 32));
    _toolBar->setOrientation (Qt::Vertical);
    _toolBar->setToolButtonStyle (Qt::ToolButtonTextUnderIcon);

    _toolBar->addAction (QIcon (":/user.png"), tr ("Client"), [this](){
         webView->page()->runJavaScript("markerType = 'user';");
    });

    _toolBar->addAction (QIcon (":/storage3.png"), tr ("Storage"), [this](){
        webView->page()->runJavaScript("markerType = 'storage';");
    });

    _toolBar->addAction (QIcon (":/truck_128px.png"), tr ("Track"), [this](){
        webView->page()->runJavaScript("markerType = 'track'");
    });

    _toolBar->addSeparator ();

    _toolBar->addAction (QIcon (":/eraser.png"), tr ("Erase"), [this](){
        webView->page()->runJavaScript("eraseAll()");

        emit Planning::PlanningManager::instance()->aboutToRemoveAllObjects();

    });

    _toolBar->addSeparator ();

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _toolBar->addWidget(empty);


    _toolBar->addAction(QIcon::fromTheme("media-playback-start"), tr("Start"), [this](){
        if (appState == State::WAITING)
        {
            appState = State::PROCESSING;
            this->_toolBar->actions()[7]->setIcon(QIcon::fromTheme("media-playback-stop"));


            //test code start
            startPlotting(59.9584, 30.3141, 59.9387, 30.3142);
            //test code end

            Planning::PlanningManager::instance()->stop();
        }
        else
        {
            appState = State::WAITING;
            this->_toolBar->actions()[7]->setIcon(QIcon::fromTheme("media-playback-start"));
            Planning::PlanningManager::instance()->startup();
        }
    });

        _toolBar->addAction(QIcon::fromTheme("view-refresh"), tr("Refresh"), [this](){
            qDebug() << "Refresh button pressed";
            for (auto& tmp : markers)
            {
                qDebug() << "latitude: " << tmp.latitude << " longitude: " << tmp.longitude << " marker: " << tmp.markerType;
            }
        });
    _toolBar->addSeparator ();

    _toolBar->addAction (QIcon (":/icons/export_32x32.png"), tr ("Export"), [this](){
        qDebug() << "Export button pressed";
    });
    _toolBar->addAction (QIcon (":/icons/import_32x32.png"), tr ("Import"), [this](){
        qDebug() << "Import button pressed";
    });

    _toolBar->addSeparator ();

    _toolBar->addAction (QIcon (":/icons/help_32x32.png"), tr ("Help"), [this](){
        qDebug() << "Help button pressed";
    });

    _toolBar->addSeparator();

}

void MapWidget::OnTabClicked(bool is_only_open)
{
    if ( !is_only_open && isVisible() )
    {
        if ( QWidget * parWidget = qobject_cast<QWidget *>( parent() ) )
            if ( CustomMdiSubWindow * parMdi = dynamic_cast<CustomMdiSubWindow *>( parWidget ) )
                parMdi->TabHide();
    }
    else
    {
        if ( !isVisible() )
            emit OpenTabWindow();
    }
}

bool MapWidget::plotRoute(double startLat, double startLng, double endLat, double endLng) {
    // Здесь можно добавить код для прокладки маршрута на карте
    (void)startLat;
    (void)startLng;
    (void)endLat;
    (void)endLng;
    return true;
}


 void MapWidget::startPlotting(double startLatitude, double startLongitude, double stopLatitude, double stopLongitude)
 {
     webView->page()->runJavaScript("plotRoutes(" + QString::number(startLatitude) + ',' + QString::number(startLongitude) + ',' + QString::number(stopLatitude) + ',' + QString::number(stopLongitude) + ");");
 }
