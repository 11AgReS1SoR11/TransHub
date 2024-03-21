#ifndef BASEPANELWIDGET_H
#define BASEPANELWIDGET_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreBasePanel.h>

#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QMap>

#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>

namespace Ui {
class BasePanelWidget;
}

//!
//! \file BasePanelWidget.h
//! \class BasePanelWidget
//! \brief Базовый класс информационной панели
//!
class BasePanelWidget : public QWidget, public ISystemGuiCoreBasePanel
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param panelType Тип панели
    //! \param parent Виджет-родитель
    explicit BasePanelWidget (ISystemGuiCoreBasePanel::BasePanelType panelType, QWidget *parent = nullptr);

    //! \brief Деструктор класса
    ~BasePanelWidget();

    //! \brief Запросить виджет со вкладками
    //! \return QTabWidget
    QTabWidget* getTabWidget ();

    //! \brief Инициализация вкладок
    void initTabs ();

    //! \brief Обновление положения вкладок
    void updateTabs ();

    //! \brief Показать / Скрыть вкладку
    //! \param tabName Название вкладки
    //! \param value Видимость
    void setVisibleTab (const QString& tabName, bool value);

    //! \brief Задать порядок вкладок
    //! \param tabsName Вектор названий вкладок
    void setTabsPosition (QVector<QString>* tabsName);

    //! \brief Задать активную вкладку
    //! \param tabName Название вкладки
    void setActiveTab (const QString& tabName);

    //! \brief Запросить тип панели
    //! \return Тип панели
    ISystemGuiCoreBasePanel::BasePanelType panelType () override;

    //! \brief Запросить положение вкладок внутри панели
    //! \return Положение вкладок
    ISystemGuiCoreBasePanel::BasePanelTabPosition tabPosition () override;

    //! \brief Задать положение вкладок внутри панели
    //! \param tabPosition положение вкладок
    void setTabPosition (ISystemGuiCoreBasePanel::BasePanelTabPosition tabPosition) override;

    //! \brief Добавить вкладку с виджетом
    //! \param tabName Название вкладки
    //! \param tabIcon Иконка вкладки
    //! \param tabWidget Виджет
    void addTabWidget (const QString& tabName,
                       const QIcon &tabIcon,
                       QWidget *tabWidget) override;

private:
    //! \brief Функция разворачивания иконки на 90 градусов
    //! \param icon Иконка
    //! \param angle Угол разворота
    //! \return Развернутая иконка
    QIcon rotateIcon (const QIcon &icon, int angle = 90);

    //! \brief Видимая ли вкладка
    //! \param tabName Название
    //! \return true / false
    bool isVisibleTab (const QString &tabName);

    Ui::BasePanelWidget *ui;                    /**< объект графического интерфейса */

    QMap <QString, QWidget*>    _allTabs;       /**< виджеты вкладок */
    QMap <QString, QIcon>       _allTabsIcon;   /**< иконки вкладок */

    QMutex _mutex;                              /**< мьютекс */

    BasePanelType               _panelType;     /**< тип панели */
    BasePanelTabPosition        _tabPosition;   /**< положение вкладок */
};

#endif // BASEPANELWIDGET_H
