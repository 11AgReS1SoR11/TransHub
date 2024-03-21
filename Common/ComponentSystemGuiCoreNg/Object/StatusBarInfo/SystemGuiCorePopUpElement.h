#ifndef SYSTEMGUICOREPOPUPELEMENT_H
#define SYSTEMGUICOREPOPUPELEMENT_H

#include <IObject/ISystemGuiCore/ISystemGuiCorePopUpElement.h>

#include <QtWidgets/QListWidgetItem>
#include <QtCore/QDateTime>

//!
//! \file SystemGuiCorePopUpElement.h
//! \class SystemGuiCorePopUpElement
//! \brief Базовый класс всплывающего сообщения
//!
class SystemGuiCorePopUpElement : public ISystemGuiCorePopUpElement
{
    Q_OBJECT

public:
    //!
    //! \brief Конструктор класса
    //! \param guid Uuid объекта
    //! \param caption Заголовок
    //! \param dataText Текст
    //! \param elementType Тип
    //! \param addTime Время добавления
    //! \param elementIcon_16x16 Иконка 16x16
    //! \param elementIcon_48x48 Иконка 48x48
    //!
    SystemGuiCorePopUpElement(const QString guid,
                              const QString &caption,
                              const QString &dataText,
                              ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType elementType,
                              const QDateTime &addTime,
                              QIcon elementIcon_16x16 = QIcon(),
                              QIcon elementIcon_48x48 = QIcon());

    //! \brief Деструктор класса
    virtual ~SystemGuiCorePopUpElement();

    //! \brief Активировать элемент
    void activateElement();

    //! \brief Проинформировать о закрытии элемента
    void closeElement();

    //! \brief Получить uuid объекта
    //! \return Uuid объекта
    const QString& getElementGuid() const;

    //! \brief Запросить заголовок сообщения
    //! \return Заголовок сообщения
    virtual const QString& getElementCaption() const;

    //! \brief Запросить текст сообщения
    //! \return Текст сообщения
    virtual const QString& getElementDataText() const;

    //! \brief Запросить иконку 16х16
    //! \return QIcon
    virtual const QIcon& getElementIcon_16x16() const;

    //! \brief Запросить иконку 48х48
    //! \return QIcon
    virtual const QIcon& getElementIcon_48x48() const;

    //! \brief Запросить дату-время добавления сообщения
    //! \return QDateTime
    virtual const QDateTime& getElementAddTime() const;

    //! \brief Запросить тип сообщения
    //! \return Тип сообщения
    virtual ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType getElementType() const;

protected:
    QDateTime   _addTime;                   /**< время добавления */
    QString     _elementGuid;               /**< uuid объекта */
    QString     _elementCaption;            /**< заголовок */
    QString     _elementDataText;           /**< текст */
    ISystemGuiCorePopUpType _elementType;   /**< тип */
    QIcon _elementIcon_16x16;               /**< иконка 16х16 */
    QIcon _elementIcon_48x48;               /**< иконка 48х48 */
};

#endif // SYSTEMGUICOREPOPUPELEMENT_H
