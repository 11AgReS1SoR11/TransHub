#ifndef ISYSTEMGUICORETOOLBAR_H
#define ISYSTEMGUICORETOOLBAR_H

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QWidget>

//!
//! \file ISystemGuiCoreToolBar.h
//! \class ISystemGuiCoreToolBar
//! \brief Базовый класс панели инструментов
//! \author Иванов Антон, Горбачёв Иван
//!
class ISystemGuiCoreToolBar
{
public:
    //!
    //! \brief Перечисление типов соединений для добавляемых QAction-ов
    //!
    enum ConnectType {
        UseConnect = 0, //!< Будет использован дефолтный коннект в ISystemGuiCoreMainWindow
        NotUseConnect   //!< Отслеживание изменения состояния QAction-а возложена на разработчика компонента
    };

    //! \brief Деструктор класса
    virtual ~ISystemGuiCoreToolBar () {}

    //! \brief Добавить QAction на панель инструментов
    //! \param actionName Название
    //! \param actionIcon Иконка
    //! \param toolTip Всплывающая подсказка
    //! \param parent Родитель (если используется тип соединения UseConnect, то родитель должен быть ISystemGuiCoreParentWidget!)
    //! \param connectType Тип соединения
    //! \return QAction
    virtual QAction *addToolBarAction (const QString &actionName,
                                       const QIcon &actionIcon,
                                       const QString &toolTip = QString (),
                                       QObject *parent = nullptr,
                                       ConnectType connectType = UseConnect) = 0;

    //! \brief Добавить QAction на панель инструментов
    //! \param toolBarAction QAction
    //! \param connectType Тип соединения
    //! \return QAction
    virtual QAction *addToolBarAction (QAction *toolBarAction, ConnectType connectType = UseConnect) = 0;

    //! \brief Добавить виджет на панель инструментов
    //! \param widget Виджет
    //! \return QAction
    virtual QAction* addToolBarWidget (QWidget *widget) = 0;

    //! \brief Очистка тулбара.
    virtual void clear () = 0;

    //! \brief Установка стиля.
    //! \param toolButtonStyle Стиль.
    virtual void setToolButtonStyle (Qt::ToolButtonStyle toolButtonStyle) = 0;
};

#endif // ISYSTEMGUICORETOOLBAR_H
