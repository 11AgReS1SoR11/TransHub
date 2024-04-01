#ifndef QCUSTOMMDISUBWINDOW_H
#define QCUSTOMMDISUBWINDOW_H

#include <QMdiSubWindow>
#include <experimental/optional>

class CustomMdiSubWindow : public QMdiSubWindow {
    Q_OBJECT

public:
    CustomMdiSubWindow(QWidget *parent = nullptr) : QMdiSubWindow(parent) {}

protected:
    void hideEvent(QHideEvent * ) override;

signals:
    void SaveGWSettings( const QString &, std::experimental::optional<QByteArray> );
    void CloseGWidget( const QString & );
    void HideSubWindow();
};

#endif // QCUSTOMMDISUBWINDOW_H
