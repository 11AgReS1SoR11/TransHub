#ifndef TOPTOOLBAR_H
#define TOPTOOLBAR_H

#include <QToolBar>
#include <QComboBox>

class TopToolBar: public QToolBar
{
public:
    explicit TopToolBar(QToolBar* parent = nullptr);
    explicit TopToolBar(const QString &title, QWidget *parent = nullptr);
    ~TopToolBar();

private:
    QComboBox* cars { nullptr };
    QComboBox* products  { nullptr };

};

#endif // TOPTOOLBAR_H
