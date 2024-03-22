#ifndef SETTINGS_H
#define SETTINGS_H

//!
//! \brief Класс глобальных настроек компонента.
//! \author Сажин Даниил
//!
class Settings
{
    friend class ComponentProcessingGui;

public:
    //! \brief Вернуть инстанс.
    static Settings &instance ();
    //! \brief Удалить инстанс.
    static void deleteInstance ();
    //! \brief Вернуть флаг режима отладки.
    bool debug () const;

private:
    //! \brief Контсруктор.
    Settings () {}
    //! \brief Деструктор.
    ~Settings () {}

    //! Инстанс.
    static Settings *_instance;

    //! Флаг режима отладки.
    bool debug_ {false};
};

#endif // SETTINGS_H
