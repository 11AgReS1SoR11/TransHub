#ifndef TRANSHUBWINGOW_H
#define TRANSHUBWINGOW_H

#include <objects/leftscreen.h>
#include <objects/rightscreen.h>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDockWidget>
#include <QSettings>

class TransHubWindow: public QMainWindow {
    Q_OBJECT
public:
    explicit TransHubWindow(QWidget *parent = 0);
    ~TransHubWindow();

private:
    LeftScreen* left_screen { nullptr };
    QAction* newFileAction { nullptr };
    QAction* openFileAction { nullptr };
    QAction* saveFileAction { nullptr };
    QAction* saveAsFileAction { nullptr };
    QAction* exitProgramAction { nullptr };
    QToolBar* fileToolBar { nullptr };
    QToolBar* topToolBar { nullptr };
    QComboBox* comboBox_1 { nullptr };
    QComboBox* comboBox_2 { nullptr };
    QCheckBox* checkBox_1 { nullptr };
    QDockWidget* dockWidget { nullptr };
    QTableWidget *tableWidget { nullptr };
    QPushButton* pushButton { nullptr };

    QMenu* fileMenu { nullptr };

    void elementsPrograms();
    void writeSettins();
    void readSettings();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void exitProgram();
};


#endif // TRANSHUBWINGOW_H

