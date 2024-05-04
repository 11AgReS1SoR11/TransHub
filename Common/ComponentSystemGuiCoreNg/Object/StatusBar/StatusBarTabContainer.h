#ifndef STATUSBARTABCONTAINER_H
#define STATUSBARTABCONTAINER_H

#include <QWidget>
#include <QHBoxLayout>

#include "ISystemGuiCoreStatusBarTabWidget.h"
#include "ComponentSystemGuiCoreNg/ComponentSystemGuiCoreNg_global.h"

//!
//! \brief Класс хранения вкладок в StatusBar
//! \author Кромачев Тимофей
//!

class COMPONENTSYSTEMGUICORENGSHARED_EXPORT StatusBarTabContainer : public QWidget
{
    Q_OBJECT
public:

    //! \brief Конструктор класса
    //! \param parent Виджет-родитель
    explicit StatusBarTabContainer( QWidget * parent = nullptr );

    //! \brief Добавить TabWidget
    //! \param tabw виджет для добавления
    void addTab( ISystemGuiCoreStatusBarTabWidget * tabw );

    //! \brief Показать все скрытые окна
    void ShowAll();

public slots:
    /*!
     * \brief Слот, удаляющий TabWidget
     */
    void removeTab();

private:
    QHBoxLayout * tabwsHL;              /**< layout для TabWidget */
    QList<ISystemGuiCoreStatusBarTabWidget *> tabws;  /**< список содержащихся TabWidget */
};

#endif // STATUSBARTABCONTAINER_H
