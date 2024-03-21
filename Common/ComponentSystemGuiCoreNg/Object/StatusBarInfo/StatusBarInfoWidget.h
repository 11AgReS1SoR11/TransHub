#ifndef STATUSBARINFOWIDGET_H
#define STATUSBARINFOWIDGET_H

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
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QBuffer>
#include <QtCore/QPointer>


#include "StatusBarInfoWidgetPopupWindow.h"
#include "StatusBarInfoWidgetToolTip.h"
#include "StatusBarInfoWidgetData.h"
#include "SystemGuiCorePopUpElement.h"

class MainWindow;

/*!
 * \file StatusBarInfoWidget.h
 * \class StatusBarInfoWidget
 * \brief Класс виджета информирования о событиях
 */
class StatusBarInfoWidget : public QLabel
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param mainWindow Основное окно системы
     * \param parent Виджет-родитель
     * \param f Флаги
     */
    explicit StatusBarInfoWidget (MainWindow* mainWindow, QWidget *parent = nullptr);

    /*!
     * \brief Деструктор класса
     */
    virtual ~StatusBarInfoWidget();

    /*!
     * \brief Добавить событие
     * \param popupElement Объект всплывающего сообщения
     * \param autoClose Автоматически скрывать
     */
    void addStatusElement(SystemGuiCorePopUpElement* popupElement, bool autoClose = true);

protected:
    /*!
     * \brief Метод обработки нажатия мыши
     * \param event QMouseEvent
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Метод обработки перемещения мыши
     * \param event QMouseEvent
     */
    void mouseMoveEvent(QMouseEvent *event);

    /*!
     * \brief Метод обработки получения фокуса
     * \param event QEvent
     */
    virtual void enterEvent(QEvent *event);

    /*!
     * \brief Метод обработки потери фокуса
     * \param event QEvent
     */
    virtual void leaveEvent(QEvent *event);

    MainWindow* _mainWindow;                                /**< основное окно системы */
    StatusBarInfoWidgetToolTip* _infoToolTip;               /**< всплывающая подсказка */
    StatusBarInfoWidgetData*    _dataWindow;                //!< окно с информацией о событиях

    QVector<StatusBarInfoWidgetPopupWindow*> _allPopUps;    /**< список всплывающих окон, открытых в данный момент */

    bool _alreadyShow;          /**< всплывающая посказка уже выведена  */
    int _actionElements;        /**< количество не просмотренных событий */

    QPixmap _activeMakeData;    //!< активная иконка "есть новые данные"
    QPixmap _activeNoData;      //!< активная иконка "нет данных"
    QPixmap _notActiveMakeData; //!< пассивная иконка "нет новых данных"
    QPixmap _notActiveNoData;   //!< пассивная иконка "нет данных"

    QVector<SystemGuiCorePopUpElement*> _popupElements;     /**< список объектов всплывающих сообщений */

    QStandardItemModel* _popupModel;    /**< модель с данными */

    bool    _isMute;            /**< воспроизводить ли звук */
    bool    _isStopped;         /**< остановлено ли воспроизведение */
    QBuffer    _soundBuffer;    /**< буффер со звуковым файлом */

    ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType _currentSound;              /**< тип текущего звука воспроизведения */
    ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType _currentSoundRe;            /**< тип звука для повторного воспроизведения */
    QVector <ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType> _replaySounds;    /**< список звуков для повторного воспроизведения */

    QTimer  _soundTimerPlay;    /**< таймер, отвечающий за начало воспроизведения */
    QTimer  _soundTimerStop;    /**< таймер, отвечающий за окончание воспроизведения */
    QTimer  _soundTimerAdd;     /**< таймер, отвечающий за добавление звука на повтор */
    QMap<ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType, QByteArray> _allSamples;  /**< список звуков */


    QMutex _mutex;  /**< мьютекс */

    /*!
     * \brief Получить последнее всплывающее окно
     * \return Всплывающее окно
     */
    StatusBarInfoWidgetPopupWindow* getLastPopUp();

    //! \brief Подсветить иконку.
    //! Создание картинки с эффектом подсветки.
    //! \param pix Базовая иконка.
    //! \return Иконка.
    QPixmap highlighPixmap (const QPixmap &pix);

    //! \brief Инициализация звуков
    void initSamples ();

    //! \brief Инициализация аудиоустройств
    void initAudio ();

    //! \brief Инициализация модели данных
    void initModel();

    //! \brief Добавить событие в модель
    //! \param popupElement Объект всплывающего сообщения
    void addStatusElementInModel(SystemGuiCorePopUpElement *popupElement);

    /*!
     * \brief Удалить событие из модели
     * \param index Индекс
     */
    void removeStatusElementInModel(const QModelIndex& index);

    /*!
     * \brief Получить объект всплывающего сообщения из модели
     * \param index Индекс
     * \return Объект всплывающего сообщения
     */
    SystemGuiCorePopUpElement* getPopupElementFromModel(const QModelIndex& index);

public slots:
    /*!
     * \brief Слот обработки громкости
     * \param isMute true / false
     */
    void slotVolumeChanged(bool isMute);

    /*!
     * \brief Слот обработки скрытия всплывающей подсказки
     */
    void slotHideToolTip();

protected slots:
    /*!
     * \brief Слот закрытия всплывающего окна
     * \param window Всплывающее окно
     */
    void closePopupWindow(StatusBarInfoWidgetPopupWindow* window);

    /*!
     * \brief Слот смещения всплывающих окон
     */
    void offsetPopups();

    /*!
     * \brief Слот закрытия всплывающей подсказки
     */
    void cloceToolTipWindow();

    /*!
     * \brief Слот закрытия окна с данными
     */
    void closeDataWindow();

    /*!
     * \brief Слот активации элемента в модели
     * \param index Индекс
     */
    void activateStatusElement(QModelIndex index);

    /*!
     * \brief Слот удаления элемента
     * \param index Индекс
     */
    void removeStatusElement(QModelIndex index);

    /*!
     * \brief Слот удаления всех элементов
     */
    void removeAllElements();

    /*!
     * \brief Слот сброса счетчика непросмотренных элементов
     */
    void clearCount();

    /*!
     * \brief Слот воспроизведения звука
     */
    void playSound();

    /*!
     * \brief Слот воспроизведения звука
     * \param type Тип звука
     */
    void playSound(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType type);

    /*!
     * \brief Слот повторного воспроизведения звука
     * \param type Тип звука
     */
    void replaySound(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType type);

    /*!
     * \brief Слот повторного воспроизведения звука
     */
    void replaySound();

    /*!
     * \brief Слот остановки воспроизведения
     */
    void stopPlaySound();

    /*!
     * \brief Слот добавления звука на повторное воспроизведение
     */
    void addSound();

signals:
    /*!
     * \brief Сигнал, информирующий о необходимости закрыть всплывающую подсказку
     */
    void signalHideToolTip();

    /*!
     * \brief Сигнал, информирующий о необходимости смещения всплывающих окон
     */
    void signalOffsetPopups();
};

#endif // STATUSBARINFOWIDGET_H
