#ifndef SYSTEMWINDOWSCREATOR_H
#define SYSTEMWINDOWSCREATOR_H

#include <QtCore/QObject>

#include "../SettingsPanel/SettingsPanel.h"
#include "../SettingsPanel/SettingsPanelContainer.h"

#include "AboutSystemDialog.h"
#include "AboutComponentsWidget.h"

#include <ComponentsCore5alpha/ComponentManager>
#include <IObject/ISystemGuiCore/ISystemGuiCoreParentWidget.h>
#include <IObject/ISystemGuiCore/ISystemIconManager.h>

class MainWindow;

//!
//! \file SystemWindowsCreator.h
//! \class SystemWindowsCreator
//! \brief Класс генератора системных окон
//! \author Кромачев Максим
//
class SystemWindowsCreator : public QObject, public ISystemGuiCoreParentWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param panelContainer Менеджер виджетов настроек.
    //! \param iconManager Менеджер иконок.
    //! \param windowTitle Заголовок окна системы.
    SystemWindowsCreator (SettingsPanelContainer *panelContainer,
                          ISystemIconManager *iconManager,
                          const QString &windowTitle);

    //! \brief Деструктор.
    virtual ~SystemWindowsCreator();

    //! \brief Запросить виджет окна по имени.
    //! \param actionName Название окна (название QAction-а, которое его вызвало)
    //! \return Виджет окна
    QWidget* getWidget (const QString& actionName, const QString &, WidgetType &, WidgetShowType &) override;

    //! \brief Инициализация графического интерфейса компонента.
    void initGui () override {}

protected:
    QString                 _windowTitle;               //!< заголовок окна
    SettingsPanelContainer  *_panelContainer {nullptr}; //!< менеджер виджетов настроек
    ISystemIconManager      *_iconManager {nullptr};    //!< менеджер иконок
};

#endif // SYSTEMWINDOWSCREATOR_H
