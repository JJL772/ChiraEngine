#include "uuidProvider.h"

uuids::uuid uuidProvider::getNewUUID() {
    return uuidProvider::uuidGenerator();
}
