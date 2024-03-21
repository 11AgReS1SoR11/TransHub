#ifndef MAINWINDOWCONNECTOR_H
#define MAINWINDOWCONNECTOR_H

#include <IObject/ISystemGuiCore/IMainWindowConnector.h>

//!
//! \file MainWindowConnector.h
//! \class MainWindowConnector
//! \brief Базовый класс коннектора для MainWindow
//!
class MainWindowConnector : public IMainWindowConnector
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Класс-родитель
    explicit MainWindowConnector (QObject* parent)
        : IMainWindowConnector (parent) {}

    //! \brief Деструктор класса
    virtual ~MainWindowConnector () {}
};

#endif // MAINWINDOWCONNECTOR_H
