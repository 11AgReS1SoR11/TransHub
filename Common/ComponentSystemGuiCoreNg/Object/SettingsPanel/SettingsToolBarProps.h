#ifndef SETTINGSTOOLBARPROPS_H
#define SETTINGSTOOLBARPROPS_H

#include "../ToolBar/SystemGuiCoreToolBar.h"
#include <QtWidgets/QWidget>

namespace Ui {
    class SettingsToolBarProps;
}

//!
//! \file SettingsToolBarProps.h
//! \class SettingsToolBarProps
//! \brief Класс виджета свойств панели инструментов
//!
class SettingsToolBarProps : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param toolBar Панель инструментов
    //! \param parent Виджет-родитель
    explicit SettingsToolBarProps (SystemGuiCoreToolBar *toolBar, QWidget *parent = nullptr);

    //! \brief Деструтор класса
    virtual ~SettingsToolBarProps ();

    //! \brief Получить заголовок панели
    //! \return Заголовок панели
    QString toolbarTitle () const;

    //! \brief Используется ли панель
    //! \return true / false
    bool isToolbarUsed () const;

    //! \brief Разрешено ли перемещать панель
    //! \return true / false
    bool isToolbarMovable () const;

    //! \brief Отображение текста под иконками.
    //! \return true/false
    bool isToolBarShowText () const;

    //! \brief Задать иконку
    //! \param icon Иконка
    void setIcon (const QIcon &icon);

    //! \brief Задать, что панель используется
    //! \param isUsed true / false
    void setToolbarUsed (bool isUsed);

    //! \brief Задать, что панель можно перемещать
    //! \param isMovable true / false
    void setToolbarMovable (bool isMovable);

    //! \brief Установка свойства отображения текста под иконками.
    //! \param value true/false
    void setToolBarShowText (bool value);

    //! \brief Корректно ли внесены все настройки
    //! \return true / false
    bool isComplete () const;

    //! \brief Отменить настройки
    void cancelSettings ();

    //! \brief Применить настройки
    void applySettings ();

signals:
    //! \brief Сигнал, информирующий об изменении настроек
    void settingsChanged ();

protected slots:
    //! \brief Слот обработки ввода данных
    //! \param isChecked true / false
    void onCheckGroup (bool isChecked);

    //! \brief Слот обработки ввода данных
    //! \param isChecked true / false
    void onCheck (bool isChecked);

private:
    Ui::SettingsToolBarProps *ui {nullptr};   //!< объект графического интерфейса
    SystemGuiCoreToolBar *_toolBar {nullptr}; //!< панель инструментов

    QMap<QString, bool> _lastState; //!< последнее состояние

    //! \brief Сохранить последнее состояние
    void saveLastState ();
};

#endif // SETTINGSTOOLBARPROPS_H
