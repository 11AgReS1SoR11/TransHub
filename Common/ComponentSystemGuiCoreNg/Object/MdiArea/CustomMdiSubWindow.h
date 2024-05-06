#ifndef QCUSTOMMDISUBWINDOW_H
#define QCUSTOMMDISUBWINDOW_H

#include <QMdiSubWindow>
#include <experimental/optional>

#include "ComponentSystemGuiCoreNg/ComponentSystemGuiCoreNg_global.h"

//!
//! \brief Кастомный класс MdiArea
//! \author Кромачев Тимофей
//!

class COMPONENTSYSTEMGUICORENGSHARED_EXPORT CustomMdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    //! \brief Конструктор
    //! \param parent родительский объект
    CustomMdiSubWindow(QWidget *parent = nullptr);

    //! \brief Свернуть окно
    void TabHide();

protected:
    void hideEvent(QHideEvent * ) override;
    void closeEvent(QCloseEvent * ) override;
    void resizeEvent(QResizeEvent* event) override;

signals:
    void HideSubWindow();
    void CloseSubWindow();
    //void DeleteMdiSubWindow();
};

#endif // QCUSTOMMDISUBWINDOW_H
