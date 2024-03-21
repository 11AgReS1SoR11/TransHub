#ifndef TABLEBOOLDELEGATE_H
#define TABLEBOOLDELEGATE_H

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
//! \file TableBoolDelegate.h
//! \class TableBoolDelegate
//! \brief Класс делегата преобразования булевых значений
//!
class TableBoolDelegate : public ISystemGuiCoreStyleItemDelegate
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Класс-родитель
    explicit TableBoolDelegate(QObject* parent = 0);

    //! \brief Деструктор класса
    virtual ~TableBoolDelegate ();

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

protected:
    //! \brief Преобразование данных
    //! \param[in] value Значение
    //! \param[out] textData Результат
    void getData (int value, QString& textData) const;

    //! \brief Преобразование данных
    //! \param[in] value Значение
    //! \param[out] textData Результат
    void getData (bool value, QString& textData) const;

//signals:
    // \brief Сигнал об изменении данных в делегате.
    //        Введен для возможности перерисовки таблицы с учетом измененных данных
    //void updateTableData();
};

#endif // TABLEBOOLDELEGATE_H
