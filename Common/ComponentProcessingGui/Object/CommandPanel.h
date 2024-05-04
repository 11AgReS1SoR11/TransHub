#ifndef COMMANDPANEL_H
#define COMMANDPANEL_H

#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QItemSelection>
#include "planningmanager.h"


namespace Ui {
    class CommandPanel;
}

//!
//! \brief Виджет для панели с участниками маршрута.
//! \author Кромачев Максим, Сажин Даниил
//!
class CommandPanel : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param parent Родительский виджет.
    explicit CommandPanel (QWidget *parent = nullptr);

    //! \brief Деструктор.
    virtual ~CommandPanel ();

protected slots:
    //! \brief Обработчик добавления нового объекта на карту
    void slotUpdated ();

    //! \brief Обработчик ответа с ошибкой от менеджера.
    //! \param msg Сообщение.
    //! \param type Тип запроса.
    void slotUpdateFailed (const QString &msg, int type);

    //! \brief Инициализация списка объектов
    void slotInit ();

    void clearAll();

private:

    enum Columns {
        Supplier = 0, //!<Поставщик
        Stockroom, //!<Склад
        Receiver, //!<Получатель
        ColumnsCount
    };

    //! \brief Отобразить сообщение об ошибке.
    //! \param msg Сообщение.
    void showMessage (const QString &msg, bool vis = true);

    //! Интерфейс.
    Ui::CommandPanel *ui { nullptr };

    //! Модель участников маршрута
    QStandardItemModel *_model { nullptr };
    //! Модель сортировки.
    QSortFilterProxyModel *_sfModel { nullptr };

    //! Флаг режима отладки.
    bool _debug {false};

signals:

    void UpdateSignal();
};

#endif // COMMANDPANEL_H
