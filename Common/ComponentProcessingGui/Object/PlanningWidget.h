#ifndef PLANNINGWIDGET_H
#define PLANNINGWIDGET_H

#include <QtWidgets/QToolBar>
#include <QtGui/QStandardItemModel>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QItemSelection>
#include "planningmanager.h"
#include <qglobal.h>

#include "ISystemGuiCoreStatusBarTabWindow.h"
#include "../ComponentSystemGuiCoreNg/Object/MdiArea/CustomMdiSubWindow.h"

namespace Ui {
    class PlanningWidget;
}

//!
//! \brief Виджет отображения команд.
//! \author Кромачев Максим.
//!
class PlanningWidget : public ISystemGuiCoreStatusBarTabWindow
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param parent Родительский виджет.
    explicit PlanningWidget (QWidget *parent = nullptr);

    //! \brief Деструктор.
    virtual ~PlanningWidget ();

    //! \brief Получение иконки для вкладки
    //! \return соответствующая иконка
    QPixmap GetWindowIcon() override;

protected slots:
    //! \brief Обработчик обновления информации об участниках маршрута
    void slotUpdated ();

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

    void OnTabClicked(bool is_only_open) override;

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
