//!
//! \file ISystemGuiCorePopUpElement.h
//!

#ifndef ISYSTEMGUICOREPOPUPELEMENT_H
#define ISYSTEMGUICOREPOPUPELEMENT_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtGui/QIcon>

//!
//! \class ISystemGuiCorePopUpElement
//! \brief Базовый класс всплывающего сообщения
//!
class ISystemGuiCorePopUpElement : public QObject
{
    Q_OBJECT

public:
    //!
    //! \brief Перечисление типов сообщений
    //!
    enum ISystemGuiCorePopUpType
    {
        ISystemGuiCorePopUpInfo = 0,    /**< информация */
        ISystemGuiCorePopUpOk,          /**< ок */
        ISystemGuiCorePopUpWarning,     /**< предупреждение */
        ISystemGuiCorePopUpCritical,    /**< критическое */
        ISystemGuiCorePopUpNotType      /**< тип не задан */
    };

public:
    //! \brief Деструктор класса
    virtual ~ISystemGuiCorePopUpElement() {}

    //! \brief Запросить заголовок сообщения
    //! \return Заголовок сообщения
    virtual const QString& getElementCaption() const = 0;

    //! \brief Запросить текст сообщения
    //! \return Текст сообщения
    virtual const QString& getElementDataText() const = 0;

    //! \brief Запросить иконку 16х16
    //! \return QIcon
    virtual const QIcon& getElementIcon_16x16() const = 0;

    //! \brief Запросить иконку 48х48
    //! \return QIcon
    virtual const QIcon& getElementIcon_48x48() const = 0;

    //! \brief Запросить дату-время добавления сообщения
    //! \return QDateTime
    virtual const QDateTime& getElementAddTime() const = 0;

    //! \brief Запросить тип сообщения
    //! \return Тип сообщения
    virtual ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType getElementType() const = 0;

signals:
    //! \brief Сигнал информирующий о двойном клике на элементе
    //! \param popupElement Элемент
    void doubleClickInElement (ISystemGuiCorePopUpElement* popupElement);

    //! \brief Сигнал информирующий о закрытии всплывающей подсказки
    //! \param popupElement Элемент
    void popUpClosed (ISystemGuiCorePopUpElement* popupElement);
};

#endif // ISYSTEMGUICOREPOPUPELEMENT_H
