#include "Settings.h"

Settings *Settings::_instance = nullptr;

Settings &Settings::instance ()
{
    if (!_instance)
        _instance = new Settings ();
    return *_instance;
}

void Settings::deleteInstance () {
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

bool Settings::debug () const {
    return debug_;
}
