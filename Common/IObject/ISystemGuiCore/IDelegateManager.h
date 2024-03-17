#ifndef IDELEGATEMANAGER_H
#define IDELEGATEMANAGER_H

#include "ISystemGuiCoreStyleItemDelegate.h"

//!
//! \file IDelegateManager.h
//! \class IDelegateManager
//! \brief Базовый класс менеджера делегатов
//!
class IDelegateManager
{
public:
    //! \brief Деструктор класса
    virtual ~IDelegateManager () {}

    //! \brief Проверить наличие делегата.
    //! \param delegateName Название делегата
    //! \return true / false
    virtual bool contains (const QString &delegateName) const = 0;

    //! \brief Запросить делегат для элементов.
    //! \param delegateName Название делегата
    //! \return Делегат
    virtual ISystemGuiCoreStyleItemDelegate *getItemDelegate (const QString &delegateName) = 0;
};

#endif // IDELEGATEMANAGER_H
