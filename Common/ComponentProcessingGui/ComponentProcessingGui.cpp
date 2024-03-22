#include "ComponentProcessingGui.h"
#include "Object/Settings.h"

#include <QtCore/QDebug>
#include <ComponentsCore5alpha/ComponentManager>
using namespace ComponentsCore;

ComponentProcessingGui::ComponentProcessingGui ()
{}

ComponentProcessingGui::~ComponentProcessingGui ()
{
    if (_editor)
        delete _editor;
    Settings::deleteInstance ();
}

bool ComponentProcessingGui::init ()
{
    auto args = arguments ();
    auto it = args.cbegin ();
    auto const end = args.cend ();

    while (it != end) {
        if (it.key () == "--debug")
            Settings::instance ().debug_ = true;
        ++it;
    }

    auto guicore = ComponentManager::getObject<ISystemGuiCore> ();
    if (!guicore) {
        qCritical () << "[ComponentProcessingGui][init] Could not find 'ISystemGuiCore'";
        return false;
    }

    _editor = new ProcessingGui ();

    return true;
}
