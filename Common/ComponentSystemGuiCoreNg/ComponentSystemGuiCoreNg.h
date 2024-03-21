//!
//!  \file SystemGuiCoreNg.h
//!

#ifndef COMPONENTSYSTEMGUICORENG_H
#define COMPONENTSYSTEMGUICORENG_H

#include "ComponentSystemGuiCoreNg_global.h"
#include <ComponentsCore5alpha/IComponent>

#include "Object/SystemGuiCoreNg.h"

//!
//! \class ComponentSystemGuiCoreNg
//! \brief Основной класс компонента ядра графического интерфейса системы
//!
class COMPONENTSYSTEMGUICORENGSHARED_EXPORT ComponentSystemGuiCoreNg : public ComponentsCore::IComponent
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.inteltech.Qt.ComponentSystemGuiCoreNg" FILE "ComponentSystemGuiCoreNg.json")

public:
    //! \brief Конструткор
    ComponentSystemGuiCoreNg ();
    //! \brief Деструктор
    virtual ~ComponentSystemGuiCoreNg ();

    //! \brief Инициализация компонента.
    //! \return true / false
    bool init () override;

protected:
    //! Объект ядра графического интерфейса
    SystemGuiCoreNg *_guiCore {nullptr};
};

#endif // COMPONENTSYSTEMGUICORENG_H
