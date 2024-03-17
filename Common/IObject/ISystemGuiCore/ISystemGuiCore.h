#ifndef ISYSTEMGUICORE_H
#define ISYSTEMGUICORE_H

#include <memory>

#include <QtWidgets/QMainWindow>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtWidgets/QSystemTrayIcon>

#include "ISystemGuiCoreMainWindow.h"
#include "IFloatingWidget.h"
#include "ISystemGuiCoreParentWidget.h"
#include "ISystemIconManager.h"
#include "IDelegateManager.h"
#include "IHelpDesigner.h"
#include "ISystemGuiCoreSettings.h"

//!
//! \file ISystemGuiCore.h
//! \class ISystemGuiCore
//! \brief Базовый класс графического ядра системы
//! \author Иванов Антон
//!
class ISystemGuiCore : public QObject
{
    Q_OBJECT

public:
    enum UserMode {
        USER_MODE = 0,
        ADMIN_MODE,
        DEVELOPER_MODE,
        UNKNOWN_MODE = 0xffff
    };

    //! \brief Конструктор класса
    //! \param parent Родительский объект
    explicit ISystemGuiCore (QObject *parent = nullptr)
        : QObject (parent)
    {
        setObjectName ("ISystemGuiCore");
    }
    //! \brief Деструктор
    virtual ~ISystemGuiCore () {}

    //! \brief Запросить список сформированных основных окон.
    //! \return Список основных окон
    virtual QVector<ISystemGuiCoreMainWindow*> getMainWindows () const = 0;

    //! \brief Запросить основное окно по типо отображаемых данных.
    //!        Если необходимое окно отсутствует, возвращается первое сформированное окно
    //!        или окно, отвечающее за отображение всех типов данных.
    //! \param windowType Тип отображаемых данных, поддерживаемое окном
    //! \return Основное окно
    virtual ISystemGuiCoreMainWindow* getMainWindow (const QString &windowType) = 0;

    //! \brief Зарегистрировать графический компонент.
    //! \param guiComponent Графический компонент
    virtual void registrateGuiComponent (ISystemGuiCoreParentWidget *guiComponent) = 0;

    //! \brief Регистрация менеджера иконок.
    //!        В системе присутствует всегда только один менеджер иконок!
    //! \param iconManager Менеджер иконок
    virtual void registrateIconManager (ISystemIconManager* iconManager) = 0;

    //! \brief Регистрация менеджера делегатов для элементов.
    //! \param delegateManager Менеджер делегатов
    virtual void registrateDelegateManager (IDelegateManager* delegateManager) = 0;

    //! \brief Запросить менеджера иконок.
    //! \return Менеджер иконок
    virtual ISystemIconManager* getIconManager () = 0;

    //! \brief Запросить конструктор файла справки
    //! \return Конструктор файла справки
    virtual IHelpDesigner* getHelpDesigner () = 0;

    //! \brief Запросить логин текущего пользователя.
    //! \return Логин пользователя
    virtual QString getCurrentUserLogin () const = 0;

    //! \brief Создание нового MainWindow
    //! \param mainWindowTypes Доступные типы отображаемых данных
    //! \param windowTitle Заголовок окна
    virtual void addMainWindow (const QVector<QString> &mainWindowTypes, const QString &windowTitle) = 0;

    //! \brief Вернуть режим работы текущего пользователя
    //! \return Режим работы текущего пользователя
    virtual UserMode userMode () = 0;

    //! \brief Вернуть объект системного трея
    //! \return Объект системного трея
    virtual std::shared_ptr<QSystemTrayIcon> systemTrayIcon () = 0;

    //! \brief Вернуть объект настроек компонента.
    //! \return Объект настроек компонента.
    virtual ISystemGuiCoreSettings *settings () = 0;

signals:
    //! \brief Сигнал о начале загрузки графического ядра
    void startInitGui ();

    //! \brief Сигнал об окончании загрузки графического ядра
    void finishInitGui ();
};

#endif // ISYSTEMGUICORE_H
