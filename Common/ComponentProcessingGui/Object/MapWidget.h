#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QVBoxLayout>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebChannel/QWebChannel>
#include <QToolBar>
#include "PointCoordinates.h"
#include "planningmanager.h"

#include "ISystemGuiCoreStatusBarTabWindow.h"
#include "../ComponentSystemGuiCoreNg/Object/MdiArea/CustomMdiSubWindow.h"

namespace Ui {
    class MapWidget;
}

//!
//! \brief Виджет отображения интерфейса с картой.
//! \author Кромачев Максим.
//!

class MapWidget : public ISystemGuiCoreStatusBarTabWindow
{
    Q_OBJECT

public:

    struct Marker{
        double latitude;
        double longitude;
        QString markerType;
    };

    enum class State
    {
      WAITING = 0
     ,PROCESSING
    };

    //! \brief Конструктор.
    //! \param parent Родительский виджет.
    explicit MapWidget(QWidget *parent = nullptr);

    //! \brief Деструктор.
    ~MapWidget() {

        emit Planning::PlanningManager::instance()->aboutToRemoveAllObjects();

        //Planning::PlanningManager::deleteInstance();
    }

    enum class Type {
        STORAGE = 0,
        USER,
        TRUCK
    };

    //! \brief Получение иконки для вкладки
    //! \return соответствующая иконка
    QPixmap GetWindowIcon() override;

    //! \brief ????
    Q_INVOKABLE bool plotRoute(double startLat, double startLng, double endLat, double endLng);

    void startPlotting(double startLatitude, double startLongitude, double stopLatitude, double stopLongitude);

protected:
    //! Интерфейс.
    Ui::MapWidget *ui {nullptr};

    //! \brief Создание ToolBar
    void createToolBar();

private:

    QWebEngineView *webView { nullptr };    /**< view для отображения интерфейса с картой */
    QWebChannel *channel { nullptr };       /**< webChannel для соединения с сервисом карты */
    QToolBar* _toolBar { nullptr };
    QHash<QString, Type> markerTypes;

    PointCoordinates* pointCoordinates;
    State appState = State::WAITING;

    QVector<Marker> markers; // vector of all existing markers

signals:
    void strartProcessing();
    void stopProcessing();

protected slots:
    void OnTabClicked(bool is_only_open) override;        /**< toolBar для кнопок для действий над интерфейсом */
};

#endif // MAPWIDGET_H
