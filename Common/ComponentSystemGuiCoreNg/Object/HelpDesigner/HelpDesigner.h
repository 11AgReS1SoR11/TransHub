#ifndef HELPDESIGNER_H
#define HELPDESIGNER_H

#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QMutex>

#include <IObject/ISystemGuiCore/IHelpDesigner.h>
#include <IObject/ISystemGuiCore/ISystemIconManager.h>
#include <IObject/ISystemGuiCore/ISystemGuiCore.h>

#include "HelpSection.h"
#include "HelpWindow.h"

//!
//! \file HelpDesigner.h
//! \class HelpDesigner
//! \brief Базовый класс конструктора файла справки
//!
class HelpDesigner : public QObject, public IHelpDesigner
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Класс-родитель
    explicit HelpDesigner (QObject *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~HelpDesigner();

    //! \brief Запросить раздел файла справки
    //!        Если раздел не найден, то он будет создан.
    //! \param title Заголовок раздела
    //! \return IHelpSection
    IHelpSection* helpSection (const QString &title) override;

    //! \brief Добавить раздел в файл справки
    //! \param section IHelpSection
    //! \return true / false
    bool addSectionInHelp (IHelpSection *section) override;

    //! \brief Запросить список разделов
    //! \return Список разделов
    QVector<HelpSection*> sections ();

    //! \brief Задать менеджер иконок
    //! \param iconManager Менеджер иконок
    void setIconManager (ISystemIconManager *iconManager);

    //! \brief Задать системное имя
    //! \param systemName Системное имя
    void setSystemName (const QString &systemName);

    //! \brief Инициализация файла справки
    void initHelp ();

    //! \brief Загрузка файлв справки из файлов
    void loadHelpFromFiles ();

    //! \brief Сканирование help файла
    //! \param filePath Путь до файла
    //! \return Список секций с данными
    QMap<QString /*section*/, QString /*data*/> scanHelpFile (const QString &filePath);

    //! \brief Инициализация секций справки
    //! \param parentSection Родительская секция
    //! \param signaturePath Путь до секции
    void initSectionSignature (HelpSection *parentSection = nullptr, const QString &signaturePath = QString(""));

    //! \brief Инициализация оглавления справки
    //! \param parentSection Родительская секция
    //! \param sectionNum Номер секции
    void initHelpContents (HelpSection *parentSection = nullptr, const QString &sectionNum = "");

    //! \brief Инициализация тела справки
    //! \param parentSection Родительская секция
    //! \param sectionNum Номер секции
    void initHelpData (HelpSection *parentSection = nullptr, const QString &sectionNum = "");

    //! \brief Есть файл справки
    //! \return true / false
    bool isEmpty();

    //! \brief Задать режим работы текущего пользователя
    //! \param userMode
    void setUserMode (ISystemGuiCore::UserMode userMode);

protected:
    //! \brief Искаль раздел по его заголовку
    //! \param title Заголовок раздела
    //! \param parentSection Родительский раздел
    //! \return Секция
    HelpSection *findSection (const QString &title, HelpSection *parentSection = nullptr);

private:
    QVector<HelpSection*> _sections;    /**< список секций */

    QString _systemName;                /**< системное имя */
    QString _helpFile;                  /**< файл справки */

    ISystemIconManager *_iconManager {nullptr};   /**< менеджер иконок */
    HelpWindow *_helpWindow {nullptr};            /**< окно справки */

    //! Режим работы текущего пользователя
    ISystemGuiCore::UserMode _userMode {ISystemGuiCore::USER_MODE};

public slots:
    //! \brief Слот отображения окна справки
    void showHelp ();

protected slots:
    //! \brief Слот обработки закрытия окна справки
    void slotWindowDestroyed ();
};

#endif // HELPDESIGNER_H
