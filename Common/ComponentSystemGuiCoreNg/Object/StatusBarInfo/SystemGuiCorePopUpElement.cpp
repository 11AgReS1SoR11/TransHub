#include "SystemGuiCorePopUpElement.h"

SystemGuiCorePopUpElement::SystemGuiCorePopUpElement(const QString guid,
                                                     const QString &caption,
                                                     const QString &dataText,
                                                     ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType elementType,
                                                     const QDateTime &addTime,
                                                     QIcon elementIcon_16x16,
                                                     QIcon elementIcon_48x48)
{
    _elementGuid = guid;
    _elementCaption = caption;
    _elementCaption = _elementCaption.trimmed();
    _elementCaption = _elementCaption.replace(",", ";");

    _elementDataText = dataText;
    _elementDataText = _elementDataText.trimmed();
    _elementDataText = _elementDataText.replace(",", ";");

    _elementType = elementType;
    _addTime = addTime;
    _elementIcon_16x16 = elementIcon_16x16;
    _elementIcon_48x48 = elementIcon_48x48;
}

SystemGuiCorePopUpElement::~SystemGuiCorePopUpElement()
{
    _elementGuid.clear();
    _elementCaption.clear();
    _elementDataText.clear();
}

void SystemGuiCorePopUpElement::activateElement()
{
    emit doubleClickInElement(this);
}

void SystemGuiCorePopUpElement::closeElement()
{
    emit popUpClosed(this);
}

const QIcon& SystemGuiCorePopUpElement::getElementIcon_16x16() const
{
    return _elementIcon_16x16;
}

const QIcon& SystemGuiCorePopUpElement::getElementIcon_48x48() const
{
    return _elementIcon_48x48;
}

const QString& SystemGuiCorePopUpElement::getElementGuid() const
{
    return _elementGuid;
}

const QString& SystemGuiCorePopUpElement::getElementCaption() const
{
    return _elementCaption;
}

const QString& SystemGuiCorePopUpElement::getElementDataText() const
{
    return _elementDataText;
}

const QDateTime& SystemGuiCorePopUpElement::getElementAddTime() const
{
    return _addTime;
}

ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType SystemGuiCorePopUpElement::getElementType() const
{
    return _elementType;
}
