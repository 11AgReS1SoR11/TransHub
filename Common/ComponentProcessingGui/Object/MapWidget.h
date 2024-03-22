#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtGui/QStandardItemModel>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QItemSelection>


namespace Ui {
    class MapWidget;
}

//!
//! \brief Виджет отображения команд.
//! \author Кромачев Максим.
//!
class MapWidget : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param parent Родительский виджет.
    explicit MapWidget (QWidget *parent = nullptr);
    //! \brief Деструктор.
    virtual ~MapWidget ();

protected slots:
    //! \brief Обработчик обновления информации о заявках.
    //! \param guid Идентификатор зявки.
    //! \param type Тип запроса.
    void slotUpdated (const QString &guid, int type);

    //! \brief Обработчик ответа с ошибкой от менеджера.
    //! \param msg Сообщение.
    //! \param type Тип запроса.
    void slotUpdateFailed (const QString &msg, int type);

    //! \brief Инициализация списка заявок.
    void slotInit ();

    //! \brief Обработка вызова контекстного меню.
    void slotRequestContextMenu (const QPoint &pos);

    //! \brief Обновление группировки строк.
    void updateSpan ();

    //! \brief Обновление состояния кнопок тулбара
    void updateToolbar ();

    //! \brief Обработчик клика "Просмотр".
    void slotShowClicked ();

    //! \brief Обработчик выделения строки в таблице.
    void slotSelectionChanged (const QItemSelection &se, const QItemSelection &de);

private:
    //! \brief Создание тулбара.
    void createToolBar ();

    //! Интерфейс.
    Ui::MapWidget *ui {nullptr};

    enum Columns {
        RequestNumber = 0,  //!< Номер заявки
        RsRnNumber,         //!< Номер р/с,р/н
        Status,             //!< Статус
        MapNumber,      //!< Номер команды
        ReceiverCallsign,   //!< Узел связи корреспондента
        Production,         //!< Технология (АПД Оникс, Созвездие, ОНИИП)
        CommunicationType,  //!< Вид связи
        InterfaceType,      //!< Тип стыка
        Mode,               //!< Режим работы
        StartDate,          //!< Дата начала
        FreqsRx,            //!< Частоты приёма
        FreqsTx,            //!< Частоты переадчи
        ReceiveDate,        //!< Время поступления
        Operator,           //!< Оператор
        ColumnsCount        //!< Количество колонок
    };



    //! \brief Отобразить сообщение об ошибке.
    //! \param msg Сообщение.
    void showMessage (const QString &msg, bool vis = true);

    //! Тулбар.
    QToolBar *_toolBar { nullptr };


    //! Модель заявок.
    QStandardItemModel *_model { nullptr };
    //! Модель сортировки.
    QSortFilterProxyModel *_sfmodel { nullptr };

    //! Последний выделенный индекс.
    QModelIndex _currentIndex;

    //! Флаг режима отладки.
    bool _debug {false};
};

#endif // MapWIDGET_H
