#ifndef PROCESSINGGUITABWINDOWSMANAGER_H
#define PROCESSINGGUITABWINDOWSMANAGER_H

#include <QHash>
#include <QPoint>
#include <QObject>
#include <experimental/optional>

#include "ISystemGuiCoreStatusBarTabWindow.h"
#include "ISystemGuiCoreStatusBarTabWidget.h"

//!
//! \brief Класс для управления оконными виджетами оболочки
//! \author Кромачев Тимофей
//!

class ProcessingGuiTabWindowsManager : public QObject
{
    Q_OBJECT
public:
    using TabWindowSettings = std::experimental::optional<QPair<QByteArray, QPoint>>;

    static ProcessingGuiTabWindowsManager & instance() {
        static ProcessingGuiTabWindowsManager instance;
        return instance;
    }

    //! \brief Деструктор класса
    ~ProcessingGuiTabWindowsManager();

    //! \brief Получить виджет
    //! \param gwkey ключ виджета
    //! \return Виджет для сворачиваемого окна
    ISystemGuiCoreStatusBarTabWindow * GetWidget( const QString & );

    //! \brief Содержится ли активный виджет в менеджере
    //! \param gwkey ключ виджета
    //! \return true / false
    bool Contained( const QString & ) const;

public slots: 
    /*!
     * \brief Слот, удаляющий закрытый виджет
     */
    void OnCloseGWidget( const QString & );

    /*!
     * \brief Слот, сохраняющий настройки виджета
     */
    void OnSaveGWSettings( const QString &, TabWindowSettings );

private:
    //! \brief Конструктор класса
    //! \param parent Виджет-родитель
    ProcessingGuiTabWindowsManager( QObject * = nullptr );
    ProcessingGuiTabWindowsManager(const ProcessingGuiTabWindowsManager&) = delete;
    void operator=(const ProcessingGuiTabWindowsManager&) = delete;

    //! \brief Создать новый виджет по ключу
    //! \param gwkey ключ виджета
    //! \return Виджет для сворачиваемого окна
    ISystemGuiCoreStatusBarTabWindow * GetNew( const QString & );

    QHash<QString, ISystemGuiCoreStatusBarTabWindow *> _gui_widgets;    /**< словарь с ключами, виджетами и их настройками */
};


#endif // PROCESSINGGUITABWINDOWSMANAGER_H
