#include "SystemIconManager.h"
#include <QtCore/QDebug>

SystemIconManager::SystemIconManager () {
    _init = false;
    _debug = true;
}

SystemIconManager::~SystemIconManager ()
{}

void SystemIconManager::initDefaultLogo ()
{
    if (_init)
        return;

    if (!_systemLogo.contains (QString ("16x16")))
        _systemLogo.insert (QString ("16x16"), QIcon (":/Images/Images/phoenix_logo_16.png"));

    if (!_systemLogo.contains (QString ("32x32")))
        _systemLogo.insert (QString ("32x32"), QIcon (":/Images/Images/phoenix_logo_32.png"));

    if (!_systemLogo.contains (QString ("64x64")))
        _systemLogo.insert (QString ("64x64"), QIcon (":/Images/Images/phoenix_logo_64.png"));

    if (!_systemLogo.contains (QString ("128x128")))
        _systemLogo.insert (QString ("128x128"), QIcon (":/Images/Images/phoenix_logo_128.png"));

    _init = true;
}

QIcon SystemIconManager::getIcon_16x16 (const QString &iconName)
{
    Q_UNUSED (iconName);
    return QIcon (":/Images/Images/question-white.png");
}

QIcon SystemIconManager::getIcon_24x24 (const QString &iconName)
{
    Q_UNUSED (iconName);
    return QIcon (":/Images/Images/question-white.png");
}

QIcon SystemIconManager::getIcon_32x32 (const QString &iconName)
{
    Q_UNUSED (iconName);
    return QIcon (":/Images/Images/question-white.png");
}

QIcon SystemIconManager::getSystemLogo (const QSize &iconSize)
{
    initDefaultLogo ();

    QString buffSize = QString ("%1x%2").arg (iconSize.height ()).arg (iconSize.width ());
    if (_systemLogo.contains (buffSize))
        return _systemLogo.value (buffSize);

    return QIcon ();
}

QIcon SystemIconManager::getSystemLogo (int iconWith, int iconHeight)
{
    initDefaultLogo ();

    QString buffSize = QString ("%1x%2").arg (iconHeight).arg (iconWith);
    if (_systemLogo.contains (buffSize))
        return _systemLogo.value (buffSize);

    return QIcon ();
}

void SystemIconManager::setSystemLogo (const QSize &iconSize, const QIcon &icon)
{
    if (iconSize.height () != iconSize.width ()) {
        qWarning () << "[SystemIconManager][setSystemLogo] Logo Icon is not square!";
        return;
    }

    QString buffSize = QString ("%1x%2").arg (iconSize.height ()).arg (iconSize.width ());
    //if (_systemLogo.contains (buffSize) && _debug)
    //    qDebug () << "[SystemIconManager][setSystemLogo] Logo Icon already add! Size:"
    //              << buffSize;

    _systemLogo.insert (buffSize, icon);
}

void SystemIconManager::setSystemLogo (int iconWith, int iconHeight, const QIcon &icon)
{
    if (iconHeight != iconWith) {
        qWarning() << "[SystemIconManager][setSystemLogo] Logo Icon is not square!";
        return;
    }

    QString buffSize = QString ("%1x%2").arg (iconHeight).arg (iconWith);
    //if (_systemLogo.contains (buffSize) && _debug)
    //    qDebug () << "[SystemIconManager][setSystemLogo] Logo Icon already add! Size:"
    //              << buffSize;

    _systemLogo.insert (buffSize, icon);
}
