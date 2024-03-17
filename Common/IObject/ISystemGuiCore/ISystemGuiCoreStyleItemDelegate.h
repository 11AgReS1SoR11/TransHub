#ifndef GUICORESTYLEITEMDELEGATE_H
#define GUICORESTYLEITEMDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>

#include <ComponentsCore5alpha/ComponentManager>
using namespace ComponentsCore;

//!
//! \file ISystemGuiCoreStyleItemDelegate.h
//! \class ISystemGuiCoreStyleItemDelegate
//! \brief Базовый класс делегата
//!
class ISystemGuiCoreStyleItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
     //! \brief Конструктор класса
     //! \param parent Родительский объект
    explicit ISystemGuiCoreStyleItemDelegate (QObject* parent = nullptr)
        : QStyledItemDelegate (parent)
    {
        _paintBackground = true;
    }

     //! \brief Деструктор класса
    virtual ~ISystemGuiCoreStyleItemDelegate () {}

     //! \brief Рисовать задний фон
     //! \param isPaint Да/Нет
    void paintBackground (bool isPaint) {
        _paintBackground = isPaint;
    }

signals:
    //! \brief Сигнал об изменении данных в делегате.
    //!        Введен для возможности перерисовки таблицы с учетом измененных данных
    void updateTableData ();

protected:
    bool _paintBackground;
};

#endif // GUICORESTYLEITEMDELEGATE_H
