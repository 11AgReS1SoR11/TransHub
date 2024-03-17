#ifndef IAPPLICATION_H
#define IAPPLICATION_H

#include <QtWidgets/QApplication>

//!
//! \file IApplication.h
//! \class IApplication
//! \brief Интерфейсный класс, управляющий потоком приложения с графическим интерфейсом и основными настройками.
//!
class IApplication : public QApplication
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param argc Количество входящих аргументов
    //! \param argv Входящие аргументы
    //! \param appFlags ApplicationFlags
    IApplication (int &argc, char **argv, int appFlags = ApplicationFlags)
        : QApplication (argc, argv, appFlags) {}

    //! \brief Деструктор класса
    virtual ~IApplication () {}

    //! \brief Вывод логов в консоль
    //! \param value true / false
    virtual void setLogToConsole (bool value) = 0;

    //! \brief Отсылать логи на на удалённый сервер.
    //! \param eth Собственный интерфейс с которого отсылаются логи
    virtual void setRemoteServerLog (const QString &eth, const QString &ip, unsigned int port) = 0;

    //! \brief Отсылать логи на AMQP сервер.
    //! \param eth
    //! \param host Адрес сервера
    //! \param port Порт сервера
    //! \param user Пользователь
    //! \param pass Пароль
    //! \param exchange Название exchange
    virtual void setAmqpServerLog (const QString &eth, const QString &host, unsigned int port, const QString &user,
                                   const QString &pass, const QString &exchange) = 0;

    //! \brief Запись логов в файл
    //! \param value true / false
    virtual void setLogToFile (bool value) = 0;

    //! \brief Задать максимальное количество файлов логирования
    //! \param maxNum Максимальное количество файлов логирования
    virtual void setMaxLogFilesNum (int maxNum) = 0;

    //! \brief Максимальный размер файла логирования (Мб)
    //! \param maxSize Максимальный размер файла
    //!
    //! При достижении максимума будет создан новый файл и подписан как соответствующая часть.
    virtual void setMaxLogFileSize (int maxSize) = 0;

    //! \brief Писать логи по указанному пути
    //! \param path Путь
    virtual void setLogsPath (const QString &path) = 0;

signals:
    //! \brief Сигнал, информирующий о запуске приложения
    void applicationStarted ();

    //! \brief Сигнал, информирующий о принудительном завершении приложения
    void applicationQuit ();

    //! \brief Сигнал с данными логов
    //! \param msgType Тип сообщения
    //! \param msg Текст сообщения
    //!
    //! Типы сообщений:
    //! - Debug
    //! - Warning
    //! - Critical
    void applicationLogData (const QString &msgType, const QString &msg);
};

#endif // IAPPLICATION_H
