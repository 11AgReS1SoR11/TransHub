#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>
#include <QApplication>
#include <QWidget>
#include "transhubwingow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "GuiApplication_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    TransHubWindow* window = new TransHubWindow();
    window->readSettings();
//    window->setWindowState(Qt::WindowFullScreen);
//    window->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    window->show();
    return app.exec();
}
