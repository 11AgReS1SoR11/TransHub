#ifndef ISYSTEMICONMANAGER_H
#define ISYSTEMICONMANAGER_H

#include <QtGui/QIcon>
#include <QtCore/QSize>

//!
//! \file ISystemIconManager.h
//! \class ISystemIconManager
//! \brief Класс менеджера иконок
//!
class ISystemIconManager
{
protected:
    //! Деструктор класса
    virtual ~ISystemIconManager() {}

public:
    //! \brief Запросить иконку 16x16 px.
    //! \param iconName Название конки
    //! \return QIcon
    virtual QIcon getIcon_16x16 (const QString &iconName) = 0;

    //! \brief Запросить иконку 24x24 px.
    //! \param iconName Название конки
    //! \return QIcon
    virtual QIcon getIcon_24x24 (const QString &iconName) = 0;

    //! \brief Запросить иконку 32x32 px.
    //! \param iconName Название конки
    //! \return QIcon
    virtual QIcon getIcon_32x32 (const QString &iconName) = 0;

    //! \brief Запросить иконку логотипа системы.
    //!        По умолчанию доступны: 16x16, 32x32, 64x64, 128x128, 256x256
    //! \param iconSize Размер конки
    //! \return QIcon
    virtual QIcon getSystemLogo (const QSize &iconSize) = 0;

    //! \brief Запросить иконку логотипа системы.
    //!        По умолчанию доступны: 16x16, 32x32, 64x64, 128x128, 256x256
    //! \param iconWith Ширина иконки
    //! \param iconHeight Высота иконки
    //! \return QIcon
    virtual QIcon getSystemLogo (int iconWith, int iconHeight) = 0;


    /* --------- НЕ ИСПОЛЬЗОВАТЬ! ------------------------------------------------------ */

    //! \brief Задать иконку логотипа системы.
    //! \param iconSize Размер иконки
    //! \param icon Иконка
    //!
    //! Использование данного метода компонентами категорически запрещено!
    virtual void setSystemLogo (const QSize &iconSize, const QIcon &icon) = 0;

    //! \brief Задать иконку логотипа системы.
    //! \param iconWith Ширина иконки
    //! \param iconHeight Высота иконки
    //! \param icon Иконка
    //!
    //! Использование данного метода компонентами категорически запрещено!
    virtual void setSystemLogo (int iconWith, int iconHeight, const QIcon &icon) = 0;
};

#endif // ISYSTEMICONMANAGER_H
