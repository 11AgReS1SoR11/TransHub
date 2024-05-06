#include "StatusBarTabContainer.h"

StatusBarTabContainer::StatusBarTabContainer( QWidget * parent )
    : QWidget{ parent }
{
    tabwsHL = new QHBoxLayout;
    tabwsHL->setContentsMargins(0, 0, 0, 0);
    tabwsHL->setSpacing(10);
    tabwsHL->setAlignment(Qt::AlignRight);
    setLayout( tabwsHL );
}

void StatusBarTabContainer::addTab( ISystemGuiCoreStatusBarTabWidget * tabw )
{
    tabw->setParent( this );
    tabws.append( tabw );
    tabwsHL->addWidget( tabw );
    connect( tabw, &ISystemGuiCoreStatusBarTabWidget::TabClosed, this, &StatusBarTabContainer::removeTab );
}

void StatusBarTabContainer::ShowAll()
{
    for (int i = 0; i < tabwsHL->count(); ++i) {
        if ( ISystemGuiCoreStatusBarTabWidget * sbtw = dynamic_cast<ISystemGuiCoreStatusBarTabWidget *>(tabwsHL->itemAt(i)->widget()) )
            emit sbtw->TabClicked(true);
    }
}

void StatusBarTabContainer::removeTab()
{
    ISystemGuiCoreStatusBarTabWidget * removal_tab = dynamic_cast<ISystemGuiCoreStatusBarTabWidget *>( sender() );
    if ( !removal_tab )
        return;

    int index = tabws.indexOf( removal_tab );
    if (index != -1)
    {
        tabwsHL->removeWidget( removal_tab );
        tabws.removeAt( index );
        removal_tab->deleteLater();
    }
}
