#ifndef SYSTEMSPLASHSCREEN_H
#define SYSTEMSPLASHSCREEN_H

#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QApplication>
#include <QtGui/QImage>
#include <QtGui/QBitmap>
#include <QtWidgets/QMessageBox>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QFileInfoList>

class Application;

//!
//! \file SystemSplashScreen.h
//! \class SystemSplashScreen
//! \brief Класс окна инициализации системы
//!
class SplashScreen : public QSplashScreen
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param iconsList Список иконок логотипа
    //! \param app Объект приложения
    //! \param parent Родительский объект
    explicit SplashScreen (const QFileInfoList &iconsList, Application *app, QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~SplashScreen ();

    //! \brief Метод перехвата всех сообщений
    //! \param target Объект
    //! \param event Событие
    bool eventFilter (QObject *target, QEvent *event);

protected:
    //! \brief Метод загрузки и инициализации системы
    //! \param administratorMode Режим администратора (true / false)
    //! \param userLogin Логин текущего пользователя
    void loadData (int userMode, const QString& userLogin);

    //! \brief Метод поиска логотипа
    //! \param iconsList Список иконок логотипа
    //! \param iconSize Размер иконки
    //! \return Иконка логотипа
    QPixmap searchLogo (const QFileInfoList &iconsList, const QSize &iconSize);

    Application *_app {nullptr};    //!< объект приложения
    bool _initGuiCore {false};      //!< ожидание инициализации графического ядра
    QFileInfoList _iconsList;       //!< список иконок логотипа

public slots:
    //! \brief Слот обработки запуска окна
    //! \param userMode Режим текущего пользователя
    //! \param userLogin Логин текущего пользователя
    void show (int userMode, const QString &userLogin);

    //! \brief Отображение и запуск инициализации.
    void showAndInit ();

protected slots:
    //! \brief Слот обработки продолжения загрузки системы
    void continueWhile ();

signals:
    //! \brief Сигнал, информирующий об ошибке инициализации системы
    void loadError ();
};

#endif // SYSTEMSPLASHSCREEN_H
