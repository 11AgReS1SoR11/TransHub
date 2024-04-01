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
    GuiWidgetsManager( QObject * = nullptr );
    ~GuiWidgetsManager();

    QWidget *GetWidget( const QString & );
    std::experimental::optional<QByteArray> GetWidgetSettings( const QString & );

public slots:
    void OnCloseGWidget( const QString & );
    void OnSaveGWSettings( const QString &, std::experimental::optional<QByteArray> );

private:
    QWidget * GetNew( const QString & );

    QHash<QString, QPair<QWidget *,  std::experimental::optional<QByteArray>>> _gui_widgets;
};


#endif // GUIWIDGETSMANAGER_H
