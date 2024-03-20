#ifndef RUNERRORDIALOG_H
#define RUNERRORDIALOG_H

#include <QtWidgets/QDialog>
#include <QtCore/QFileInfoList>

namespace Ui {
    class RunErrorDialog;
}

//!
//! \file RunErrorDialog.h
//! \class RunErrorDialog
//! \brief Класс окна вывода ошибок работы системы
//!
class RunErrorDialog : public QDialog
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param iconsList Список иконок логотипа
    //! \param errorCaption Заголовок ошибки
    //! \param errorDataLog Лог ошибки
    //! \param parent Родительский объект
    explicit RunErrorDialog (const QFileInfoList &iconsList,
                              const QString& errorCaption,
                              const QString& errorDataLog,
                              QWidget *parent = nullptr);

     //! \brief Деструктор класса
    ~RunErrorDialog ();

private:
    //!< объект графического интерфейса
    Ui::RunErrorDialog *ui {nullptr};

     //! \brief Метод поиска логотипа
     //! \param iconsList Список иконок логотипа
     //! \param iconSize Размер иконки
     //! \return Иконка логотипа
    QPixmap searchLogo (const QFileInfoList &iconsList, const QSize &iconSize);

    //! \brief Метод конвертации изображения в оттенки серого
    //! \param pixmap Изображение.
    //! \return Монохромное изображение.
    QPixmap convertPixmapToMonoxrom (const QPixmap &pixmap) const;
};

#endif // RUNERRORDIALOG_H
