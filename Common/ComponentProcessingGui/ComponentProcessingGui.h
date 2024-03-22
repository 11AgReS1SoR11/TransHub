#ifndef COMPONENTPROCESSINGGUI_H
#define COMPONENTPROCESSINGGUI_H

#include "ComponentProcessingGui_global.h"
#include <ComponentsCore5alpha/IComponent>


#include "Object/ProcessingGui.h"

//!
//! \brief Класс компонента.
//! \author Кромачев Максим
//!
class COMPONENTPROCESSINGGUISHARED_EXPORT ComponentProcessingGui
        : public ComponentsCore::IComponent
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.TransHub.Qt.ComponentProcessingGui"
                      FILE "ComponentProcessingGui.json")

public:
    //! \brief Конструктор.
    ComponentProcessingGui ();

    //! \brief Деструктор.
    virtual ~ComponentProcessingGui ();

    //! \brief Инициализация компонента.
    //! \return true / false
    bool init () override;

protected:
    //! Интерфейс
    ProcessingGui *_editor {nullptr};
};

#endif // COMPONENTPROCESSINGGUI_H
