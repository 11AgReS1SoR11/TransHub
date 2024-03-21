#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QtWidgets/QWidget>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QStackedWidget>
#include <QtGui/QCloseEvent>

#include <QtCore/QList>
#include <QtCore/QPointer>

#include "SettingsPanelContainer.h"

namespace Ui {
class SettingsPanel;
}

//!
//! \file SettingsPanel.h
//! \class SettingsPanel
//! \brief Класс окна настроек
//! \author Ivanov Anton
//!
class SettingsPanel : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param panelContainer Менеджер панелей настроек
    //! \param parent Виджет-родитель
    explicit SettingsPanel (SettingsPanelContainer *panelContainer, QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~SettingsPanel ();

protected:
    //! \brief Слот обработки закрытия окна
    //! \param event QCloseEvent
    void closeEvent (QCloseEvent *event) override;

protected slots:
    //! \brief Слот обработки выбора строки
    //! \param row Строка
    void currentRowChanged (int row);

    //! \brief Слот обработки активации элемента
    //! \param item Элемент
    void itemActivated (QListWidgetItem *item);

    //! \brief Слот обработки ввода данных активного виджета настроек
    void settingsChanged ();

    //! \brief Слот поиска виджета настроек
    //! \param title Заголовок виджета
    void searchSettings (const QString &title);

    //! \brief Слот обработки кнопки "Ок"
    void onOk ();

    //! \brief Слот обработки кнопки "Применить"
    void onApply();

    //! \brief Слот обработки кнопки "Отмена"
    void onCancel();

private:
    Ui::SettingsPanel *ui {nullptr};    //!< объект графического интерфейса
    QPointer<SettingsPanelContainer> _panelContainer;           /**< менеджер панелей настроек */

    QList<ISystemGuiCoreSettingsPanelWidget*> _settingsWidgets; /**< список виджетов настроек */

    //! \brief Инициализация виджета
    void initGui ();

    //! \brief Есть ли хоть один корректно заполненый виджет
    //! \return true / false
    bool isCompleteAtLeastOne ();

    //! \brief Сохранение всех
    void privateOk ();

    //! \brief Сохранение конкретного виджета
    //! \param panelWidget Виджет настроек
    void privateApply (ISystemGuiCoreSettingsPanelWidget *panelWidget);

    //! \brief Отмена изменений
    void privateCancel ();
};

#endif // SETTINGSPANEL_H
