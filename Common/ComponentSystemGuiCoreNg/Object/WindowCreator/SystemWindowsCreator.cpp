#include "SystemWindowsCreator.h"
#include "../MainWindowDefines.h"
#include <QtCore/QDebug>

SystemWindowsCreator::SystemWindowsCreator (SettingsPanelContainer *panelContainer,
                                            ISystemIconManager *iconManager,
                                            const QString &windowTitle)
    : _windowTitle (windowTitle)
{
    _iconManager = iconManager;
    _panelContainer = panelContainer;
}

SystemWindowsCreator::~SystemWindowsCreator ()
{}

QWidget* SystemWindowsCreator::getWidget (const QString &actionName, const QString &actionSignature,
                                          WidgetType &/*type*/, WidgetShowType &/*showType*/)
{
    if (actionName == tr ("About ...")) {
        return new AboutSystemDialog (_iconManager->getSystemLogo (QSize (128,128)).pixmap (128,128), _windowTitle);
    }
    if (actionName == tr ("Components status")) {
        return new AboutComponentsWidget ();
    }
    if (actionName == tr ("Options")) {
        return new SettingsPanel (_panelContainer);
    }

    qDebug () << qPrintable (QString ("[SystemWindowsCreator][getWidget] Receive undefined new widget request. Action name: '%1', Action signature: '%2'")
                             .arg (actionName).arg (actionSignature));

    return nullptr;
}
