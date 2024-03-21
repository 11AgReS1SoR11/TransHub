#ifndef MAINWINDOWCONSOLE_H
#define MAINWINDOWCONSOLE_H

#include <QtWidgets/QWidget>
#include <QtGui/QClipboard>
#include <QtWidgets/QMenu>
#include <QtGui/QStandardItemModel>
#include <QtCore/QSortFilterProxyModel>

#include <QtCore/QSettings>
#include <QtCore/QDateTime>
#include "../Delegates/TableColorDelegate.h"

#include <Application/IApplication.h>

namespace Ui {
    class MainWindowConsole;
}

/*!
 * \file MainWindowConsole.h
 * \class MainWindowConsole
 * \brief Класс виджета системной консоли
 */
class MainWindowConsole : public QWidget
{
    Q_OBJECT
    
public:
    /*!
     * \brief Конструктор класса
     * \param parent Виджет-родитель
     */
    explicit MainWindowConsole (QWidget *parent = nullptr);

    /*!
     * \brief Деструктор класса
     */
    ~MainWindowConsole();
    
private:
    Ui::MainWindowConsole *ui;          /**< объект графического интерфейса */

    IApplication* _app;                 /**< менеджер GUI приложения */

    QStandardItemModel* _model;         /**< модель данных */
    QSortFilterProxyModel* _sortModel;  /**< модель сортировки */
    QModelIndex _currentIndex;          /**< текущий индекс */

    /*!
     * \brief Загрузить режим отображения
     */
    void loadWindowMode();

    /*!
     * \brief Сохранить режим отображения
     */
    void saveWindowModel();

    /*!
     * \brief Инициализация модели данных
     */
    void initModel();

protected slots:
    /*!
     * \brief Слот обработки системных логов
     * \param msgType Тип сообщения
     * \param msg Сообщение
     */
    void applicationLogData (const QString &msgType, const QString &msg);

    /*!
     * \brief Слот обработки кнопки "Debug"
     * \param value true / false
     */
    void clickDebug(bool value);

    /*!
     * \brief Слот обработки кнопки "Warning"
     * \param value true / false
     */
    void clickWarning(bool value);

    /*!
     * \brief Слот обработки кнопки "Critical"
     * \param value true / false
     */
    void clickCritical(bool value);

    /*!
     * \brief Слот обработки кнопки "Очистить"
     */
    void clickClear();

    /*!
     * \brief Слот обработки кнопки "Копировать"
     */
    void clickCopy();

    /*!
     * \brief Слот обработки нажатия на элементе таблицы
     * \param index Индекс
     */
    void clickInCell(const QModelIndex& index);

    /*!
     * \brief Слот отображения контекстного меню
     * \param pos Положение
     */
    void contextMenuView(const QPoint& pos);

    /*!
     * \brief Слот обновления модели
     * \param indexFrom Индекс начала
     * \param indexTo Индекс конца
     */
    void updateDataModel(const QModelIndex& indexFrom,
                         const QModelIndex& indexTo);
};

#endif // MAINWINDOWCONSOLE_H
