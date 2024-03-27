#include "ComponentFull_Search.hpp"

#include <QtCore/QDebug>

#include <ComponentsCore5alpha/ComponentManager>

ComponentFull_Search::ComponentFull_Search ()
{}

ComponentFull_Search::~ComponentFull_Search ()
{
    if (_fullSearchInt != nullptr)
    {
        removeObject(_fullSearchInt);
        _fullSearchInt = nullptr;
    }
    if (_fullSearchDouble != nullptr)
    {
        removeObject(_fullSearchDouble);
        _fullSearchDouble = nullptr;
    }
}

bool ComponentFull_Search::init ()
{
    // -- Init Component Object
    _fullSearchInt = &Algo::Full_Search<int>::getInstance();
    addObject (_fullSearchInt);

    _fullSearchDouble = &Algo::Full_Search<double>::getInstance();
    addObject (_fullSearchDouble);

    return true;
}

