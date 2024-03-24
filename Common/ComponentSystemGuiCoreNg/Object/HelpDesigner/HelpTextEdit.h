#ifndef HELPTEXTEDIT_H
#define HELPTEXTEDIT_H

#include <QtWidgets/QTextEdit>
#include <QtGui/QMouseEvent>

//!
//! \file HelpTextEdit.h
//! \class HelpTextEdit
//! \brief Класс виджета QTextEdit для окна справки
//! \author Кромачев Максим
//!
class HelpTextEdit : public QTextEdit {

    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Виджет-родитель
    explicit HelpTextEdit (QWidget *parent = nullptr);

    //! \brief Конструктор класса
    //! \param text Текст
    //! \param parent Виджет-родитель
    explicit HelpTextEdit (const QString &text, QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~HelpTextEdit ();

protected:
    //! \brief Метод обработки отпускания кнопки мыши
    //! \param e QMouseEvent
    void mouseReleaseEvent (QMouseEvent *e);

signals:
    //! \brief Сигнал, информирующий о необходимости перехода к якорю
    //! \param anchor Путь до якоря
    void gotoAnchor (const QString &anchor);
};

#endif // HELPTEXTEDIT_H
