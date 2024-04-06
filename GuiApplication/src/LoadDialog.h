#ifndef LOADDIALOG_H
#define LOADDIALOG_H

#include <QtWidgets/QDialog>
#include <QtGui/QBitmap>
#include <QtGui/QCloseEvent>
#include <QtCore/QVector>
#include <QtCore/QFileInfoList>
#include <QtCore/QSize>
#include <QtCore/QSettings>

#include "InformWidget.h"

namespace Ui {
class LoadDialog;
}

//!
//! \file DialogLoadComponents.h
//! \class DialogLoadComponents
//! \brief Класс окна, показывающего загрузку комплекса
//!
class LoadDialog : public QDialog
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param iconsList Список иконок логотипа
    //! \param parent Родительский объект
    explicit LoadDialog (const QFileInfoList &iconsList, QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~LoadDialog ();

public slots:
    //! \brief Слот обработки запуска окна
    void execDialog ();

protected slots:
    //! \brief Слот обработки завершения запуска подсистемы компонентов.
    void slotInitDone ();

    // \brief Слот обработки результата поиска количества файлов в каталоге
    // \param files Количество файлов
    //    void foundFilesInFolder (int files);

    // \brief Слот обработки результата поиска количества компонентов в каталоге
    // \param components Количество компонентов
    //void foundComponentsInFolder (int components);

    // \brief Слот обработки ошибки загрузки компонента
    // \param errorData Сообщение об ошибки загрузки компонента
    //void loadSystemComponentError (const QString& errorData);

    // \brief Слот обработки результата загрузки компонента
    // \param componentName Имя компонента
    // \param componentVer Версия компонента
    //void loadSystemComponent (const QString& componentName,
    //                          const QString& componentVer);

    // \brief Слот обработки инициализации компонента
    // \param componentName Имя компонента
    // \param componentVer Версия компонента
    // \param componentGuid Uuid компонента
    // \param loadStatus Статус инициализации
    // \param errorData Сообщение об ошибке инициализации
    //void initSystemComponent (const QString& componentName,
    //                          const QString& componentVer,
    //                          const QString& componentGuid,
    //                          bool loadStatus,
    //                          const QString& errorData);

    //! \brief Показать дополнительный прогресс бар
    //! \param isShow Показать / скрыть
    void showProgressBar (bool isShow);

    //! \brief Сигнал изменения максимального значения прогресс бара
    //! \param value Значение
    void changeProgressBarMax (int maxValue);

    //! \brief Сигнал изменения текста прогресс бара
    //! \param text Значение
    void changeProgressBarText (const QString &text);

    //! \brief Изменение значенея дополнительного прогресс бара
    //! \param value Новое значение
    void changeProgressBarValue (int value);

    //! \brief Слот обработки последующего запуска системы
    void clickNext ();

    //! \brief Слот обработки остановки запуска системы
    void clickCancel ();

    //! \brief Слот обработки вывода журнала запуска системы
    //! \param value true / false
    void clickLogg (bool value);

    //! \brief Слот обработки отображения меню настроек загрузчика
    //! \param value true / false
    void clickSettings (bool value);

    //! \brief Слот запуска загрузки комплекса
    void slotStart ();

    //! \brief Обработчик уведомления о загрузке компонента.
    //! \param componentName Название компонента.
    //! \param componentVer Версия компонента.
    //! \param status Статус загрузки.
    //! \param error Ошибка.
    void slotComponentLoaded (const QString &componentName,
                              const QString &componentVer,
                              bool status,
                              const QString &error = QString ());

    //! \brief Обработчик завершения загрузки (load) всех компонентов.
    void slotComponentsLoaded ();

    //! \brief Обработчик уведомления об инициализации компонента.
    //! \param componentName Название компонента
    //! \param componentVer Версия компонента
    //! \param status Статутс загрузки
    //! \param error Описание ошибки.
    void slotComponentInitialized (const QString &componentName,
                                   const QString &componentVer,
                                   bool status,
                                   const QString &error = QString ());

    //! \brief Обработчик завершения инициализации (initialize) всех компонентов.
    void slotComponentsInitialized ();

    // \brief Обработчик закрытия.
    //void reject () override;

    void changeEvent(QEvent * event) override;

signals:
    //! \brief Сигнал, информирующий об ошибке инициализации системы
    void loadError ();

    //protected:
    // \brief Метод обработки закрытия окна
    // \param event QCloseEvent
    //virtual void closeEvent(QCloseEvent *event);

private:
    Ui::LoadDialog *ui {nullptr};           //!< объект графического интерфейса

    InformWidget *_infoWidget {nullptr};    //!< объект информационного фиджета

    bool _loadError {false};                //!< есть ли ошибки загрузки
    bool _initError {false};                //!< есть ли ошибки инициализации
    int _newError {0};                      //!< количество новых ошибок инициализации
    int _updateError {0};                   //!< количество ошибок обнорвления компонентов
    QVector<QString> _prevErrors;           //!< список ошибок инициализации предыдущего запуска
    QVector<QString> _currentErrors;        //!< список ошибок инициализации текущего запуска

    //! \brief Метод загрузки данных предыдущего запуска
    void loadStart ();

    //! \brief Метод сохранения данных текущего запуска
    void saveStart ();

    //! \brief Метод поиска логотипа
    //! \param iconsList Список иконок логотипа
    //! \param iconSize Размер иконки
    //! \return Иконка логотипа
    QIcon searchLogo (const QFileInfoList &iconsList, const QSize &iconSize);
};

#endif // LOADDIALOG_H
