#ifndef IHELPSECTION_H
#define IHELPSECTION_H

#include <QtCore/QString>

#include "IHelpItem.h"

//!
//! \file IHelpSection.h
//! \class IHelpSection
//! \brief Базовый класс раздела файла справки
//!
class IHelpSection
{
public:
    //! \brief Деструктор класса
    virtual ~IHelpSection() {}

    //! \brief Получить заголовок раздела
    //! \return Заголовок раздела
    virtual QString title () const = 0;

    //! \brief Получить текст раздела
    //! \return Текст раздела
    virtual QString text () const = 0;

    //! \brief Является ли подразделом
    //! \return true / false
    virtual bool isSubsection () const = 0;

    //! \brief Получить список используемых изображений
    //! \return Список используемых изображений
    //!
    //! key     - название изображения, используемого в тексте
    //! value   - изображение
    virtual const QMap<QString, QImage>* imageResources () = 0;

    //! \brief Задать текст раздела
    //! \param text Текст раздела
    virtual void setText (const QString &text) = 0;

    //! \brief Задать текст раздела из файла
    //!        Файл удаляется автоматически!
    //! \param textFile Файл
    virtual void setTextFile (QFile *textFile) = 0;

    //! \brief Задать список используемых изображений
    //! \param imageResources Список используемых изображений
    virtual void setImageResources (const QMap<QString, QImage> &imageResources) = 0;

    //! \brief Добавить изображение в список используемых
    //! \param imageName Название изображения
    //! \param image Изображение
    //! \return true / false
    virtual bool addImageResource (const QString &imageName, const QImage &image) = 0;

    //! \brief Добавить пункт раздела
    //! \param item Пункт раздела
    //! \return true / false
    virtual bool addItem (IHelpItem *item) = 0;

    //! \brief Добавить подраздел
    //! \param subsection Подраздел
    //! \return true / false
    virtual bool addSubsection (IHelpSection *subsection) = 0;
};

#endif // IHELPSECTION_H
