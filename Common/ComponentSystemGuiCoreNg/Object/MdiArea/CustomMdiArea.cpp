#include "CustomMdiArea.h"
#include "CustomMdiSubWindow.h"

CustomMdiArea::CustomMdiArea( QWidget *parent )
    : QMdiArea(parent)
{
}

void CustomMdiArea::OnHideSubWindow()
{
    if ( auto * mdisw = qobject_cast<CustomMdiSubWindow *>( sender() ) )
    {
        removeSubWindow( mdisw );
        QWidget * swt = mdisw->widget();
        swt->setParent(nullptr);
        QByteArray mdisw_state = mdisw->saveGeometry();
        emit mdisw->SaveGWSettings( swt->objectName(), mdisw_state );
        mdisw->deleteLater();
        swt->hide();
    }
}

void CustomMdiArea::OnCloseSubWindow()
{
    if ( auto * mdisw = qobject_cast<CustomMdiSubWindow *>( sender() ) )
    {
        removeSubWindow( mdisw );
        QWidget * swt = mdisw->widget();
        swt->setParent(nullptr);
        emit mdisw->CloseGWidget( swt->objectName() );
        mdisw->deleteLater();
        swt->hide();
    }
}

