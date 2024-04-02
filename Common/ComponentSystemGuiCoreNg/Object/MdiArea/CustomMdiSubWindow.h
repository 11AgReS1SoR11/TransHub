#ifndef QCUSTOMMDISUBWINDOW_H
#define QCUSTOMMDISUBWINDOW_H

//#include "../ComponentProcessingGui/ComponentProcessingGui_global.h"
#include <QMdiSubWindow>
#include <experimental/optional>

class Q_DECL_EXPORT CustomMdiSubWindow : public QMdiSubWindow {
    Q_OBJECT

public:
    CustomMdiSubWindow(QWidget *parent = nullptr) : QMdiSubWindow(parent) {}
    ~CustomMdiSubWindow();

protected:
    void hideEvent(QHideEvent * ) override;
    void closeEvent(QCloseEvent * ) override;

signals:
    void SaveGWSettings( const QString &, std::experimental::optional<QByteArray> );
    void CloseGWidget( const QString & );
    void HideSubWindow();
    void CloseSubWindow();
    void DeleteMdiSubWindow();
};

#endif // QCUSTOMMDISUBWINDOW_H
