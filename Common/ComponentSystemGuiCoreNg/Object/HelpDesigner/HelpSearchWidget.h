#ifndef HELPSEARCHWIDGET_H
#define HELPSEARCHWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtGui/QTextDocument>
#include <QtGui/QTextCursor>
#include <QtWidgets/QWidget>

#include <QtCore/QVector>

namespace Ui {
    class HelpSearchWidget;
}

/*!
 * \file HelpSearchWidget.h
 * \class HelpSearchWidget
 * \brief Класс виджета поиска по файлу справке
 */
class HelpSearchWidget : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param textEdit QTextEdit
     * \param parent Виджет-родитель
     */
    explicit HelpSearchWidget(QTextEdit *textEdit, QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~HelpSearchWidget();

    /*!
     * \brief Очистить поиск
     */
    void clearSearch();

private:
    Ui::HelpSearchWidget *ui;       /**< объект графического интерфейса */
    QTextEdit       *_textEdit;     /**< QTextEdit */
    QTextDocument   *_document;     /**< QTextDocument */

    QTextCursor     _prevCursor;    /**< предыдущий курсор */
    QString         _searchText;    /**< текст, который следует найти */
    QVector<int>    _cursorPos;     /**< список положений курсора */
    int _currentIndex;              /**< текущий индекс */

protected slots:
    /*!
     * \brief Слот поиска текста
     * \param text Текст
     */
    void onSearch(const QString &text);

    /*!
     * \brief Поиск следующего
     */
    void onNext();

    /*!
     * \brief Поиск предыдущего
     */
    void onPrev();

    /*!
     * \brief Скрыть виджет
     */
    void onHide();

signals:
    /*!
     * \brief Сигнал, информирующий о скрытии виджета
     */
    void signalHide();
};

#endif // HELPSEARCHWIDGET_H
