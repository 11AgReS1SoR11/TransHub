#include "transhubwingow.h"

void TransHubWindow::writeSettings()
{
    QSettings settings("Software Inc.", "texxtEditor");

    //settings.beginGroup("mainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());

    //settings.endArray();
}

void TransHubWindow::readSettings()
{
    QSettings settings("Software Inc.", "texxtEditor");
    //settings.beginGroup("mainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    //settings.endArray();
}
