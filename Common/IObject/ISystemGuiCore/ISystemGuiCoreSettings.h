#ifndef ISYSTEMGUICORESETTINGS_H
#define ISYSTEMGUICORESETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

//!
//! \brief Базовый класс глобальных настроек компонента.
//! \author Горбачёв Иван
//!
class ISystemGuiCoreSettings : public QObject
{
    Q_OBJECT

protected:
    //! \brief Конструктор.
    //! \param parent Родительский объект.
    explicit ISystemGuiCoreSettings (QObject *parent = nullptr)
        : QObject (parent)
    {}

    //! \brief Деструктор.
    virtual ~ ISystemGuiCoreSettings () {}

public:
    enum SettinsProperty {
        ToolButtonTextVisible   = 0,
        StatusBarVisible
    };

    //! \brief Установка настройки.
    //! \param property Тип свойства.
    //! \param value Значение свойства.
    virtual void setSettingsProperty (SettinsProperty property, const QVariant &value) = 0;

    //! \brief Установить настройки.
    //! \param groups Список групп.
    //! \param key Ключ.
    //! \param value Значение.
    virtual void setSettingsProperty (const QList<QString> &groups, const QString &key, const QVariant &value) = 0;

    //! \brief Вернуть значение найстройки.
    //! \param property Свойство.
    //! \return Значение свойства.
    virtual QVariant settingsProperty (SettinsProperty property) = 0;

    //! \brief Вернуть найстройки.
    //! \param groups Список групп.
    //! \param key Ключ.
    //! \return Значение.
    virtual QVariant settingsProperty (const QList<QString> &groups, const QString &key) = 0;

    //! \brief Отображение текста в QToolBar.
    //! \param value true/false.
    void setToolButtonTexVisible (bool value) {
        setSettingsProperty (ToolButtonTextVisible, value);
    }

    //! \brief Отображение текста в QToolBar.
    //! \return value true/false.
    bool toolButtonTexVisible () {
        return settingsProperty (ToolButtonTextVisible).toBool ();
    }

    //! \brief Отображение QStatusBar.
    //! \param value true/false
    void setStatusBarVisible (bool value) {
        setSettingsProperty (StatusBarVisible, value);
    }

    //! \brief Отображение QStatusBar.
    //! \return true/false
    bool statusBarVisible () {
        return settingsProperty (StatusBarVisible).toBool ();
    }

signals:
    //! \brief Сигнал изменения значения настройки.
    //! \param property Тип настройки.
    //! \param value Значение настройки.
    void settingsChanged (ISystemGuiCoreSettings::SettinsProperty property, const QVariant &value);

    void settingsChanged (const QList<QString> &groups, const QString &key, const QVariant &value);
};

#endif // ISYSTEMGUICORESETTINGS_H
