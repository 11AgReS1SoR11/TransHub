#include "CustomMdiArea.h"
#include "CustomMdiSubWindow.h"
#include "ISystemGuiCoreStatusBarTabWindow.h"
#include "../MainWindowDefines.h"
#include <QVariant>

CustomMdiArea::CustomMdiArea( QWidget *parent )
    : QMdiArea(parent)
{
}

void CustomMdiArea::OnHideSubWindow()
{
    if ( auto * mdisw = dynamic_cast<CustomMdiSubWindow *>( sender() ) )
    {
        ISystemGuiCoreStatusBarTabWindow * tabw = dynamic_cast<ISystemGuiCoreStatusBarTabWindow *> ( mdisw->widget() );
        Q_ASSERT( tabw );

        tabw->setParent(nullptr);
        QByteArray mdisw_state = mdisw->saveGeometry();
        QPoint mdisw_pos = mdisw->pos();
//        tabw->setProperty( MDI_POSITION, mdisw_state );
//        tabw->setProperty( MDI_GEOMETRY, mdisw_pos );
        removeSubWindow( mdisw );
        emit tabw->SaveTabWindowSettings( tabw->objectName(), QPair( mdisw_state, mdisw_pos ) );
        emit tabw->HideTabWindow();
        mdisw->deleteLater();
        tabw->hide();
    }
}

void CustomMdiArea::OnCloseSubWindow()
{
    if ( auto * mdisw = dynamic_cast<CustomMdiSubWindow *>( sender() ) )
    {
        ISystemGuiCoreStatusBarTabWindow * tabw = dynamic_cast<ISystemGuiCoreStatusBarTabWindow *> ( mdisw->widget() );
        Q_ASSERT( tabw );

        tabw->setParent(nullptr);
        removeSubWindow( mdisw );
        emit tabw->CloseTabWindow( tabw->objectName() );

        mdisw->deleteLater();
        tabw->hide();
    }
}

void CustomMdiArea::OnOpenSubWindow()
{
    auto * senderWidget = qobject_cast<QWidget *>( sender() );
    Q_ASSERT( senderWidget );
    ISystemGuiCoreStatusBarTabWindow * tabw = dynamic_cast<ISystemGuiCoreStatusBarTabWindow *> ( senderWidget );
    Q_ASSERT( tabw );

    CustomMdiSubWindow * subWindow = new CustomMdiSubWindow;
    subWindow->setWidget( tabw );

    addSubWindow( subWindow );

    connect( (CustomMdiSubWindow *)subWindow, &CustomMdiSubWindow::HideSubWindow,
             this, &CustomMdiArea::OnHideSubWindow );
    connect( (CustomMdiSubWindow *)subWindow, &CustomMdiSubWindow::CloseSubWindow,
             this, &CustomMdiArea::OnCloseSubWindow );

    tabw->show();
    tabw->setVisible(true);

    if ( tabw->property(MDI_POSITION).isValid() )
        subWindow->move( tabw->property(MDI_POSITION).toPoint() );

    if ( tabw->property(MDI_GEOMETRY).isValid() )
        subWindow->restoreGeometry( tabw->property(MDI_GEOMETRY).toByteArray() );
    else
        subWindow->showMaximized();
}

