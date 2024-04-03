#ifndef CUSTOMMDIAREA_H
#define CUSTOMMDIAREA_H

#include <QMdiArea>

#define CUSTOMMDIAREASHARED_EXPORT Q_DECL_EXPORT

class CUSTOMMDIAREASHARED_EXPORT CustomMdiArea : public QMdiArea
{
    Q_OBJECT
public:
    CustomMdiArea( QWidget *parent = nullptr );

public slots:
    void OnHideSubWindow();
    void OnCloseSubWindow();

};

#endif // CUSTOMMDIAREA_H
