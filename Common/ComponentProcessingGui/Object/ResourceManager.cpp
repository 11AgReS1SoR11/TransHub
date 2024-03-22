#include "ResourceManager.h"

QIcon ResourceManager::urgencyCategory (int value)
{
    switch (value) {
    case 0:
        return QIcon (":/icons/monolit_24x24.png");
    case 1:
        return QIcon (":/icons/vozduh_24x24.png");
    case 2:
        return QIcon (":/icons/raketa_24x24.png");
    case 3:
        return QIcon (":/icons/samolet_24x24.png");
    case 4:
        return QIcon (":/icons/obychnyi_24x24.png");
    default:
        return QIcon ();
    }
}
