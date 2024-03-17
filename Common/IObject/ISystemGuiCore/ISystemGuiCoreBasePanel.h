#ifndef ISYSTEMGUICORERIGHTPANEL_H
#define ISYSTEMGUICORERIGHTPANEL_H

#include <QtWidgets/QWidget>
#include <QtGui/QIcon>

//!
//! \file ISystemGuiCoreBasePanel.h
//! \class ISystemGuiCoreBasePanel
//! \brief Базовый класс информационной панели
//!
class ISystemGuiCoreBasePanel
{
public:
    //!
    //! \brief Перечисление типов информационных панелей
    //!
    enum BasePanelType {
        BasePanelLeft = 0,  /**< левая информационная панель */
        BasePanelRight,     /**< правая информационная панель */
        BasePanelBottom     /**< нижняя информационная панель */
    };

    //!
    //! \brief Перечисление положений вкладок внутри панели
    //!
    enum BasePanelTabPosition {
        BasePanelTabDefault = 0,    /**< по умолчанию */
        BasePanelTabNorth,          /**< по верхнему краю */
        BasePanelTabSouth,          /**< по нижнему краю */
        BasePanelTabWest,           /**< по левому краю */
        BasePanelTabEast            /**< по правому краю */
    };

    //! \brief Деструктор класса
    virtual ~ISystemGuiCoreBasePanel () {}

    //! \brief Запросить тип панели
    //! \return Тип панели
    virtual ISystemGuiCoreBasePanel::BasePanelType panelType () = 0;

    //! \brief Запросить положение вкладок внутри панели
    //! \return Положение вкладок
    virtual ISystemGuiCoreBasePanel::BasePanelTabPosition tabPosition () = 0;

    //! \brief Задать положение вкладок внутри панели
    //! \param положение вкладок
    virtual void setTabPosition (ISystemGuiCoreBasePanel::BasePanelTabPosition tabPosition) = 0;

    //! \brief Добавить вкладку с виджетом
    //! \param tabName Название вкладки
    //! \param tabIcon Иконка вкладки
    //! \param tabWidget Виджет
    virtual void addTabWidget (const QString &tabName, const QIcon &tabIcon, QWidget* tabWidget) = 0;
};

#endif // ISYSTEMGUICORERIGHTPANEL_H
