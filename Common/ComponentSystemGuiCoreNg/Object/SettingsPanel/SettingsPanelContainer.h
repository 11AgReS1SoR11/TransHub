#ifndef SETTINGSPANELCONTAINER_H
#define SETTINGSPANELCONTAINER_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreSettingsPanelWidget.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

//!
//! \file SettingsPanelContainer.h
//! \class SettingsPanelContainer
//! \brief Класс менеджера виджетов настроек
//! \author Ivanov Anton
//!
class SettingsPanelContainer : public QObject
{
    Q_OBJECT

public:
    //! \brief Конструктор
    //! \param parent Родительский объект
    explicit SettingsPanelContainer (QObject* parent = nullptr);

    //! \brief Деструктор
    virtual ~SettingsPanelContainer ();

    //! \brief Добавить виджет настроек
    //! \param widget Виджет
    //! \return true / false
    bool addWidget (ISystemGuiCoreSettingsPanelWidget* widget);

    //! \brief Удалить виджет настроек
    //! \param widget Виджет
    //! \return true / false
    bool removeWidget (ISystemGuiCoreSettingsPanelWidget* widget);

    //! \brief Получить список виджетов настроек
    //! \return Список виджетов настроек
    QList<ISystemGuiCoreSettingsPanelWidget*> settingsPanels () const;

    //! \brief Загрузить настройки
    //! \param settings QSettings
    void loadSettings (QSettings* settings);

    //! \brief Сохранить настройки
    //! \param settings QSettings
    void saveSettings (QSettings* settings);

protected:
    //! список виджетов настроек
    QMap<QString, ISystemGuiCoreSettingsPanelWidget*> _widgets;
};

#endif // SETTINGSPANELCONTAINER_H
