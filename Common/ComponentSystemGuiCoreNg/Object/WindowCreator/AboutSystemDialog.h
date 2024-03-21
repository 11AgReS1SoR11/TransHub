//!
//! \file DialogAboutSystem.h
//!

#ifndef DIALOGABOUTSYSTEM_H
#define DIALOGABOUTSYSTEM_H

#include <QtWidgets/QDialog>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

#include <ComponentsCore5alpha/ComponentManager>

namespace Ui {
class AboutSystemDialog;
}

//!
//! \class DialogAboutSystem
//! \brief Класс окна с информацией о системе
//! \author Ivanov Anton
//!
class AboutSystemDialog : public QDialog
{
    Q_OBJECT

public:
    //! \brief Конструктор
    //! \param iconLogo Логотип
    //! \param windowTitle Заголовок окна
    //! \param parent Виджет-родитель
    explicit AboutSystemDialog (const QPixmap &iconLogo, const QString &windowTitle,
                                QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~AboutSystemDialog ();

protected:
    //! \brief Обработчик нажатия кнопки.
    //! \param e Объект события.
    void keyPressEvent (QKeyEvent *e) override;

private:
    //! Объект графического интерфейса
    Ui::AboutSystemDialog *ui {nullptr};
};

#endif // DIALOGABOUTSYSTEM_H
