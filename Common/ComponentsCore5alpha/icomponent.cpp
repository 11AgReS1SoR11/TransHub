#include "icomponent.h"
#include "icomponent_p.h"
#include "componentmanager.h"
using namespace ComponentsCore;

IComponent::IComponent ()
    : d (new Internal::IComponentPrivate ())
{}

IComponent::~IComponent ()
{
    delete d;
    d = nullptr;
}

void IComponent::setLastError (const QString &error) {
    d->_spec->setLastError (error);
}

QString IComponent::lastError () const
{
    if (!d->_spec->errors ().isEmpty ())
        return d->_spec->errors ().last ();
    else
        return QString ("");
}

QStringList IComponent::errors () const {
    return d->_spec->errors ();
}

void IComponent::setLastWarning (const QString &warning) {
    d->_spec->setLastWarning (warning);
}

QString IComponent::lastWarning () const
{
    if (!d->_spec->warnings ().isEmpty ())
        return d->_spec->warnings ().last ();
    else
        return QString ("");
}

QStringList IComponent::warnings () const {
    return d->_spec->warnings ();
}

ComponentSpec *IComponent::spec () const {
    return d->_spec;
}

QMap<QString, QString> IComponent::arguments() const {
    return d->_spec->arguments ();
}

void IComponent::showProgressBar (bool isShow) {
    ComponentManager::showProgressBar (isShow);
}

void IComponent::setProgressBarMax (int maxValue) {
    ComponentManager::changeProgressBarMax (maxValue);
}

void IComponent::setProgressBarText (const QString &text) {
    ComponentManager::changeProgressBarText (text);
}

void IComponent::changeProgressBarValue (int value) {
    ComponentManager::changeProgressBarValue (value);
}

void IComponent::addObject (QObject* object) {
    ComponentManager::addObject (object);
}

void IComponent::removeObject (QObject* object) {
    ComponentManager::removeObject (object);
}
