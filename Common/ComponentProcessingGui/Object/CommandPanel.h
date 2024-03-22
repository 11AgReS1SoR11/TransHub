#ifndef COMMANDPANEL_H
#define COMMANDPANEL_H

#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>
#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QItemSelection>


namespace Ui {
    class CommandPanel;
}

//!
//! \brief Виджет для панели с командами.
//! Отображение команд в сокращённом виде.
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
    //! \brief Обработчик ответа успешного завершения запроса.
    //! \param guid Идентификатор запроса.
    //! \param type Тип запроса.
    void slotUpdated (const QString &guid, int type);

    //! \brief Обработчик ответа с ошибкой от менеджера.
    //! \param msg Сообщение.
    //! \param type Тип запроса.
    void slotUpdateFailed (const QString &msg, int type);

    //! \brief Инициализация списка заявок.
    void slotInit ();

private:
    enum Columns {
        RequestNumber = 0,  //-- Номер заявки
        RsRnNumber,         //-- Номер р/с,р/н
        Direction,          //-- Направление
        CommandNumber,      //-- Номер команды
        StartDate,          //-- Дата начала выполнения заявки
        ColumnsCount        //-- Количество колонок
    };

    //! \brief Отобразить сообщение об ошибке.
    //! \param msg Сообщение.
    void showMessage (const QString &msg, bool vis = true);

    //! Интерфейс.
    Ui::CommandPanel *ui { nullptr };


    //! Модель заявок.
    QStandardItemModel *_model { nullptr };
    //! Модель сортировки.
    QSortFilterProxyModel *_sfModel { nullptr };

    //! Флаг режима отладки.
    bool _debug {false};
};

#endif // COMMANDPANEL_H
