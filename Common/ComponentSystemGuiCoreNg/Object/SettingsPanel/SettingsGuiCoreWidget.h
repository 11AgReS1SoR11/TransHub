#ifndef SETTINGSGUICOREWIDGET_H
#define SETTINGSGUICOREWIDGET_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreSettings.h>
#include <IObject/ISystemGuiCore/ISystemGuiCoreSettingsPanelWidget.h>

#include <QtWidgets/QWidget>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QAction>

namespace Ui {
    class SettingsGuiCoreWidget;
}

//!
//! \file SettingsGuiCore.h
//! \class SettingsGuiCore
//! \brief Класс виджета настроек рабочей области
//! \author Кромачев Максим
//!
class SettingsGuiCoreWidget : public ISystemGuiCoreSettingsPanelWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор класса.
    //! \param mdiArea Рабочая область.
    //! \param tileAct Раздел меню "Плитка".
    //! \param cascadeAct Раздел меню "Каскадом".
    //! \param parent Родительский виджет.
    explicit SettingsGuiCoreWidget (ISystemGuiCoreSettings *settings, QMdiArea *mdiArea, QAction *tileAct,
                                    QAction *cascadeAct, QWidget *parent = nullptr);

    //! \brief Деструктор.
    virtual ~SettingsGuiCoreWidget ();

    //! \brief Название раздела с настройками.
    //! \return Название.
    QString settingsTitle () const override;

    //! \brief Иконка раздела с настройками.
    //! \return Иконка.
    QIcon settingsIcon () const override;

    //! \brief Корректно ли внесены все настройки.
    //! \return true / false.
    bool isComplete () const override;

    //! \brief Применить настройки.
    void applySettings () override;

    //! \brief Отменить внесенные изменения и вернуть настройки в предыдущее состояние.
    void cancelSettings () override;

    //! \brief Список настроек для сохранения.
    //! \return QMap<Название настройки, Значение>.
    QMap<QString, QVariant> settingsList () override;

    //! \brief Задать список настроек, считанных из сохраненного ранее файла с настройками.
    //! \param settingsList Список настроек.
    void setSettingsList (const QMap<QString, QVariant> &settingsList) override;

private:
    Ui::SettingsGuiCoreWidget *ui {nullptr};  //!< объект графического интерфейса

    ISystemGuiCoreSettings *_settings {nullptr};

    QMdiArea *_mdiArea {nullptr};       //!< рабочая область
    QAction *_tileAct {nullptr};        //!< раздел меню "Плитка"
    QAction *_cascadeAct{nullptr};      //!< раздел меню "Каскадом"

    QString _title;     //!< заголовок
    QIcon   _icon;      //!< иконка
    bool    _isLoad;    //!< загружен ли виджет

    QMap<QString, bool> _lastState; //!< последнее состояние

    //! \brief Сохранить последнее состояние
    void saveLastState ();
};

#endif // SETTINGSGUICOREWIDGET_H
