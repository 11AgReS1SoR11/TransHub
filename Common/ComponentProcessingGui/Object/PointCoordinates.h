#ifndef POINTCOORDINATES_H
#define POINTCOORDINATES_H
#include <QWidget>
#include <QDebug>

class PointCoordinates : public QObject
{
    Q_OBJECT
public:
    explicit PointCoordinates(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void sendMessage(double latitude, double longitude, QString markerType, QString opType)
        {
            emit pointClicked( latitude,  longitude,  markerType,  opType);
        }

signals:
    void pointClicked(double latitude, double longitude, QString markerType, QString opType);
};


#endif // POINTCOORDINATES_H
