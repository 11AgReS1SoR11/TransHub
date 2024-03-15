#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtCore/QFileInfoList>
#include <QtGui/QRgb>
#include <QCoreApplication>
#include <QApplication>


//!
//! \file Application.h
//! \class Application
//! \brief Класс приложения
//!
class Application : public QApplication
{
    Q_OBJECT

public:

    Application(int argc, char *argv[]) : QApplication(argc, argv){

    }

    void setLogToConsole(bool key = true){

    };

    void setMaxLogFilesNum (int size) {

    }
    void setLogToFile (bool key) {

    }
    void setLogsPath (QString){

    }

    int exec() {
        emit applicationStarted();
        return QApplication::exec();
    }

signals:
    void applicationStarted();


};

#endif // APPLICATION_H
