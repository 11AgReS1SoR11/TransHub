#ifndef IMAINWINDOWCONNECTOR_H
#define IMAINWINDOWCONNECTOR_H

#include <QtCore/QObject>

//!
//! \file IMainWindowConnector.h
//! \class IMainWindowConnector
//! \brief Базовый класс коннектора для MainWindow
//!
class IMainWindowConnector : public QObject
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Родительский объект
    explicit IMainWindowConnector (QObject* parent = nullptr)
        : QObject (parent) {}

    //! \brief Деструктор класса
    virtual ~IMainWindowConnector() {}

signals:
    //! \brief Сигнал об открытии окна внутри рабочей области.
    //! \param windowName Название окна (подпись QAction-а, которое его вызвало)
    //!        Пример подписи: Инструменты/Окна/Закрыть активное
    //!        т.е. Инструменты - меню в MenuBar-е
    //!             Окна - подменю
    //!             Закрыть активное - название QAction-а
    //!
    //!        Если QAction распологается на панели инструментов, то перед подписью будет префикс TOOLBAR
    //!        Пример: TOOLBAR Окна/Закрыть активное
    //!        т.е. Окна - название панели инструментов
    //!             Закрыть активное - название QAction-а
    void subWindowOpened (const QString &windowName);

    //! \brief Сигнал о закрытии окна внутри рабочей области.
    //! \param windowName Название окна (подпись QAction-а, которое его вызвало)
    //!        Пример подписи: Инструменты/Окна/Закрыть активное
    //!        т.е. Инструменты - меню в MenuBar-е
    //!             Окна - подменю
    //!             Закрыть активное - название QAction-а
    //!
    //!        Если QAction распологается на панели инструментов, то перед подписью будет префикс TOOLBAR
    //!        Пример: TOOLBAR Окна/Закрыть активное
    //!        т.е. Окна - название панели инструментов
    //!             Закрыть активное - название QAction-а
    void subWindowClosed (const QString &windowName);
};

#endif // IMAINWINDOWCONNECTOR_H
