#ifndef SYSTEMGUICORETOOLBAR_H
#define SYSTEMGUICORETOOLBAR_H

#include <QtWidgets/QAction>
#include <QtWidgets/QToolBar>
#include <QtCore/QMutex>
#include <IObject/ISystemGuiCore/ISystemGuiCoreToolBar.h>

//!
//! \file SystemGuiCoreToolBar.h
//! \class SystemGuiCoreToolBar
//! \brief Базовый класс панели инструментов
//!
class SystemGuiCoreToolBar : public QToolBar, public ISystemGuiCoreToolBar
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param title Заголовок
    //! \param parent Виджет-родитель
    explicit SystemGuiCoreToolBar (const QString &title, QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~SystemGuiCoreToolBar();

    //! \brief Запросить заголовок QToolBar-а
    //! \return Заголовок
    QString getTitle() const;

    // - ISystemGuiCoreToolBar

    //! \brief Добавить QAction на панель инструментов
    //! \param actionName Название
    //! \param actionIcon Иконка
    //! \param toolTip Всплывающая подсказка
    //! \param parent Родитель (если используется тип соединения UseConnect, то родитель должен быть ISystemGuiCoreParentWidget!)
    //! \param connectType Тип соединения
    //! \return QAction
    QAction* addToolBarAction (const QString &actionName,
                               const QIcon &actionIcon,
                               const QString &toolTip = QString(""),
                               QObject *parent = nullptr,
                               ISystemGuiCoreToolBar::ConnectType connectType = ISystemGuiCoreToolBar::UseConnect) override;

    //! \brief Добавить QAction на панель инструментов
    //! \param toolBarAction QAction
    //! \param connectType Тип соединения
    //! \return QAction
    QAction* addToolBarAction (QAction* toolBarAction,
                               ISystemGuiCoreToolBar::ConnectType connectType = ISystemGuiCoreToolBar::UseConnect) override;

    //! \brief Добавить виджет на панель инструментов
    //! \param widget Виджет
    //! \return QAction
    QAction* addToolBarWidget (QWidget* widget) override;

    //! \brief Очистка тулбара.
    void clear () override;

    //! \brief Установка стиля.
    //! \param toolButtonStyle Стиль.
    void setToolButtonStyle (Qt::ToolButtonStyle toolButtonStyle) override;

protected:
    QString _title; //!< заголовок
    QMutex  _mutex; //!< мьютекс
};

#endif // SYSTEMGUICORETOOLBAR_H
