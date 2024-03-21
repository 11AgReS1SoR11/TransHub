#ifndef SYSTEMICONMANAGER_H
#define SYSTEMICONMANAGER_H

#include <QtCore/QMutex>
#include <QtCore/QMap>
#include <IObject/ISystemGuiCore/ISystemIconManager.h>

//!
//! \file SystemIconManager.h
//! \class SystemIconManager
//! \brief Класс менеджера иконок
//!
class SystemIconManager : public ISystemIconManager
{
public:
    //! Конструктор класса
    SystemIconManager ();

    //! Деструктор класса
    virtual ~SystemIconManager ();

    //!
    //! \brief Запросить иконку 16x16 px.
    //! \param iconName Название конки
    //! \return QIcon
    //!
    QIcon getIcon_16x16 (const QString &iconName);

    //!
    //! \brief Запросить иконку 24x24 px.
    //! \param iconName Название конки
    //! \return QIcon
    //!
    QIcon getIcon_24x24 (const QString &iconName);

    //!
    //! \brief Запросить иконку 32x32 px.
    //! \param iconName Название конки
    //! \return QIcon
    //!
    QIcon getIcon_32x32 (const QString &iconName);

    //!
    //! \brief Запросить иконку логотипа системы.
    //!        По умолчанию доступны: 16x16, 32x32, 64x64, 128x128, 256x256
    //! \param iconSize Размер конки
    //! \return QIcon
    //!
    QIcon getSystemLogo (const QSize &iconSize);

    //!
    //! \brief Запросить иконку логотипа системы.
    //!        По умолчанию доступны: 16x16, 32x32, 64x64, 128x128, 256x256
    //! \param iconWith Ширина иконки
    //! \param iconHeight Высота иконки
    //! \return QIcon
    //!
    QIcon getSystemLogo (int iconWith, int iconHeight);

    // ---- not used!
    //!
    //! \brief Задать иконку логотипа системы.
    //! \param iconSize Размер иконки
    //! \param icon Иконка
    //!
    //! Использование данного метода компонентами категорически запрещено!
    //!
    void setSystemLogo (const QSize &iconSize, const QIcon &icon);

    //!
    //! \brief Задать иконку логотипа системы.
    //! \param iconWith Ширина иконки
    //! \param iconHeight Высота иконки
    //! \param icon Иконка
    //!
    //! Использование данного метода компонентами категорически запрещено!
    //!
    void setSystemLogo (int iconWith, int iconHeight, const QIcon &icon);

protected:

    //! Инициализация базовых иконок
    void initDefaultLogo ();

    bool _init;                         //!< проинициализирован ли объект класса
    QMap<QString, QIcon> _systemLogo;   //!< список системных иконок

    bool _debug;                        //!< Флаг режима отладки.
};

#endif // SYSTEMICONMANAGER_H
