#ifndef EXTERNALDIALOG_H
#define EXTERNALDIALOG_H

#include <QtWidgets/QDialog>

//!
//! \brief Класс внешнего окна, для которого в качестве родителя выступает QDialog.
//! ВАЖНО! Если окно открывается через главное меню, то окно помещается в качестве
//! дочернего в ExternalDialog. Если дочерний виджет хочет перехватывать close event,
//! ему необходимо определить метод Q_INVOKABLE bool invokeClose (). Если метод
//!
class ExternalDialog : public QDialog
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param childWidget Виджет для отображения.
    //! \param parent Родительский виджет.
    explicit ExternalDialog (QWidget *childWidget, QWidget *parent = nullptr);

protected slots:
    //! \brief Обработчик удаления виджета.
    //! \param object Указтель на удалямый объект.
    void childDestoyed (QObject *);

protected:
    //! \brief Обработчик нажатия на клаве.
    //! \param event объект события.
    void keyPressEvent (QKeyEvent *event) override;

    //! \brief Обарботчик сигнала о закрытии окна
    //! \param event Объект события.
    void closeEvent (QCloseEvent *event) override;

    //! Дочерний виджет
    QWidget *_childWidget {nullptr};

    //! Флаг режима отладки.
    bool _debug = true;
};

#endif // EXTERNALDIALOG_H
