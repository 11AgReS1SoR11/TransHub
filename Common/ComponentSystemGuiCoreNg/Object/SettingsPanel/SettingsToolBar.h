#ifndef SETTINGSTOOLBAR_H
#define SETTINGSTOOLBAR_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreSettingsPanelWidget.h>

#include "../ToolBar/SystemGuiCoreToolBar.h"

#include <QtCore/QString>
#include <QtCore/QMap>

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>

#include "SettingsToolBarProps.h"

namespace Ui {
    class SettingsToolBar;
}

//!
//! \file SettingsToolBar.h
//! \class SettingsToolBar
//! \brief Класс виджета настроек панелей инструментов
//!
class SettingsToolBar : public ISystemGuiCoreSettingsPanelWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор
    //! \param toolbarsList Список панелей инструментов
    //! \param parent Родительский виджет
    explicit SettingsToolBar (const QList<SystemGuiCoreToolBar*> &toolbarsList, QWidget *parent = nullptr);

    //! \brief Деструктор
    virtual ~SettingsToolBar ();

    //! \brief Название раздела с настройками
    //! \return Название
    QString settingsTitle () const override;

    //! \brief Иконка раздела с настройками
    //! \return Иконка
    QIcon settingsIcon () const override;

    //! \brief Корректно ли внесены все настройки
    //! \return true / false
    bool isComplete () const override;

    //! \brief Применить настройки
    void applySettings () override;

    //! \brief Отменить внесенные изменения и вернуть настройки в предыдущее состояние
    void cancelSettings () override;

    //! \brief Список настроек для сохранения
    //! \return QMap<Название настройки, Значение>
    QMap<QString, QVariant> settingsList () override;

    //! \brief Задать список настроек, считанных из сохраненного ранее файла с настройками
    //! \param settingsList Список настроек
    void setSettingsList (const QMap<QString, QVariant> &settingsList) override;

private:
    Ui::SettingsToolBar *ui;    //!< объект графического интерфейса

    QString _title;             //!< заголовок
    QIcon   _icon;              //!< иконка

    QMap<QString, SystemGuiCoreToolBar*> _toolbarsWidgets;  //!< список панелей инструментов
    QMap<QString, SettingsToolBarProps*> _toolbars;         //!< список свойств панелей инструментов

    //! \brief Корректно ли внесены все настройки
    //! \return true / false
    bool isCompletePrivate () const;
};

#endif // SETTINGSTOOLBAR_H
