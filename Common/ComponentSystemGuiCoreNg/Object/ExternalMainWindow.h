#ifndef EXTERNALMAINWINDOW_H
#define EXTERNALMAINWINDOW_H

#include <QtWidgets/QMainWindow>

//!
//! \brief Класс внешнего окна.
//!
class ExternalMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param childWidget Дочерний виджет.
    //! \param parent Родительский виджет.
    explicit ExternalMainWindow (QWidget *childWidget, QWidget *parent = nullptr);
    
signals:
    //! \brief Сигнла о закрытии окна.
    //! \param value Код.
    //void finished (int value);

protected:
    //! \brief Обарботчик сигнала о закрытии окна
    //! \param event Объект события.
    //void closeEvent (QCloseEvent *event);

    //! \brief Дочерний виджет.
    QWidget *_childWidget;
};

#endif // EXTERNALMAINWINDOW_H
