#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QtCore/QString>
#include <QtCore/QVector>

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtGui/QTextDocument>
#include <QtGui/QCloseEvent>

#include "HelpSection.h"
#include "HelpTextEdit.h"
#include "HelpSearchWidget.h"

namespace Ui {
class HelpWindow;
}

//!
//! \file HelpWindow.h
//! \class HelpWindow
//! \brief Класс окна файла справки
//! \author Ivanov Anton
//!
class HelpWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param fileText Текст
    //! \param sections Список секций
    //! \param parent Виджет-родитель
    explicit HelpWindow (const QString &fileText,
                         const QVector<HelpSection*> &sections,
                         bool isAdminMode,
                         const QPixmap &logo,
                         QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~HelpWindow ();

//signals:
//    // \brief Сигнал, информирующий о закрытии окна
//    void signalClose ();

protected:
    // \brief Обработка закрытия окна
    // \param event QCloseEvent
    //void closeEvent (QCloseEvent *event);

private:
     //! \brief Инициализация ресурсов
     //! \param section Секция
    void initSource (HelpSection* section = nullptr);

     //! \brief Инициализация изображения секции
     //! \param section Секция
    void initSourceImages (HelpSection* section);

     //! \brief Инициализация изображения пункта
     //! \param item Пункт
    void initSourceImages(IHelpItem* item);

private:
    Ui::HelpWindow      *ui;                /**< объект графического интерфейса */
    QTextDocument       *_document {nullptr};         /**< QTextDocument */
    HelpTextEdit        *_textEdit {nullptr};         /**< виджет QTextEdit для окна справки */
    HelpSearchWidget    *_searchWidget {nullptr};     /**< виджет поиска по файлу справке */

    QString                 _fileText;      /**< текст */
    QVector<HelpSection*>   _sections;      /**< "дерево" разделов */
    QMap<QString, QImage>   _addedSources;  /**< добавленные ресурсы */

    QVector<QString>        _anchorsList;   /**< список якорей в порядке добавления */
    int _currentAnchor {0};                     /**< индекс текущего якоря */

    QMenu   *_contextMenu {nullptr};  /**< контекстное меню */
    QAction *_actSearch {nullptr};    /**< пункт меню "Искать" */
    QAction *_actContents {nullptr};  /**< пункт меню "К оглавлению" */
    QAction *_actPrev {nullptr};      /**< пункт меню "Назад" */
    QAction *_actNext {nullptr};      /**< пункт меню "Вперёд" */
    QAction *_actClear {nullptr};     /**< пункт меню "Очистить историю" */

    QAction *_actSeparatorPdf {nullptr};  /**< разделитель */
    QAction *_actPdf {nullptr};           /**< пункт меню "Сохранить в pdf" */

    QPixmap _logo;

protected slots:
    /*!
     * \brief Слот перемещения к началу текста
     */
    void onScrollToContents();

    /*!
     * \brief Слот перемещения к следующему якорю
     */
    void onNext();

    /*!
     * \brief Слот перемещения к предыдущему якорю
     */
    void onPrev();

    /*!
     * \brief Слот очистки истории
     */
    void onClear();

    /*!
     * \brief Слот добавления якоря
     * \param anchor Якорь
     */
    void onAddAnchor(QString anchor);

    /*!
     * \brief Слот отображения контекстного меню
     * \param point Положение
     */
    void contextMenu(QPoint point);

    void onPdf();
};

#endif // HELPWINDOW_H
