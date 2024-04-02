#ifndef GUIWIDGETSMANAGER_H
#define GUIWIDGETSMANAGER_H

#include <QHash>
#include <QPoint>
#include <QObject>
#include <experimental/optional>

class QWidget;
class GuiWidgetsManager : public QObject
{
    Q_OBJECT
public:
    using GWSettings = std::experimental::optional<QPair<QByteArray, QPoint>>;

    GuiWidgetsManager( QObject * = nullptr );
    ~GuiWidgetsManager();

    QWidget *GetWidget( const QString & );
    GWSettings GetWidgetSettings( const QString & );

public slots:
    void OnCloseGWidget( const QString & );
    void OnSaveGWSettings( const QString &, GWSettings );

private:
    QWidget * GetNew( const QString & );

    QHash<QString, QPair<QWidget *, GWSettings>> _gui_widgets;
};


#endif // GUIWIDGETSMANAGER_H
