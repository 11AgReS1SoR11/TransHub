#ifndef ISYSTEMGUICORESETTINGSPANELWIDGET_H
#define ISYSTEMGUICORESETTINGSPANELWIDGET_H

#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QMap>

#include <QtGui/QIcon>
#include <QtWidgets/QWidget>

//!
//! \file ISystemGuiCoreSettingsPanelWidget.h
//! \class ISystemGuiCoreSettingsPanelWidget
//! \brief Базовый класс виджета настроек
//!
//! Удаляется автоматически!
//!
class ISystemGuiCoreSettingsPanelWidget : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param parent Родительский виджет.
    //! \param f Флаг
    explicit ISystemGuiCoreSettingsPanelWidget (QWidget* parent = nullptr)
        : QWidget (parent) {}

    //! \brief Деструктор.
    virtual ~ISystemGuiCoreSettingsPanelWidget () {}

    //! \brief Получить название раздела с настройками.
    //! \return Название раздела с настройками.
    virtual QString settingsTitle () const = 0;

    //! \brief Получить иконку раздела с настройками.
    //! \return Иконка раздела с настройками.
    virtual QIcon settingsIcon () const = 0;

    //! \brief Корректно ли внесены все настройки.
    //! \return true / false.
    virtual bool isComplete () const = 0;

    //! \brief Применить настройки.
    virtual void applySettings () = 0;

    //! \brief Отменить внесенные изменения и вернуть настройки в предыдущее состояние.
    virtual void cancelSettings () = 0;

    //! \brief Получить список настроек для сохранения.
    //! \return Список настроек для сохранения.
    //!
    //! key     - Название настройки
    //! value   - Значение
    virtual QMap<QString, QVariant> settingsList () = 0;

    //! \brief Задать список настроек, считанных из сохраненного ранее файла с настройками.
    //! \param settingsList Список настроек.
    virtual void setSettingsList (const QMap<QString, QVariant> &settingsList) = 0;

signals:
    //! \brief Сигнал, информирующий об изменении настроек.
    void settingsChanged ();
};

#endif // ISYSTEMGUICORESETTINGSPANELWIDGET_H
