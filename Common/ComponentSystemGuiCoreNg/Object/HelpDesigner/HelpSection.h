#ifndef HELPSECTION_H
#define HELPSECTION_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QMap>
#include <QtCore/QFile>
#include <QtCore/QMutex>

#include <QtGui/QImage>

#include <IObject/ISystemGuiCore/IHelpSection.h>
#include <IObject/ISystemGuiCore/IHelpItem.h>

//!
//! \file HelpSection.h
//! \class HelpSection
//! \brief Базовый класс раздела файла справки
//!
class HelpSection : public QObject, public IHelpSection
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param title Заголовок
    //! \param parent Класс-родитель
    explicit HelpSection (const QString &title, QObject *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~HelpSection ();

    //! \brief Отметить как добавленное в help
    //! \param isAdded true / false
    void addedInHelp (bool isAdded);

    //! \brief Отметить, что является подразделом
    //! \param isSubsection true / false
    void setSubsection (bool isSubsection);

    //! \brief Добавлено ли в help
    //! \return true / false
    bool isAdded ();

    //! \brief Является ли подразделом
    //! \return true / false
    bool isSubsection () const override;

    //! \brief Поиск IHelpItem-а по его названию
    //! \param itemTitle Название IHelpItem-а
    //! \return IHelpItem
    IHelpItem* item (const QString& itemTitle);

    //! \brief Запросить список пунктов раздела
    //! \return Список пунктов раздела
    QVector<IHelpItem*> items ();

    //! \brief Запросить список разделов
    //! \return Список разделов
    QVector<HelpSection*> sections ();


    // - IHelpSection

    //! \brief Получить заголовок раздела
    //! \return Заголовок раздела
    QString title () const override;

    //! \brief Получить текст раздела
    //! \return Текст раздела
    QString text () const override;

    //! \brief Получить список используемых изображений
    //! \return Список используемых изображений
    //!
    //! key     - название изображения, используемого в тексте
    //! value   - изображение
    const QMap<QString, QImage>* imageResources () override;

    //! \brief Задать текст раздела
    //! \param text Текст раздела
    void setText (const QString &text) override;

    //! \brief Задать текст раздела из файла
    //! \param textFile Файл
    void setTextFile (QFile *textFile) override;

    //! \brief Задать список используемых изображений
    //! \param imageResources Список используемых изображений
    void setImageResources (const QMap<QString, QImage> &imageResources) override;

    //! \brief Добавить изображение в список используемых
    //! \param imageName Название изображения
    //! \param image Изображение
    //! \return true / false
    bool addImageResource (const QString &imageName, const QImage &image) override;

    //! \brief Добавить пункт раздела
    //! \param item Пункт раздела
    //! \return true / false
    bool addItem (IHelpItem *item) override;

    //! \brief Добавить подраздел
    //! \param subsection Подраздел
    //! \return true / false
    bool addSubsection (IHelpSection *subsection) override;

private:
    bool    _addedInHelp;                       /**< добавлен ли раздел в файл справки */
    bool    _isSubsection;                      /**< является ли подразделом */
    QString _title;                             /**< заголовок */
    QString _text;                              /**< текст */
    QMap<QString, QImage>   _imageResources;    /**< список используемых изображений */
    QVector<IHelpItem*>     _items;             /**< список пунктов раздела */
    QVector<HelpSection*>   _subsections;       /**< список подразделов */

    mutable QMutex  _mutex;                     /**< мьютекс */

    QImage scaleImage (QImage img);

    void scaleImageMap ();
};

#endif // HELPSECTION_H
