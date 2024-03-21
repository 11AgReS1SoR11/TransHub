#ifndef SETTINGSPANELAPPLOADER_H
#define SETTINGSPANELAPPLOADER_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreSettingsPanelWidget.h>

#include <QtWidgets/QWidget>

namespace Ui {
    class SettingsAppLoader;
}

//!
//! \file SettingsAppLoader.h
//! \class SettingsAppLoader
//! \brief Класс виджета настроек загрузчика приложения
//! \author Ivanov Anton
//!
class SettingsAppLoader : public ISystemGuiCoreSettingsPanelWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор
    //! \param parent родительский виджет
    explicit SettingsAppLoader (QWidget *parent = nullptr);

    //! \brief Деструктор
    virtual ~SettingsAppLoader ();

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
    Ui::SettingsAppLoader *ui;      //!< объект граяического интерфейса

    QString _title;                 //!< название раздела
    QIcon   _icon;                  //!< иконка раздела

    QVector<QString> _prevErrors;   //!< список ошибок

    QMap<QString, bool> _lastState; //!< список последних состояний

    //! \brief Загрузить настройки
    void loadSettings ();

    //! \brief Сохранить настройки
    void saveSettings ();

    //! \brief Сохранить последнее состояние
    void saveLastState();

    //! \brief Корректно ли внесены все настройки
    //! \return true / false
    bool isCompletePrivate() const;

protected slots:
    //! \brief Слот обработки воода данных
    //! \param isChecked true / false
    void onCheck (bool isChecked);
};

#endif // SETTINGSPANELAPPLOADER_H
