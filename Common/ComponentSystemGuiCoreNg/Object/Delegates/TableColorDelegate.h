#ifndef TABLECOLORDELEGATE_H
#define TABLECOLORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOptionViewItem>
#include <QtCore/QModelIndex>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>

#include <IObject/ISystemGuiCore/ISystemGuiCoreStyleItemDelegate.h>

//!
//! \file TableColorDelegate.h
//! \class TableColorDelegate
//! \brief Класс делегата раскаски таблицы
//!
class TableColorDelegate : public ISystemGuiCoreStyleItemDelegate
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Класс-родитель
    explicit TableColorDelegate(QObject* parent = 0);

    //! \brief Деструктор класса
    virtual ~TableColorDelegate ();

    //! \brief Метод отрисовки
    //! \param painter QPainter
    //! \param option QStyleOptionViewItem
    //! \param index Индекс
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    //! \brief Метод получения размера ячейки
    //! \param option QStyleOptionViewItem
    //! \param index Индекс
    //! \return Размер ячейки
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index ) const;

//signals:
    // \brief Сигнал об изменении данных в делегате.
    //        Введен для возможности перерисовки таблицы с учетом измененных данных
    //void updateTableData();
};

#endif // TABLECOLORDELEGATE_H
