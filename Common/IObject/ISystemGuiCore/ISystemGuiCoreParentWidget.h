#ifndef ISYSTEMGUICOREPARENTWIDGET_H
#define ISYSTEMGUICOREPARENTWIDGET_H

#include <QtWidgets/QWidget>

//!
//! \file ISystemGuiCoreParentWidget.h
//! \class ISystemGuiCoreParentWidget
//!
//! \brief Класс базового графического компонента
//! \author Иванов Антон, Горбачёв Иван
//!
class ISystemGuiCoreParentWidget
{
public:
    //!
    //! \brief Тип окна
    //!
    enum WidgetType {
        MdiType = 0,            //!< Часть MDI
        ExternalMainWindowType, //!< Отдельное QMainWindow
        ExternalWidgetType      //!< Отдельный QDialog
    };

    //! В каком состоянии откроется окно.
    enum WidgetShowType {
        ShowNormal = 0,     //!< Обычное состояние.
        ShowMaximized,      //!< Макисмальное.
        ShowMinimized,      //!< Минимальное.
        ShowFullScreen      //!< Во весь экран.
    };

    //! \brief Деструктор
    virtual ~ISystemGuiCoreParentWidget () {}

    //! \brief Вернуть виджет по имени.
    //! \param actionName Название виджета (название QAction-а, которое его вызвало)
    //! \param actionSignature Сигнатура вызова (полный путь в меню до вызывающего QAction)
    //! \param type Тип виджета \see WidgetType
    //! \param showType Режим открытия.
    //! \return Виджет окна
    virtual QWidget *getWidget (const QString &actionName, const QString &actionSignature,
                                WidgetType &type, WidgetShowType &showType) {
        (void)actionName;
        (void)actionSignature;
        (void)type;
        (void)showType;
        return nullptr;
    }

    //! \brief Инициализация графического интерфейса компонента.
    virtual void initGui () {
        return;
    }
};

#endif // ISYSTEMGUICOREPARENTWIDGET_H
