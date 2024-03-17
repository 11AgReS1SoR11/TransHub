#ifndef IHELPITEM_H
#define IHELPITEM_H

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QMap>

#include <QtGui/QImage>

#define HTML_SYMBOL_TAB         "&nbsp;&nbsp;&nbsp;&nbsp;"
#define HTML_SYMBOL_NEW_LINE    "<br/>"

#define IMG_MAX_WIDTH 800

//!
//! \file IHelpItem.h
//! \class IHelpItem
//! \brief Базовый класс пункта раздела (подраздела) файла справки
//!
//! Удаление производится автоматически!
//!
//! 1. Поддерживаются HTML тэги.
//! 2. Поддерживаются ссылки на объекты в тексте следующего формата:
//!     <a href="scrollToMe">NAME</a>         - ссылка на объект, к которому следует перейти по клику мыши
//!     <a name="scrollToMe">DATA PATH</a>    - объект, к которому перейдем по клику мыши
//!     или
//!     <h1 id="scrollToMe">DATA PATH</h1>    - объект, к которому перейдем по клику мыши
//!     Значения параметра name (id) должны быть уникальными для ВСЕГО документа!
//!     Анализ повторов в рамках всего документа не производится!
//!
//! 3. Поддерживается отображение изображений следующего формата:
//!     <img src="test_image.png" /> где
//!     test_image.png - название изображения, которое вы положите в imageResources.
//!     Названия изображений должны быть уникальными для ВСЕГО документа!
//!     Анализ повторов в рамках всего документа производится на этапе генерации итогового файла!
//!
//! 4. Рекомендации по оформлению:
//! 4.1. Максимальный размер изображений: ширина - 800px; высота - 600px.
//! 4.2. Подпись к изображению курсивным шрифтом: <i>ПОДПИСЬ К ИЗОБРАЖЕНИЮ</i>
//! 4.3. Изображение и подпись к нему располагаются по центру: <center></center>
//!      Пример: <br/><center><img src="menu_tools.png" /><br/><i>Меню</i></center>
//!
//! 4.4. Часто используемые теги форматирования текста:
//! 4.4.1.  <font size=N>ТЕКСТ</font>   - размер шрифта N
//! 4.4.2.  <font color=N>ТЕКСТ</font>  - цвет текста N (цвет задается либо строчно (red, green и т.д.) или в шестнадцатиричной записи (#F2E4B9))
//! 4.4.2.  <b>ТЕКСТ</b>                - жирный текст
//! 4.4.3.  <i>ТЕКСТ</i>                - курсивный текст
//! 4.4.4.  <strike>ТЕКСТ</strike>      - зачеркнутый текст
//! 4.4.5.  <u>ТЕКСТ</u>                - подчеркнутый текст
//! 4.4.6.  <sup>ТЕКСТ</sup>            - верхний индекс
//! 4.4.7.  <sub>ТЕКСТ</sub>            - нижний индекс
//! 4.4.8.  <small>ТЕКСТ</small>        - малый текст
//! 4.4.9.  <big>ТЕКСТ</big>            - большой текст
//! 4.4.10. <center>ТЕКСТ</center>      - расположение по центру
//!
//! 4.5. Прочие теги и символы:
//! 4.5.1.  &nbsp;&nbsp;&nbsp;&nbsp;    - символ табуляции
//! 4.5.2.  <br/>                       - перенос на новую строку
//!
//! 4.6. Таблицы:
//! 4.6.1.  Форматирование таблицы выполнить по описаному ниже образцу:
//!         <table border=1 bordercolor=black width=100% cellpadding=5 cellspacing=0>
//!         <i>ПОДПИСЬ К ТАБЛИЦЕ</i> - подпись к таблице
//!         <tr bgcolor=#F2E4B9><th>header 1</td><th>header 2</td><th>header 3</td></tr> - заголовки столбцов
//!         <tr bgcolor=#FFFFE4><td>data 1 1</td><td>data 1 2</td><td>data 1 3</td></tr> - строка 1
//!         <tr bgcolor=#F2E4B9><td>data 2 1</td><td>data 2 2</td><td>data 2 3</td></tr> - строка 2
//!         <tr bgcolor=#FFFFE4><td>data 3 1</td><td>data 3 2</td><td>data 3 3</td></tr> - строка 3 и т.д.
//!         </table>
//!
//! 4.6.2.  Цвета строк таблицы:
//!         - #F2E4B9
//!         - #FFFFE4
//!
class IHelpItem : public QObject
{
public:
    //! \brief Конструктор класса
    //! \param title Заголовок пункта
    //! \param parent Родительский объект
    IHelpItem (const QString &title, QObject *parent = nullptr)
        : QObject (parent), _title (title) {}

    //! \brief Конструктор класса
    //! \param title Заголовок пункта
    //! \param text Текст пункта
    //! \param parent Родительский объект
    IHelpItem (const QString &title, const QString &text, QObject *parent = nullptr)
        : QObject (parent), _title (title), _text (text) {}

    //! \brief Конструктор класса
    //! \param title Заголовок пункта
    //! \param text Текст пункта
    //! \param imageResources Список используемых изображений
    //! \param parent Родительский объект
    IHelpItem (const QString &title, const QString &text, const QMap<QString, QImage> &imageResources, QObject *parent = nullptr)
        : QObject (parent), _title (title), _text (text), _imageResources (imageResources) {}

