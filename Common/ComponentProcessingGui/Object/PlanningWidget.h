#ifndef PLANNINGWIDGET_H
#define PLANNINGWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtGui/QStandardItemModel>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QItemSelection>

#include <qglobal.h>

namespace Ui {
    class PlanningWidget;
}

//!
//! \brief Виджет отображения команд.
//! \author Кромачев Максим.
//!
class PlanningWidget : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param parent Родительский виджет.
    explicit PlanningWidget (QWidget *parent = nullptr);
    //! \brief Деструктор.
    virtual ~PlanningWidget ();

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
    Ui::PlanningWidget *ui {nullptr};


    enum Columns {
        Supplier = 0, //!<Поставщик
        Stockroom, //!<Склад
        Receiver, //!<Получатель
        ColumnsCount
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

#endif // PLANNINGWIDGET_H
