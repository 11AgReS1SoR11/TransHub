#ifndef ICOMPONENTPRIVATE_H
#define ICOMPONENTPRIVATE_H

#include "icomponent.h"

namespace ComponentsCore
{

class ComponentSpec;

namespace Internal
{

class IComponentPrivate
{
public:
    ComponentSpec *_spec {nullptr};
};
}

}

#endif // ICOMPONENTPRIVATE_H