    //! \brief Конструктор класса
    //! \param title Заголовок пункта
    //! \param textFile Объект файла с текстом пункта
    //! \param parent Родительский объект
    IHelpItem (const QString &title, QFile *textFile, QObject *parent = nullptr)
        : QObject (parent), _title (title), _text ("")
    {
        if (!textFile->open (QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical () << qPrintable (QString ("[IHelpItem] Can not open file! Item title: %1; File: %2")
                                        .arg (_title, textFile->fileName ()));

            delete textFile;
            return;
        }

        _text = QString (textFile->readAll ());
        textFile->close ();

        if (_text.isEmpty ())
            qWarning () << qPrintable (QString ("[IHelpItem] Text file is Empty! Item title: %1; File: %2")
                                       .arg (_title, textFile->fileName ()));

        delete textFile;
    }

    //! \brief Конструктор класса
    //! \param title Заголовок пункта
    //! \param textFile Объект файла с текстом пункта
    //! \param imageResources Список используемых изображений
    //! \param parent Родительский объект
    IHelpItem (const QString &title, QFile *textFile, const QMap<QString, QImage> &imageResources, QObject *parent = nullptr)
        : QObject (parent), _title (title), _text (""), _imageResources (imageResources)
    {
        if (!textFile->open (QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical () << qPrintable (QString ("[IHelpItem] Can not open file! Item title: %1; File: %2")
                                        .arg (_title, textFile->fileName ()));

            delete textFile;
            return;
        }

        _text = QString (textFile->readAll ());
        textFile->close ();

        if (_text.isEmpty ())
            qWarning () << qPrintable (QString ("[IHelpItem] Text file is Empty! Item title: %1; File: %2")
                                       .arg (_title, textFile->fileName ()));

        delete textFile;
    }

    //! \brief Деструктор класса
    virtual ~IHelpItem()
    {
        _title.clear ();
        _text.clear ();
        _imageResources.clear ();
    }

    //! \brief Получить заголовок пункта
    //! \return Заголовок пункта
    QString title () const {
        return _title;
    }

    //! \brief Получить текст пункта
    //! \return Текст пункта
    QString text () const {
        return _text;
    }

    //! \brief Получить список используемых изображений
    //! \return Список используемых изображений
    //!
    //! key     - название изображения, используемого в тексте
    //! value   - изображение
    const QMap<QString, QImage>* imageResources () {
        return &_imageResources;
    }

    //! \brief Задать заголовок пункта
    //! \param title Заголовок пункта
    void setTitle (const QString &title) {
        _title = title;
    }

    //! \brief Задать текст пункта
    //! \param text Текст пункта
    void setText (const QString &text) {
        _text = text;
    }

    //! \brief Задать текст пункта из файла
    //!        Файл удаляется автоматически!
    //! \param textFile Объект файла
    void setTextFile (QFile *textFile)
    {
        if (!textFile->open (QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical () << qPrintable (QString ("[IHelpItem][setTextFile] Can not open file! Item title: %1; File: %2")
                                        .arg (_title, textFile->fileName ()));

            delete textFile;
            return;
        }

        _text = QString (textFile->readAll ());
        textFile->close ();

        if (_text.isEmpty ())
            qWarning () << qPrintable (QString ("[IHelpItem][setTextFile] Text file is Empty! Item title: %1; File: %2")
                                       .arg (_title, textFile->fileName ()));

        delete textFile;
    }

    //! \brief Задать список используемых изображений
    //! \param imageResources Список используемых изображений
    void setImageResources (const QMap<QString, QImage> &imageResources)
    {
        _imageResources = imageResources;
        scaleImageMap ();
    }

    //! \brief Добавить изображение в список используемых
    //! \param imageName Название изображения
    //! \param image Изображение
    //! \return true / false
    bool addImageResource (const QString &imageName, const QImage &image)
    {
        if (image.isNull ()) {
            qCritical () << qPrintable (QString ("[IHelpItem][addImageResource] Image is NULL! Add failed! Item title: %1")
                                        .arg (_title));
            return false;
        }
        if (_imageResources.contains (imageName)) {
            qCritical () << qPrintable (QString ("[IHelpItem][addImageResource] Image with this name already added! Add failed! Item title: %1")
                                        .arg (_title));
            return false;
        }

        QImage buffImage = scaleImage (image);
        if (buffImage.isNull ()) {
            qCritical () << qPrintable (QString ("[IHelpItem][addImageResource] Scale image failed! Image is NULL! Add failed! Item title: %1")
                                        .arg (_title));
            return false;
        }

        _imageResources.insert (imageName, buffImage);
        return true;
    }

private:
    QString _title;                         /**< заголовок пункта */
    QString _text;                          /**< текст пункта */
    QMap<QString, QImage> _imageResources;  /**< список используемых изображений */

    QImage scaleImage (QImage img)
    {
        int height = img.rect ().height ();
        int width = img.rect ().width ();

        if (width <= IMG_MAX_WIDTH)
            return img;

        double tempWidth = (double) width;
        double tempHeight = (double) height;
        double tempSize = tempWidth / ((double) IMG_MAX_WIDTH);
        double tempHeightImg = tempHeight/tempSize;

        return img.scaled (IMG_MAX_WIDTH, tempHeightImg);
    }

    void scaleImageMap ()
    {
        QMap<QString, QImage> buffImageResources;
        QMapIterator<QString, QImage> i (_imageResources);
        while (i.hasNext ()) {
            i.next ();
            QImage buffImage = scaleImage (i.value ());
            if (buffImage.isNull ()) {
                qCritical () << qPrintable (QString ("[IHelpItem][scaleImageMap] Scale image failed! Image is NULL! Add failed! Item title: %1")
                                            .arg (i.key ()));
                continue;
            }
            buffImageResources.insert (i.key (), buffImage);
        }
        _imageResources.clear ();
        _imageResources = buffImageResources;
    }
};

#endif // IHELPITEM_H
