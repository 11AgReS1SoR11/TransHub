#include <QGuiApplication>
#include <QLocale>
#include <QTranslator>
#include <QApplication>
#include <QWidget>

#include <Application/Application.h>

#include <ComponentsCore5alpha/ComponentManager>
#include <QtCore/QDebug>
#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>
#include <QtCore/QProcess>
#include <QtCore/QArgument>
#include <QtCore/QStringList>
#include <QtCore/QMultiMap>

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#ifdef ENABLE_QT_BREAKPAD
#include <QtBreakpad.h>
#endif

#include <functions.h>
#include <GuiVersion.h>
#include <SplashScreen.h>
#include <LoadDialog.h>
#include <QtWidgets/QFileDialog>
#include <QPalette>

#include <execinfo.h>
#include <signal.h>

bool parseArguments (const QStringList &in, QMultiMap<QString, QString> &arguments);

//!
//! \brief Запуска приложения.
//! \param argc Количество входных параметров.
//! \param argv Входные параметры.
//! \return Результат выполнения.
//!
int main (int argc, char *argv[])
{
    Application app (argc, argv);
    app.setOrganizationName (QObject::tr ("TransHub"));

    QColor backgroundColor = QColor(245, 245, 235);
    QPalette palette;
    palette.setColor(QPalette::Window, backgroundColor);
    QApplication::setPalette(palette);

    QMultiMap<QString, QString> args;
    if (!parseArguments (app.QCoreApplication::arguments (), args)) {
        qCritical () << "[main] Could not parse input arguments";
        return EXIT_FAILURE;
    }

    //-- путь до компонентов
    QStringList components_path = args.values ("--components-path");
    if (components_path.isEmpty ())
        components_path.append (qApp->applicationDirPath () + QDir::separator () + "components");
    qInfo () << "[main] Paths to components set as:" << components_path;


    //-- название приложения
    QString app_name;
    if (args.contains ("--appname"))
    {
        if (args.values ("--appname").count () > 1) {
            qCritical () << "[main] Use only single command 'appname'";
            return EXIT_FAILURE;
        }

        app_name = args.value ("--appname");
        if (app_name.isEmpty ()) {
            qWarning () << "[main] Set application name after with '--appname'";
            return EXIT_FAILURE;
        }
    }
    app.setApplicationName (app_name);

    //-- Путь до каталога файла логов
    QString log_path;
    if (args.contains ("--log-path")) {
        log_path = args.value ("--log-path");
        qInfo () << "[main] Log path set:" << log_path;
    }

    if (args.contains ("--log-to-console"))
        app.setLogToConsole (true);

    app.setMaxLogFilesNum (10);
    app.setLogToFile (true);
    app.setLogsPath (log_path);

    //-- установка кокеда
    QTextCodec::setCodecForLocale (QTextCodec::codecForName ("UTF8"));

    //-- загрузка и инсталяция переводов

    QTranslator qt1LanguageTranslator;
    qt1LanguageTranslator.load(QString("QtLanguage_") + QString("ru_RU"));
    qApp->installTranslator(&qt1LanguageTranslator);

    QTranslator qt2LanguageTranslator;
    qt2LanguageTranslator.load("ComponentProcessingGui.qm");
    qApp->installTranslator(&qt2LanguageTranslator);

    QTranslator qt3LanguageTranslator;
    qt3LanguageTranslator.load("ComponentSystemGuiCoreNg.qm");
    qApp->installTranslator(&qt3LanguageTranslator);

    //-- ищем алтернативные иконки логотипа
    QFileInfoList iconsFiles = loadIcons ();

#ifdef ENABLE_QT_BREAKPAD
    QtSystemExceptionHandler systemExceptionHandler (QCoreApplication::applicationDirPath ());
    QtBreakpad::init (QCoreApplication::applicationDirPath ());
#endif

    using namespace ComponentsCore;
    if(!ComponentManager::instance()->setComponentPaths(components_path, args.contains("--profiling")))
    {
        //return EXIT_FAILURE;
    }
    if(ComponentManager::instance()->components().isEmpty()){
        qCritical () << "[main] No components found";
        //return EXIT_FAILURE;
    }


    ComponentManager::addArguments(args);

    LoadDialog *dialogLoad = new LoadDialog (iconsFiles);
    dialogLoad->setAttribute (Qt::WA_DeleteOnClose);

    SplashScreen *splashScreen = new SplashScreen (iconsFiles, &app);
    splashScreen->setAttribute (Qt::WA_DeleteOnClose);

    QObject::connect (&app, &Application::applicationStarted,
                      dialogLoad, &LoadDialog::execDialog);
    QObject::connect (dialogLoad, &LoadDialog::accepted,
                      splashScreen, &SplashScreen::showAndInit);
    QObject::connect (dialogLoad, &LoadDialog::rejected,
                      &app, &Application::quit);
    QObject::connect (splashScreen, &SplashScreen::loadError,
                      &app, &Application::quit);

    int returnValue = app.exec();


    return returnValue;
}

bool parseArguments (const QStringList &in, QMultiMap<QString, QString> &arguments)
{
    int i = 1;
    while (i < in.size ())
    {
        //-- аргумент
        if (in[i].startsWith ("--"))
        {
            //-- НЕ последний
            if (i + 1 < in.size ())
            {
                auto key = in[i++];
                if (i < in.size() && !in[i].startsWith ("--"))
                {
                    if (arguments.contains (key)) {
                        qCritical () << "[main] Use only single argument:"
                                     << key;
                        return false;
                    }

                    while (i < in.size() && !in[i].startsWith ("--"))
                        arguments.insert (key, in[i++]);
                }
                else
                {
                    if (!arguments.contains (key)) {
                        arguments.insert (key, "");
                    } else {
                        qCritical () << "[main] Use only single argument:"
                                     << key;
                        return false;
                    }
                }
            }
            else
            {
                if (!arguments.contains (in[i])) {
                    arguments.insert (in[i++], "");
                } else {
                    qCritical () << "[main] Use only single argument:"
                                 << in[i];
                    return false;
                }
            }
        }
        else {
            qCritical () << "[main] Incorrect input argument:"
                         << in[i];
            return false;
        }
    }

    return true;
}





