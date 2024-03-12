#include "transhubwingow.h"
#include <QDebug>
void TransHubWindow::writeSettings()
{
    QSettings settings("Software Inc.", "texxtEditor");

    //settings.beginGroup("mainWindow");
    qDebug() << "write!";
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());


    settings.endArray();
}

void TransHubWindow::readSettings()
{
    QSettings settings("Software Inc.", "texxtEditor");
    qDebug() << "read!";
    //settings.beginGroup("mainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    //settings.endArray();
}
