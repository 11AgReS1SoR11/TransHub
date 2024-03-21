#ifndef GEOCOORDINATEDELEGATE_H
#define GEOCOORDINATEDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>
#include <IObject/ISystemGuiCore/ISystemGuiCoreStyleItemDelegate.h>

//!
//! \file GeoCoordinateDelegate.h
//! \class GeoCoordinateDelegate
//! \brief Делегат для отображения географических координат.
//!     Для указания типа координаты необходимо проставить в делегате свойство <dimension>.
//!     Допустимые значения свойства <dimention>:
//!     Широта - lat, latitude
//!     Долгота - lng, longitude
//!
class GeoCoordinateDelegate : public ISystemGuiCoreStyleItemDelegate
{
    Q_OBJECT

public:
    //! \brief Конструктор
    //! \param parent Родительский объект
    explicit GeoCoordinateDelegate (QObject* parent = nullptr);

    //! \brief Деструктор
    virtual ~GeoCoordinateDelegate ();

    //! \brief Метод отрисовки
    //! \param painter QPainter
    //! \param option QStyleOptionViewItem
    //! \param index Индекс
    void paint (QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const override;

    //! \brief Метод получения размера ячейки
    //! \param option QStyleOptionViewItem
    //! \param index Индекс
    //! \return Размер ячейки
    QSize sizeHint (const QStyleOptionViewItem &option,
                    const QModelIndex &index ) const override;

    //signals:
    // \brief Сигнал, информирующий об обновлении данных
    //void updateTableData ();

protected:
    //! \brief Метод перевода координат
    //! \param index Индекс
    void recalculate (const QModelIndex &index) const;

    //! координаты
    mutable QString _coordinateText;
};

#endif // GEOCOORDINATEDELEGATE_H
