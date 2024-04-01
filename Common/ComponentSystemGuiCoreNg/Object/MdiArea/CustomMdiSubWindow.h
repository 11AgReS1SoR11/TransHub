#ifndef QCUSTOMMDISUBWINDOW_H
#define QCUSTOMMDISUBWINDOW_H

//#include "../ComponentProcessingGui/ComponentProcessingGui_global.h"
#include <QMdiSubWindow>
#include <experimental/optional>

class Q_DECL_IMPORT CustomMdiSubWindow : public QMdiSubWindow {
    Q_OBJECT

public:
    CustomMdiSubWindow(QWidget *parent = nullptr) : QMdiSubWindow(parent) {}

protected:
    void hideEvent(QHideEvent * ) override;
    void closeEvent(QCloseEvent * ) override;

signals:
    void SaveGWSettings( const QString &, std::experimental::optional<QByteArray> );
    void CloseGWidget( const QString & );
    void HideSubWindow();
    void CloseSubWindow();
};

#endif // QCUSTOMMDISUBWINDOW_H
