#ifndef TRANSHUBWINGOW_H
#define TRANSHUBWINGOW_H

#include <objects/leftscreen.h>
#include <objects/rightscreen.h>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

class TransHubWindow: public QMainWindow {
    Q_OBJECT
public:
    explicit TransHubWindow(QWidget *parent = 0);
};

#endif // TRANSHUBWINGOW_H

