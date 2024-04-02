#ifndef CUSTOMMDIAREA_H
#define CUSTOMMDIAREA_H

#include <QMdiArea>

class CustomMdiArea : public QMdiArea
{
    Q_OBJECT
public:
    CustomMdiArea( QWidget *parent = nullptr );

public slots:
    void OnHideSubWindow();
    void OnCloseSubWindow();

};

#endif // CUSTOMMDIAREA_H
