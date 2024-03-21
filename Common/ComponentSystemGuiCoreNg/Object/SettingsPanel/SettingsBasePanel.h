#ifndef SETTINGSBASEPANEL_H
#define SETTINGSBASEPANEL_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreSettingsPanelWidget.h>

#include "../BasePanel/BasePanelDockWidget.h"
#include "../BasePanel/BasePanelWidget.h"

#include <QtCore/QString>
#include <QtCore/QMap>

#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>

namespace Ui {
    class SettingsBasePanel;
}

//!
//! \file SettingsBasePanel.h
//! \class SettingsBasePanel
//! \brief Класс виджета настроек панелей
//!
class SettingsBasePanel : public ISystemGuiCoreSettingsPanelWidget
{
    Q_OBJECT

public:
    //!
    //! \brief Конструктор класса
    //! \param basePanelDockWidget Объект виджета базовой информационной панели
    //! \param basePanelWidget Объект информационной панели
    //! \param parent Виджет-родитель
    //!
    explicit SettingsBasePanel(BasePanelDockWidget *basePanelDockWidget,
                               BasePanelWidget *basePanelWidget,
                               QWidget *parent = 0);

    //!
    //! \brief Деструктор класса
    //!
    virtual ~SettingsBasePanel();

    //!
    //! \brief Инициализация Gui панели
    //! \return true / false
    //!
    bool initGui();

    //!
    //! \brief Название раздела с настройками
    //! \return Название
    //!
    virtual QString settingsTitle() const;

    //!
    //! \brief Иконка раздела с настройками
    //! \return Иконка
    //!
    virtual QIcon settingsIcon() const;

    //!
    //! \brief Корректно ли внесены все настройки
    //! \return true / false
    //!
    virtual bool isComplete() const;

    //!
    //! \brief Применить настройки
    //!
    virtual void applySettings();

    //!
    //! \brief Отменить внесенные изменения и вернуть настройки в предыдущее состояние
    //!
    virtual void cancelSettings();

    //!
    //! \brief Список настроек для сохранения
    //! \return QMap<Название настройки, Значение>
    //!
    virtual QMap<QString, QVariant> settingsList();

    //!
    //! \brief Задать список настроек, считанных из сохраненного ранее файла с настройками
    //! \param settingsList Список настроек
    //!
    virtual void setSettingsList (const QMap<QString, QVariant> &settingsList);

private:
    Ui::SettingsBasePanel   *ui;                    /**< объект графического интерфейса */
    BasePanelDockWidget     *_basePanelDockWidget;  /**< объект виджета базовой информационной панели */
    BasePanelWidget         *_basePanelWidget;      /**< объект информационной панели */

    QString _title; /**< название */
    QIcon   _icon;  /**< иконка */
    int     _size;  /**< размер */

    ISystemGuiCoreBasePanel::BasePanelTabPosition _tabsPos; /**< положение вкладок на момент загрузки Gui */

    QMap<QString, QCheckBox*> _allTabs; /**< список вкладок */
    QMap<QString, QVariant> _lastState; /**< предыдущее состояние */

    //!
    //! \brief Функция разворачивания иконки на 90 градусов
    //! \param icon Иконка
    //! \param angle Угол разворота
    //! \return Развернутая иконка
    //!
    QIcon rotateIcon(QIcon icon, int angle = -90);

    //!
    //! \brief Сохранить предыдущее состояние
    //!
    void saveLastState();

    //!
    //! \brief Корректно ли внесены все настройки
    //! \return true / false
    //!
    bool isCompletePrivate() const;

    //!
    //! \brief Получить положение вкладок
    //! \return Положение вкладок
    //!
    ISystemGuiCoreBasePanel::BasePanelTabPosition tabPos();

protected slots:
    //!
    //! \brief Слот обработки ввода данных
    //! \param isChecked true / false
    //!
    void onCheck(bool isChecked);

    //!
    //! \brief Слот обработки выбора положения вкладок
    //! \param text Текст
    //!
    void onTapsPosChanged(const QString &text);
};

#endif // SETTINGSBASEPANEL_H
