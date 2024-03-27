//!
//!  \file Full_Search.hpp
//!

#ifndef COMPONENTFULL_SEARCH_HPP
#define COMPONENTFULL_SEARCH_HPP

#include "ComponentFull_Search_global.h"
#include <ComponentsCore5alpha/IComponent>

#include "Object/Full_Search.hpp"

//!
//! \class ComponentSystemGuiCoreNg
//! \brief Основной класс компонента ядра графического интерфейса системы
//!
class COMPONENTFULL_SEARCHSHARED_EXPORT ComponentFull_Search : public ComponentsCore::IComponent
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.TransHub.Qt.ComponentFull_Search" FILE "ComponentFull_Search.json")

public:
    //! \brief Конструткор
    ComponentFull_Search ();
    //! \brief Деструктор
    virtual ~ComponentFull_Search ();

    //! \brief Инициализация компонента.
    //! \return true / false
    bool init () override;

protected:
    //! Объект ядра графического интерфейса
    Algo::Full_Search<int> *_fullSearchInt {nullptr};
    Algo::Full_Search<double> *_fullSearchDouble {nullptr};
};

#endif // COMPONENTFULL_SEARCH_HPP
