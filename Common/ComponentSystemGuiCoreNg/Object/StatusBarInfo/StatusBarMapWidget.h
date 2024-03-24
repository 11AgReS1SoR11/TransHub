#ifndef STATUSBARMAPWIDGET_H
#define STATUSBARMAPWIDGET_H


#include <QtWidgets/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMdiSubWindow>
#include <QtWidgets/QMdiArea>
#include <QtGui/QStandardItemModel>

#include <QtCore/QMutex>
#include <QtCore/QTimer>
#include <QtCore/QVector>
#include "StatusBarMapWidgetToolTip.h"


class MainWindow;

/*!
 * \file StatusBarVolume.h
 * \class StatusBarVolume
 * \brief Класс виджета регулировки громкости
 */
class StatusBarMapWidget : public QLabel
{
    Q_OBJECT

public:

    //! \brief Конструктор класса
    //! \param mainWindow Основное окно системы
    //! \param parent Виджет-родитель
    //! \param f Флаги
    explicit StatusBarMapWidget (MainWindow* mainWindow, QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~StatusBarMapWidget ();

protected:
    //! \brief Метод обработки клика мыши
    //! \param event QMouseEvent
    void mousePressEvent (QMouseEvent *event) override;

    //! \brief Метод обработки перемещения мыши
    //! \param event QMouseEvent
    void mouseMoveEvent (QMouseEvent *event) override;;

    //! \brief Метод обработки получения фокуса
    //! \param event QEvent
    void enterEvent (QEvent *event) override;

    //! \brief Метод обработки потери фокуса
    //! \param event QEvent
    void leaveEvent (QEvent *event) override;

    MainWindow* _mainWindow;                /**< основное окно системы */
    StatusBarMapWidgetToolTip* _infoToolTip;   /**< виджет всплывающей подсказки */

    bool _alreadyShow;          /**< всплывающая посказка уже выведена */

    QPixmap _activeMute;        /**< активная иконка (нет звука) */
    QPixmap _notActiveMute;     /**< пассивная иконка (нет звука) */

    QPixmap _activeNotMute;     /**< активная иконка (есть звук) */
    QPixmap _notActiveNotMute;  /**< пассивная иконка (есть звук) */

    //! \brief Получить активную иконку
    //! \param defaultPixmap Базовая иконка
    //! \return Иконка
    QPixmap highlighPixmap (const QPixmap &defaultPixmap);


    int normalFontSize;
    int hoverFontSize;

public slots:
    /*!
     * \brief Слот скрытия всплывающей подсказки
     */
    void slotHideToolTip();

protected slots:
    /*!
     * \brief Слот закрытия всплывающей подсказки
     */
    void cloceToolTipWindow();

signals:

    /*!
     * \brief Сигнал, и нформирующий о необходимости закрыть всплывающую подсказку
     */
    void signalHideToolTip();
};


#endif // STATUSBARMAPWIDGET_H
