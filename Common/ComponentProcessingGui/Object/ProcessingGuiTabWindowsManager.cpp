#include "ProcessingGuiTabWindowsManager.h"
#include "MapWidget.h"
#include "PlanningWidget.h"
#include "ComponentSystemGuiCoreNg/Object/MainWindowDefines.h"

ProcessingGuiTabWindowsManager::ProcessingGuiTabWindowsManager( QObject * parent )
    : QObject( parent )
{
    QString gwkey = tr( "Планирование" );
    _gui_widgets[ gwkey ] = nullptr;
    gwkey = tr( "Карта" );
    _gui_widgets[ gwkey ] = nullptr;
}

ProcessingGuiTabWindowsManager::~ProcessingGuiTabWindowsManager()
{
    for ( auto it = _gui_widgets.begin(); it != _gui_widgets.end(); ++it )
        it.value()->deleteLater();
}

ISystemGuiCoreStatusBarTabWindow * ProcessingGuiTabWindowsManager::GetWidget( const QString & gwkey )
{
    if ( _gui_widgets.contains( tr( gwkey.toStdString().c_str() ) ) )
    {
        if ( !_gui_widgets[ gwkey ] )
            _gui_widgets[ gwkey ] = GetNew( gwkey );

        return _gui_widgets[ gwkey ];
    }

    return nullptr;
}

bool ProcessingGuiTabWindowsManager::Contained( const QString & gwkey ) const
{
    return ( _gui_widgets[ gwkey ] != nullptr );
}

void ProcessingGuiTabWindowsManager::OnCloseGWidget( const QString & gwkey )
{
    if ( _gui_widgets.contains( gwkey ) )
    {
        _gui_widgets[ gwkey ]->deleteLater();
        _gui_widgets[ gwkey ] = nullptr;
    }
}

void ProcessingGuiTabWindowsManager::OnSaveGWSettings( const QString & gwkey, TabWindowSettings gw_sett )
{
    if ( _gui_widgets.contains( gwkey ) )
    {
        _gui_widgets[ gwkey ]->setProperty( MDI_GEOMETRY , gw_sett.value().first );
        _gui_widgets[ gwkey ]->setProperty( MDI_POSITION , gw_sett.value().second );
    }
}

ISystemGuiCoreStatusBarTabWindow * ProcessingGuiTabWindowsManager::GetNew( const QString & gwkey )
{
    ISystemGuiCoreStatusBarTabWindow * new_widget = nullptr;

    if ( gwkey == tr("Планирование") )
        new_widget = new PlanningWidget;

    if( gwkey == tr("Карта") )
        new_widget = new MapWidget;

    if ( new_widget )
    {
        new_widget->setAttribute (Qt::WA_DeleteOnClose);
        new_widget->setObjectName( gwkey );
    }

    return new_widget;
}

