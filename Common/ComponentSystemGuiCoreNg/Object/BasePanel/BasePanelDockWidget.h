#ifndef BASEPANELDOCKWIDGET_H
#define BASEPANELDOCKWIDGET_H

#include <QtCore/QPointer>
#include <QtCore/QTimer>
#include <QtCore/QEvent>

#include <QtGui/QShowEvent>
#include <QtGui/QHideEvent>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMdiArea>

//!
//! \file BasePanelDockWidget.h
//! \class BasePanelDockWidget
//! \brief Класс виджета базовой информационной панели
//!
class BasePanelDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    //! \brief Cостояние
    enum MoveState {
        MoveVisible = 0,    //!< показана
        MoveInvisible,      //!< скрыта
        MoveShow,           //!< показывается
        MoveHide            //!< скрывается
    };

    //! \brief Конструктор класса
    //! \param title Заголовок панели
    //! \param parent Виджет-родитель
    //! \param flags Флаги
    explicit BasePanelDockWidget (const QString &title, QWidget *parent = nullptr);

    //! \brief Конструктор класса
    //! \param parent Виджет-родитель
    //! \param flags Флаги
    explicit BasePanelDockWidget (QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~BasePanelDockWidget ();

    //! \brief Задать область отображения панели
    //! \param area Область отображения
    void setDockWidgetArea (Qt::DockWidgetArea area);

    //! \brief Задать QMdiArea
    //! \param mdiArea QMdiArea
    void setMdiArea (QMdiArea *mdiArea);

    //! \brief Задать автоматическое скрытие панели
    //! \param isAutoHide true / false
    void setAutoHide (bool isAutoHide);

    //! \brief Задать видимость панели
    //! \param isVisible true / false
    void setPanelVisible (bool isVisible);

    //! \brief Является ли панель видимой
    //! \return true / false
    bool isPanelVisible ();

    //! \brief Задать состояние панели
    //! \param state Состояние панели
    void setPanelState (BasePanelDockWidget::MoveState state);

    //! \brief Получить состояние панели
    //! \return Состояние панели
    BasePanelDockWidget::MoveState panelState ();

protected:
    //! \brief Обработка отображения
    //! \param event QShowEvent
    void showEvent (QShowEvent *event) override;

    //! \brief Обработка скрытия
    //! \param event QHideEvent
    void hideEvent (QHideEvent *event) override;

    //! \brief Обработка получения фокуса
    //! \param event QEvent
    void enterEvent (QEvent *event) override;

    //! \brief Обработка потери фокуса
    //! \param event QEvent
    void leaveEvent (QEvent *event) override;

    QPointer<QMdiArea> _mdiArea;    /**< объект QMdiArea */
    Qt::DockWidgetArea _area;       /**< область отображения панели */
    MoveState   _state;             /**< состояние панели */

    bool    _isPanelVisible;        /**< видна ли панель */
    bool    _isAutoHide;            /**< задано ли авто.скрытие панели */
    int     _size;                  /**< размер (ширина или высота) */

    QTimer  _timerWaitMove;         /**< таймер, отвечающий за показ/скрытие панели */

    //! \brief Получить текущий размер панели (ширина или высота)
    //! \return Текущий размер панели (ширина или высота)
    int currentSize ();

    //! \brief Развернуть панель
    //! \param size Размер в пикселях
    //! \return true / false
    //!
    //! true - если достигли нужного размера
    bool updateShowSize(int size);

    //! \brief Свернуть панель
    //! \param size Размер в пикселях
    //! \return true / false
    //!
    //! true - если достигли нужного размера
    bool updateHideSize(int size);

    //! \brief Отображение/Скрытие панели
    //! \return true / false
    //!
    //! true - если достигли нужного результата (скрыли или показали)
    bool onMove();

public slots:
    //! \brief Слот обработки позиции мыши
    //! \param pos Позиция мыши
    void mousePos(const QPoint pos);

    //! \brief Слот отображения панели
    void showPanel();

protected slots:
    //! \brief Слот отображения/скрытия панели
    void waitTimeOut();
};

#endif // BASEPANELDOCKWIDGET_H
