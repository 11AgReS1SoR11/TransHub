#include "Application.h"
#include <ColorConsole/ColorConsoleAppender.h>

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkInterface>

#include <signal.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

// --- Log 4 Qt ---
#include <3rdparty/log4qt/logmanager.h>
#include <3rdparty/log4qt/consoleappender.h>
#include <3rdparty/log4qt/ttcclayout.h>
#include <3rdparty/log4qt/rollingfileappender.h>
#include <3rdparty/log4qt/loggerrepository.h>

//! \brief Поиск ip адреса по названию интерфеса
QHostAddress findIpByInterface (const QString &interface)
{
    auto networkInterfaces = QNetworkInterface::allInterfaces ();
    for (auto const &netInterface : qAsConst (networkInterfaces))
    {
        if (netInterface.name () != interface)
            continue;

        if (!(bool)(netInterface.flags () & QNetworkInterface::IsUp)) {
            qCritical () << "[Application][findIpByInterface] Inteface is not UP" << interface;
            return QHostAddress ();
        }
        if (!(bool)(netInterface.flags () & QNetworkInterface::IsRunning)) {
            qCritical () << "[Application][findIpByInterface] Inteface is not RUNNING" << interface;
            return QHostAddress ();
        }

        auto networkAddresses = netInterface.addressEntries ();
        for (const QNetworkAddressEntry &netAdress : qAsConst (networkAddresses))
            return netAdress.ip ();

        //        qDebug () << "";
        //        qDebug () << "Interface:" << netInterface.name ();
        //        qDebug () << "    isValid:" << netInterface.isValid ();
        //        qDebug () << "    index:" << netInterface.index ();
        //        qDebug () << "    type:" << netInterface.type ();
        //        qDebug () << "    maximumTransmissionUnit:" << netInterface.maximumTransmissionUnit ();
        //        qDebug () << "    humanReadableName:" << netInterface.humanReadableName ();
        //        qDebug () << "    hardwareAddress:" << netInterface.hardwareAddress ();
        //        qDebug () << "    hardwareAddress:" << netInterface.hardwareAddress ();

        //        qDebug () << "    IsUp:" << (bool)(netInterface.flags () & QNetworkInterface::IsUp);
        //        qDebug () << "    IsRunning:" << (bool)(netInterface.flags () & QNetworkInterface::IsRunning);
        //        qDebug () << "    CanBroadcast:" << (bool)(netInterface.flags () & QNetworkInterface::CanBroadcast);
        //        qDebug () << "    IsLoopBack:" << (bool)(netInterface.flags () & QNetworkInterface::IsLoopBack);
        //        qDebug () << "    IsPointToPoint:" << (bool)(netInterface.flags () & QNetworkInterface::IsPointToPoint);
        //        qDebug () << "    CanMulticast:" << (bool)(netInterface.flags () & QNetworkInterface::CanMulticast);

        //        auto networkAddresses = netInterface.addressEntries ();
        //        for (const QNetworkAddressEntry &netAdress : qAsConst (networkAddresses))
        //        {
        //            qDebug () << "";
        //            qDebug () << "        ip:" << netAdress.ip ();
        //            qDebug () << "        netmask:" << netAdress.netmask ();
        //            qDebug () << "        prefixLength:" << netAdress.prefixLength ();
        //            qDebug () << "        broadcast:" << netAdress.broadcast ();
        //            qDebug () << "        isLifetimeKnown:" << netAdress.isLifetimeKnown ();
        //            qDebug () << "        isPermanent:" << netAdress.isPermanent ();
        //            qDebug () << "        isPermanent:" << netAdress.isPermanent ();
        //            qDebug () << "        isTemporary:" << netAdress.isTemporary ();
        //        }
    }

    return QHostAddress ();
}

Application *Application::_instance = nullptr;

Application::Application (int &argc, char **argv, int appFlags)
    : IApplication (argc, argv, appFlags)
{
    _maxFileSize = MAX_FILE_SIZE_MB;
    _maxLogsNum = MAX_LOGS_NUM;

    Application::_instance = this;

    QTimer::singleShot (0, this, &Application::applicationStartLog4Qt);
    connect (this, &Application::applicationStartLog4Qt, this, &Application::slotStart);
    connect (this, &Application::aboutToQuit, this, &Application::stopComponentManager);

    setAttribute (Qt::AA_DontUseNativeDialogs, true);
}

Application::~Application () {
    Application::_instance = nullptr;
}

Application *Application::instance () {
    return Application::_instance;
}

void Application::setLogToConsole (bool value) {
    _logToConsole = value;
}

void Application::setRemoteServerLog (const QString &eth, const QString &ip, unsigned int port)
{
    _udpEth = eth;
    _udpHost = ip;
    _udpPort = port;
}

void Application::setAmqpServerLog (const QString &eth, const QString &host, unsigned int port,
                                    const QString &user, const QString &pass, const QString &exchange)
{
    _amqpEth = eth;
    _amqpHost = host;
    _amqpPort = port;
    _amqpUser = user;
    _amqpPass = pass;
    _amqpExchange = exchange;
}

void Application::setLogToFile (bool value) {
    _logToFile = value;
}

void Application::setMaxLogFileSize (int maxSize) {
    _maxFileSize = maxSize;
}

