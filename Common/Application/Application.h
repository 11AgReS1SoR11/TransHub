#ifndef APPLICATION_H
#define APPLICATION_H

#include "IApplication.h"

#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QtCore/QDateTime>

#include <iostream>
#include <qglobal.h>

#include <ComponentsCore5alpha/ComponentManager>

#define MAX_FILE_SIZE_MB    10
#define MAX_LOGS_NUM        1

//!
//! \file Application.h
//! \class Application
//! \brief Класс, управляющий потоком приложения с графическим интерфейсом и основными настройками.
//!
class Application : public IApplication
{
    Q_OBJECT

public:
    //! \brief Конструктор
    //! \param argc Количество входящих аргументов
    //! \param argv Входящие аргументы
    //! \param appFlags ApplicationFlags
    Application (int &argc, char **argv, int appFlags = ApplicationFlags);

    //! \brief Деструктор
    virtual ~Application ();

    //! \brief Статический метод получения объекта класса
    //! \return Объект класса
    static Application *instance ();

    //! \brief Вывод логов в консоль
    //! \param value true / false
    void setLogToConsole (bool value) override;

    //! \brief Отсылать логи на на удалённый сервер.
    //! \param eth Собственный интерфейс с которого отсылаются логи
    void setRemoteServerLog (const QString &eth, const QString &ip, unsigned int port) override;

    //! \brief Отсылать логи на AMQP сервер.
    //! \param amqpHost Адрес сервера
    //! \param amqpPort Порт сервера
    //! \param amqpUser Пользователь
    //! \param amqpPass Пароль
    //! \param amqpExchange Название exchange
    void setAmqpServerLog (const QString &eth, const QString &host, unsigned int port, const QString &user,
                           const QString &pass, const QString &exchange) override;

    //! \brief Запись логов в файл
    //! \param value true / false
    void setLogToFile (bool value) override;

    //! \brief Максимальный размер файла логирования (Мб)
    //! \param maxSize Максимальный размер файла
    //!
    //! При достижении максимума будет создан новый файл и подписан как соответствующая часть.
    void setMaxLogFileSize (int maxSize) override;

    //! \brief Задать максимальное количество файлов логирования
    //! \param maxNum Максимальное количество файлов логирования
    void setMaxLogFilesNum (int maxNum) override;

    //! \brief Писать логи по указанному пути
    //! \param path Путь
    void setLogsPath (const QString &path) override;

protected:
    int  _maxFileSize;          //!< максимальный размер файла логирования
    int  _maxLogsNum;           //!< максимальное количество файлов логирования
    bool _logToConsole {false}; //!< вывод логов в консоль
    bool _logToFile {false};    //!< запись логов в файл
    QString _logFileName;       //!< имя файла логирования

    QString _logPath;           //!< Писать логи по указанному пути

    QString _udpEth;            //!< Собственный сетевой интерфейс
    QString _udpHost;           //!< Ip адрес сервера логов.
    unsigned int _udpPort {0};  //!< port сервера логов.

    QString _amqpEth;           //!<
    QString _amqpHost;          //!<
    unsigned int _amqpPort {0}; //!<
    QString _amqpUser;          //!<
    QString _amqpPass;          //!<
    QString _amqpExchange;      //!<

    //! Инстанс
    static Application *_instance;

signals:
    //! \brief Сигнал, информирующий о начале инициализации приложения
    void applicationStartLog4Qt ();

public slots:
    //! \brief Слот обработки запуска приложения
    void slotStart ();

    //! \brief Слот инициализации логгера
    void initLog4Qt ();

    //! \brief Слот остановки менеджера компонентов
    void stopComponentManager ();

    //! \brief Слот выгрузки логгера
    void removeLog4Qt ();
};

#endif // APPLICATION_H
