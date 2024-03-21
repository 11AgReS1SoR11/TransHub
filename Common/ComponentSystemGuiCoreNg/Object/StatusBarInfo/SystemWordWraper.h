#ifndef SYSTEMWORDWRAPER_H
#define SYSTEMWORDWRAPER_H

#include <QtCore/QString>
#include <QtCore/QStringList>

/*!
 * \file SystemWordWraper.h
 * \class SystemWordWraper
 * \brief Класс переноса строк
 */
class SystemWordWraper
{
public:
    /*!
     * \brief Конструктор класса
     */
    SystemWordWraper();

    /*!
     * \brief Деструктор класса
     */
    virtual ~SystemWordWraper();

    /*!
     * \brief Статический метод получения форматированного сообщения
     * \param[in] text Текст
     * \param[in] maxSimbolsInRow Максимальное количество символов в строке
     * \param[out] rowNum Количество строк
     * \return Форматированные текст
     */
    static QString getWrapText (const QString& text, int maxSimbolsInRow, int &rowNum);
};

#endif // SYSTEMWORDWRAPER_H
