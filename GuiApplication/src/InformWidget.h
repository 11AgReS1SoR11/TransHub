#ifndef INFORMWIDGET_H
#define INFORMWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtCore/QFileInfoList>
#include <QtGui/QRgb>

namespace Ui {
    class InformWidget;
}

//!
//! \file InformWidget.h
//! \class InformWidget
//! \brief Класс информационного виджета
//!
class InformWidget : public QWidget
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param iconsList Список иконок логотипа
    //! \param w Ширина виджета
    //! \param parent Родительский объект
    explicit InformWidget (const QFileInfoList &iconsList, int w, QWidget *parent = nullptr);

    //! \brief Деструктор класса
    ~InformWidget();

    //! \brief Метод получения кнопки "отмена"
    ///! \return Кнопка "отмена"
    QToolButton* buttonCancel ();

    //! \brief Метод получения кнопки "далее"
    //! \return Кнопка "далее"
    QToolButton* buttonNext ();

    //! \brief Метод получения кнопки "Settings"
    //! \return Кнопка "Settings"
    QToolButton* buttonSettings ();

    //! \brief Метод получения кнопки "журнал запуска"
    //! \return Кнопка "журнал запуска"
    QToolButton* buttonLog ();

    //! \brief Метод получения дополнительного прогресс-бара
    //! \return Дополнительный прогресс-бар
    QProgressBar* progBarOther ();

    //! \brief Метод получения прогресс-бара "загрузка компонентов"
    //! \return Прогресс-бар "загрузка компонентов"
    QProgressBar* progBarLoad ();

    //! \brief Метод получения прогресс-бара "инициализация компонентов"
    //! \return Прогресс-бар "инициализация компонентов"
    QProgressBar* progBarInit ();

    //! \brief Метод получения лэйбла "дополнительный прогресс-бар"
    //! \return Лэйбл "дополнительный прогресс-бар"
    QLabel* labelOther ();

    //! \brief Метод получения лэйбла с иконкой "загрузка компонентов"
    //! \return Лэйбл с иконкой "загрузка компонентов"
    QLabel* labelLoadIcon ();

    //! \brief Метод получения лэйбла с текстом "загрузка компонентов"
    //! \return Лэйбл с текстом "загрузка компонентов"
    QLabel* labelLoadText ();

    //! \brief Метод получения лэйбла с иконкой "инициализация компонентов"
    //! \return Лэйбл с иконкой "инициализация компонентов"
    QLabel* labelInitIcon ();

    //! \brief Метод получения лэйбла с текстом "инициализация компонентов"
    //! \return Лэйбл с текстом "инициализация компонентов"
    QLabel* labelInitText ();

private:
    //! Объект графического интерфейса
    Ui::InformWidget *ui {nullptr};

    //! \brief Метод поиска логотипа
    //! \param iconsList Список иконок логотипа
    //! \param iconSize Размер иконки
    //! \return Иконка логотипа
    QPixmap searchLogo (const QFileInfoList &iconsList, QSize iconSize);

    //! \brief Метод поиска часто повторяемого цвета
    //! \param pixmap Иконка логотипа
    //! \return Часто повторяемый цвет
    QRgb searchRgb (const QPixmap &pixmap);
};

#endif // INFORMWIDGET_H
