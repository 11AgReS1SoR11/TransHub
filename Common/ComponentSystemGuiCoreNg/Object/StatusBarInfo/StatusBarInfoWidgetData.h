#ifndef STATUSBARINFOWIDGETDATA_H
#define STATUSBARINFOWIDGETDATA_H

#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QMutex>
#include <QtGui/QStandardItemModel>
#include "SystemGuiCorePopUpElement.h"
#include "../Delegates/TableColorDelegate.h"

namespace Ui {
    class StatusBarInfoWidgetData;
}

/*!
 * \file StatusBarInfoWidgetData.h
 * \class StatusBarInfoWidgetData
 * \brief Класс окна с данными о событиях
 */
class StatusBarInfoWidgetData : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param model Модель с данными
     * \param parent Виджет-родитель
     */
    explicit StatusBarInfoWidgetData(QStandardItemModel* model,
                                     QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~StatusBarInfoWidgetData();

    /*!
     * \brief Инициализация окна
     */
    void initWindow();

private:
    Ui::StatusBarInfoWidgetData *ui;    /**< объект графического интерфейса */

    QStandardItemModel* _model;         /**< модель с данными */
    QModelIndex _currentIndex;          /**< текущий индекс */
    QMutex _mutex;                      /**< мьютекс */
    bool _autoScroll;                   /**< использовать авто.прокрутку */
    int  _modelRows;                    /**< количество строк в модели */

protected slots:
    /*!
     * \brief Слот обработки нажатия на ячейку модели
     * \param index Индекс
     */
    void clickInCell(const QModelIndex& index);

    /*!
     * \brief Слот обработки двойного нажатия на ячейку модели
     * \param index Индекс
     */
    void doubleClickInCell(const QModelIndex& index);

    /*!
     * \brief Слот отображения контекстного меню
     * \param pos Положение
     */
    void contextMenuView(const QPoint& pos);

    /*!
     * \brief Слот обработки кнопки "Удалить элемент"
     */
    void deleteElement();

    /*!
     * \brief Слот обработки кнопки "Удалить все элементы"
     */
    void deleteAll();

    /*!
     * \brief Слот обработки кнопки "Авто.прокрутка"
     * \param value true / false
     */
    void autoScroll(bool value);

    /*!
     * \brief Слот авто.прокрутки
     */
    void autoScroll();

signals:
    /*!
     * \brief Сигнал, информирующий об активации элемента
     * \param index Индекс
     */
    void activateElement(QModelIndex index);

    /*!
     * \brief Сигнал, информирующий об удалении элемента
     * \param index Индекс
     */
    void removeElement(QModelIndex index);

    /*!
     * \brief Сигнал, информирующий об удалении всех элементов
     */
    void removeAllElements();
};

#endif // STATUSBARINFOWIDGETDATA_H
