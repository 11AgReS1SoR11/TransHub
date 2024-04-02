#include "CustomMdiArea.h"
#include "CustomMdiSubWindow.h"

CustomMdiArea::CustomMdiArea( QWidget *parent )
    : QMdiArea(parent)
{
}

void CustomMdiArea::OnHideSubWindow()
{
    if ( auto * mdisw = dynamic_cast<CustomMdiSubWindow *>( sender() ) )
    {
        QWidget * swt = mdisw->widget();
        swt->setParent(nullptr);
        QByteArray mdisw_state = mdisw->saveGeometry();
        QPoint mdisw_pos = mdisw->pos();
        removeSubWindow( mdisw );
        emit mdisw->SaveGWSettings( swt->objectName(), QPair( mdisw_state, mdisw_pos ) );
        mdisw->deleteLater();
        swt->hide();
    }
}

void CustomMdiArea::OnCloseSubWindow()
{
    if ( auto * mdisw = dynamic_cast<CustomMdiSubWindow *>( sender() ) )
    {
        removeSubWindow( mdisw );
        QWidget * swt = mdisw->widget();
        swt->setParent(nullptr);
        emit mdisw->CloseGWidget( swt->objectName() );
        mdisw->deleteLater();
        swt->hide();
    }
}

