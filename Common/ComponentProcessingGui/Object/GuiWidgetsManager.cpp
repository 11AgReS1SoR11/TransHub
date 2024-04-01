#include "GuiWidgetsManager.h"
#include "MapWidget.h"
#include "PlanningWidget.h"

GuiWidgetsManager::GuiWidgetsManager( QObject * parent )
    : QObject( parent )
{
    QString gwkey = tr("Planning");
    _gui_widgets[ gwkey ] = { GetNew( gwkey ), std::experimental::nullopt };
    gwkey = tr("Map");
    _gui_widgets[ gwkey ] = { GetNew( gwkey ), std::experimental::nullopt };
}

GuiWidgetsManager::~GuiWidgetsManager()
{
    for ( auto it = _gui_widgets.begin(); it != _gui_widgets.end(); ++it )
        it.value().first->deleteLater();
}

QWidget * GuiWidgetsManager::GetWidget( const QString & gwkey )
{
    if ( _gui_widgets.contains( gwkey ) )
    {
        if ( !_gui_widgets[ gwkey ].first )
        {
            _gui_widgets[ gwkey ].first = GetNew( gwkey );
            _gui_widgets[ gwkey ].second = std::experimental::nullopt;
        }

        return _gui_widgets[ gwkey ].first;
    }

    return nullptr;
}

std::experimental::optional<QByteArray> GuiWidgetsManager::GetWidgetSettings( const QString & gwkey )
{
    if ( _gui_widgets.contains( gwkey ) )
        return _gui_widgets[ gwkey ].second;

    return std::experimental::nullopt;
}

void GuiWidgetsManager::OnCloseGWidget( const QString & gwkey )
{
    if ( _gui_widgets.contains( gwkey ) )
    {
        _gui_widgets[ gwkey ].first->deleteLater();
        _gui_widgets[ gwkey ].first = nullptr;
        _gui_widgets[ gwkey ].second = std::experimental::nullopt;
    }
}

void GuiWidgetsManager::OnSaveGWSettings( const QString & gwkey, std::experimental::optional<QByteArray> gwsett )
{
    if ( _gui_widgets.contains( gwkey ) )
        _gui_widgets[ gwkey ].second = gwsett;
}

QWidget * GuiWidgetsManager::GetNew( const QString & gwkey )
{
    QWidget * new_widget = nullptr;

    if ( gwkey == tr("Planning") )
        new_widget = new PlanningWidget;

    if( gwkey == tr("Map") )
        new_widget = new MapWidget;

    if ( new_widget )
    {
        new_widget->setAttribute (Qt::WA_DeleteOnClose);
        new_widget->setObjectName( gwkey );
    }

    return new_widget;
}

