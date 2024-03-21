#ifndef MDIAREAWIDGET_H
#define MDIAREAWIDGET_H

#include <QtCore/QObject>

#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtWidgets/QMdiArea>
#include <QtGui/QMouseEvent>

#include <IObject/ISystemGuiCore/ISystemIconManager.h>

//!
//! \file MdiAreaFilter.h
//! \class MdiAreaFilter
//! \brief Класс фильтра для QMdiArea
//!
class MdiAreaFilter : public QObject
{
    Q_OBJECT

public:
    //!
    //! \brief Конструктор
    //! \param parent родительский объект
    //!
    MdiAreaFilter (QObject *parent = 0);

    //!
    //! \brief Деструктор класса
    //!
    ~MdiAreaFilter();

    //!
    //! \brief Задать объект MdiArea
    //! \param mdiArea объект MdiArea
    //!
    void setMdiArea (QMdiArea *mdiArea);

    //!
    //! \brief Задать менеджер иконок
    //! \param iconManager менеджер иконок
    //!
    void setIconManager (ISystemIconManager *iconManager);

    //!
    //! \brief Обновить фон MdiArea
    //!
    void updateMdiBrush();

protected:
    QMdiArea            *_mdiArea;      /**< центральный виджет */
    ISystemIconManager  *_iconManager;  /**< менеджер иконок */

    QPixmap _logo;              /**< логотип (черно-белый) */
    QColor  _baseColor;         /**< базовый цвет MdiArea */
    int     _logoSize;          /**< размер стороны логотипа */
    int     _baseDifference;    /**< базовая разница цветов MdiArea */

    //! \brief Метод конвертации изображения в оттенки серого (монохромное).
    //! \param pixmap Изображение.
    //! \return Монохромное изображение.
    QPixmap convertPixmapToMonoxrom (const QPixmap &pixmap) const;

    //! \brief Метод поиска часто повторяемого цвета
    //! \param pixmap Иконка логотипа
    //! \return Часто повторяемый цвет
    QRgb searchRgb (const QPixmap &pixmap);

    //!
    //! \brief Обработка события
    //! \param obj Объект
    //! \param event Событие
    //! \return true / false
    //!
    virtual bool eventFilter (QObject *obj, QEvent *event);

    //!
    //! \brief Иправление значения цвета
    //! Если цвет в рузльтате расчёта ешёл за рамки от 0 до 255,
    //! вернёт обработно к ближайшей границе
    //! \param value Значение
    //! \return Значение
    //!
    inline int fixer (int value) const;

signals:
    //!
    //! \brief Сигнал, информирующий о позиции мыши
    //! \param pos Позиция мыши
    //!
    void mousePos (const QPoint &pos);
};

#endif // MDIAREAWIDGET_H
