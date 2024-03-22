#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QtGui/QIcon>

//!
//! \brief Менеджер ресурсов.
//! \author Кромачев Максим
//!
class ResourceManager
{
private:
    ResourceManager () = default;
    ~ResourceManager () = default;

public:
    //! \brief Создание иконки категории срочности.
    //! 0 - Монолит
    //! 1 - Воздух
    //! 2 - Ракета
    //! 3 - Самолёт
    //! 4 - Обыкновенный
    static QIcon urgencyCategory (int value);
};

#endif // RESOURCEMANAGER_H
