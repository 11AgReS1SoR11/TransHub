#ifndef SYSTEMGUICORESETTINGS_H
#define SYSTEMGUICORESETTINGS_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreSettings.h>

//!
//! \brief Класс глобальных настроек компонента.
//! \author Кромачев Максим
//!
//! На подумать.
//! Проблемы - настройки не прменяеются мгновенно, так как класс не имеет доступа к
//! SystemGuiCoreNg. Варианты решения -
//! 1. Передать SystemGuiCoreNg в качестве указателя и объявить друженственным классом, тогда
//! класс настроек получить доступ ко всему, чему возможно;
//! 2. Скидывать сигналы об изменениях настроек, а там где надо, ловить (но так как настроек
//! может оказаться очень много, то делать много сигналов, или один на всех?);
//! 3. Класс настроек пишет настройки отображения для всех главных окон. А нужно ли это вообще менять?
//!
//! В качестве упрощения доступа к классу, можно скинуть его в "кучу" ко всем, и в случае необходимости
//! искать не SystemGuiCoreNg, а сразу ISystemGuiCoreSettings.
//!
class SystemGuiCoreSettings : public ISystemGuiCoreSettings
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param parent Родительский объект.
    explicit SystemGuiCoreSettings (QObject *parent = nullptr);

    //! \brief Деструктор.
    virtual ~SystemGuiCoreSettings ();

    //! \brief Установка значение настройки.
    //! \param property Тип свойства.
    //! \param value Значение свойства.
    void setSettingsProperty (SettinsProperty property, const QVariant &value) override;

    //! \brief Установить настройки.
    //! \param key Ключ.
    //! \param value Значение.
    void setSettingsProperty (const QList<QString> &groups, const QString &key, const QVariant &value) override;

    //! \brief Вернуть значение найстройки.
    //! \param property Свойство.
    //! \return Значение свойства.
    QVariant settingsProperty (SettinsProperty property) override;

    //! \brief Вернуть найстройки.
    //! \param key Ключ.
    //! \return Значение.
    QVariant settingsProperty (const QList<QString> &groups, const QString &key) override;
};

#endif // SYSTEMGUICORESETTINGS_H
