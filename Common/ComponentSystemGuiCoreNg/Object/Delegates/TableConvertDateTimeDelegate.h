#ifndef TABLECONVERTDATETIMEDELEGATE_H
#define TABLECONVERTDATETIMEDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOptionViewItem>
#include <QtCore/QModelIndex>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QDateTime>

#include <IObject/ISystemGuiCore/ISystemGuiCoreStyleItemDelegate.h>

//!
//! \file TableConvertDateTimeDelegate.h
//! \class TableConvertDateTimeDelegate
//! \brief Класс делегата по конвертации даты-времени
//!
class TableConvertDateTimeDelegate : public ISystemGuiCoreStyleItemDelegate
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Класс-родитель
    explicit TableConvertDateTimeDelegate (QObject* parent = nullptr);

    //! \brief Деструктор класса
    virtual ~TableConvertDateTimeDelegate ();

    //! \brief Метод отрисовки
    //! \param painter QPainter
    //! \param option QStyleOptionViewItem
    //! \param index Индекс
    void paint (QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const;

    //! \brief Метод получения размера ячейки
    //! \param option QStyleOptionViewItem
    //! \param index Индекс
    //! \return Размер ячейки
    QSize sizeHint (const QStyleOptionViewItem &option,
                    const QModelIndex &index ) const;

//signals:
    // \brief Сигнал об изменении данных в делегате.
    //        Введен для возможности перерисовки таблицы с учетом измененных данных
    //void updateTableData ();
};

#endif // TABLECONVERTDATETIMEDELEGATE_H
