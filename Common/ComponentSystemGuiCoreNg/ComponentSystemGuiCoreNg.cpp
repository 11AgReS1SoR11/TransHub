#include "ComponentSystemGuiCoreNg.h"

#include <QtCore/QDebug>

#include <ComponentsCore5alpha/ComponentManager>

ComponentSystemGuiCoreNg::ComponentSystemGuiCoreNg ()
{}

ComponentSystemGuiCoreNg::~ComponentSystemGuiCoreNg ()
{
    if (_guiCore != nullptr) {
        removeObject(_guiCore);
        delete _guiCore;
        _guiCore = nullptr;
    }
}

bool ComponentSystemGuiCoreNg::init ()
{

    // -- Init Component Object
    _guiCore = new SystemGuiCoreNg ();
    if (!_guiCore->initComponent ()) {
        qCritical () << "[ComponentSystemGuiCoreNg][init] Could not initialize 'SystemGuiCoreNg' object";

        delete _guiCore;
        _guiCore = nullptr;
        return false;
    }

    addObject (_guiCore);
    return true;
}

