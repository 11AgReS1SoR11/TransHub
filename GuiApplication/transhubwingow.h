#ifndef TRANSHUBWINGOW_H
#define TRANSHUBWINGOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDockWidget>
#include <QSettings>
#include <QTableWidget>
#include <objects/map_screen.h>
#include <objects/RightToolBar/righttoolbar.h>
#include <objects/TopToolBar/toptoolbar.h>


class TransHubWindow: public QMainWindow {
    Q_OBJECT
public:
    explicit TransHubWindow(QWidget *parent = 0);
    ~TransHubWindow();
    void writeSettings();
    void readSettings();
private:
    MapScreen* map_screen { nullptr };
    QAction* newFileAction { nullptr };
    QAction* openFileAction { nullptr };
    QAction* saveFileAction { nullptr };
    QAction* saveAsFileAction { nullptr };
    QAction* exitProgramAction { nullptr };
    RightToolBar* fileToolBar { nullptr };
    TopToolBar* topToolBar { nullptr };
    QDockWidget* dockWidget { nullptr };
    QTableWidget* tableWidget { nullptr };

    QMenu* fileMenu { nullptr };

    void elementsPrograms();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void exitProgram();
};


#endif // TRANSHUBWINGOW_H

