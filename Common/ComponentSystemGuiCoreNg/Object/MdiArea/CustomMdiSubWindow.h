#ifndef QCUSTOMMDISUBWINDOW_H
#define QCUSTOMMDISUBWINDOW_H

#include <QMdiSubWindow>
#include <experimental/optional>
#include "ComponentSystemGuiCoreNg/ComponentSystemGuiCoreNg_global.h"

class CUSTOMMDISUBWINDOWSHARED_EXPORT CustomMdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    CustomMdiSubWindow(QWidget *parent = nullptr);
    ~CustomMdiSubWindow();

protected:
    void hideEvent(QHideEvent * ) override;
    void closeEvent(QCloseEvent * ) override;

signals:
    void SaveGWSettings( const QString &, std::experimental::optional<QPair<QByteArray, QPoint>> );
    void CloseGWidget( const QString & );
    void HideSubWindow();
    void CloseSubWindow();
    void DeleteMdiSubWindow();
};

#endif // QCUSTOMMDISUBWINDOW_H
