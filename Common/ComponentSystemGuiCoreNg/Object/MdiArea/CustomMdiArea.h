#ifndef CUSTOMMDIAREA_H
#define CUSTOMMDIAREA_H

#include <QMdiArea>
#include "ComponentSystemGuiCoreNg/ComponentSystemGuiCoreNg_global.h"

//!
//! \brief Кастомный класс MdiArea
//! \author Кромачев Тимофей
//!

class COMPONENTSYSTEMGUICORENGSHARED_EXPORT CustomMdiArea : public QMdiArea
{
    Q_OBJECT
public:
    //! \brief Конструктор
    //! \param parent родительский объект
    CustomMdiArea( QWidget *parent = nullptr );

public slots:
    /*!
     * \brief Слот, обрабатывающий скрытие CustomMdiWindow
     */
    void OnHideSubWindow();

    /*!
     * \brief Слот, обрабатывающий закрытие CustomMdiWindow
     */
    void OnCloseSubWindow();

    /*!
     * \brief Слот, обрабатывающий открытие CustomMdiWindow
     */
    void OnOpenSubWindow();

signals:
    void SettingsChanged();
};

#endif // CUSTOMMDIAREA_H