void Application::setMaxLogFilesNum (int maxNum) {
    _maxLogsNum = maxNum;
}

void Application::setLogsPath (const QString &path) {
    _logPath = path;
}

void Application::slotStart () {
    initLog4Qt ();
}

void Application::initLog4Qt ()
{
    if (!_logToConsole && !_logToFile) {
        emit applicationStarted ();
        return;
    }

    auto logger = Log4Qt::LogManager::rootLogger ();
    Log4Qt::LogManager::setHandleQtMessages (true);

    auto ttccLayout = new Log4Qt::TTCCLayout (Log4Qt::TTCCLayout::ISO8601);
    ttccLayout->setName (QLatin1String ("TTCCLayout"));
    ttccLayout->activateOptions ();

    if (_logToConsole)
    {
        auto consoleAppender = new ColorConsoleAppender
                (ttccLayout, ColorConsoleAppender::STDOUT_TARGET);
        consoleAppender->setName ("ColorConsoleAppender");
        consoleAppender->activateOptions ();
        logger->addAppender (Log4Qt::AppenderSharedPtr (consoleAppender));
    }

//    if (!_udpHost.isEmpty ())
//    {
//        auto lo = findIpByInterface (_udpEth);
//        if (!lo.isNull ())
//        {
//            auto netUdpAppender = new NetworkUdpAppender
//                    (ttccLayout, lo, QHostAddress (_udpHost), _udpPort);
//            netUdpAppender->setName ("NetworkUdpAppender");
//            netUdpAppender->activateOptions ();
//            logger->addAppender (Log4Qt::AppenderSharedPtr (netUdpAppender));
//        }
//        else {
//            qCritical () << "[Application][initLog4Qt] Cannot find ip address for interface:"
//                         << _udpEth;
//        }
//    }

    if (_logToFile)
    {
        QString xdgLogHome;

        if (!_logPath.isEmpty ()) {
            xdgLogHome = _logPath;
            if (!xdgLogHome.endsWith (QDir::separator ()))
                xdgLogHome += QDir::separator ();
        }
        else {
            xdgLogHome = QLatin1String ("../../LogDir/") +
                    QCoreApplication::applicationName () + QDir::separator ();
        }

        QDir logsdir (xdgLogHome);
        if (!logsdir.exists ()) {
            if (logsdir.mkpath (xdgLogHome))
            {
                if (0 != chmod (xdgLogHome.toStdString ().c_str (), S_IRWXG | S_IRWXO | S_IRWXU)) {
                    qCritical () << "[Application][initLog4Qt] Could not change permission for the:"
                                 << xdgLogHome;
                    quit (); return;
                }
            } else {
                qCritical () << "[Application][initLog4Qt] Init Log4Qt failed! Could not create log file! Path:"
                             << xdgLogHome;
                quit (); return;
            }
        }

        //-- create file appender
        _logFileName = QString ("%1.log").arg (applicationName ().toLower ().replace (" ", "_"));
        auto fileAppender = new Log4Qt::RollingFileAppender (ttccLayout,
                                                             xdgLogHome + _logFileName,
                                                             true);

        if (_maxLogsNum > 0)
            fileAppender->setMaxBackupIndex (_maxLogsNum); // default 1

        fileAppender->setName (QLatin1String ("FileAppender"));
        fileAppender->setMaximumFileSize (_maxFileSize * 1024 * 1024); // default 10 MB
        fileAppender->activateOptions ();
        logger->addAppender (fileAppender);
    }

    //-- отправка логов на AMQP сервер
    //    if (!_amqpEth.isEmpty () && !_amqpHost.isEmpty () && _amqpPort != 0
    //            && !_amqpUser.isEmpty () && !_amqpExchange.isEmpty ())
    //    {
    //        auto lo = findIpByInterface (_amqpEth);
    //        if (!lo.isNull ())
    //        {
    //            auto amqpAppender = new AmqpAppender (ttccLayout, lo.toString (), _amqpHost, _amqpPort,
    //                                                  _amqpUser, _amqpPass, _amqpExchange);
    //            amqpAppender->setName (QLatin1String ("AmqpAppender"));
    //            amqpAppender->activateOptions ();
    //            logger->addAppender (amqpAppender);
    //        }
    //        else {
    //            qCritical () << "[Application][initLog4Qt] Cannot find ip address for interface:"
    //                         << _amqpEth;
    //        }
    //    }

    logger->info ("\n");
    logger->info ("\n");
    logger->info ("################################################################################ START");
    logger->info (QString ("Start Gui Application '%1'...").arg (applicationName ()));

    emit applicationStarted ();
}

void Application::stopComponentManager ()
{
    // --- delete all objects
    using namespace ComponentsCore;

    if (ComponentManager::instance ())
        ComponentManager::instance ()->shutdown ();

    ComponentManager::deleteInstance ();
    removeLog4Qt ();
}

void Application::removeLog4Qt ()
{
    auto logger = Log4Qt::Logger::rootLogger ();

    logger->info (QString ("Gui Application '%1' stopped.").arg (applicationName ()));
    logger->info ("################################################################################ STOP");
    logger->removeAllAppenders ();
    logger->loggerRepository ()->shutdown ();
}
