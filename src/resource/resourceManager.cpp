#include "resourceManager.h"

uuids::uuid resourceManager::addResource(abstractResource* r) {
    r->compile();
    const uuids::uuid id = uuidProvider::getNewUUID();
    resourceManager::resources[id] = std::unique_ptr<abstractResource>(r);
    return id;
}

std::weak_ptr<shader> resourceManager::getShader(const uuids::uuid& id) {
    return std::dynamic_pointer_cast<shader>(resourceManager::resources[id]);
}

std::weak_ptr<texture> resourceManager::getTexture(const uuids::uuid& id) {
    return std::dynamic_pointer_cast<texture>(resourceManager::resources[id]);
}

std::weak_ptr<material> resourceManager::getMaterial(const uuids::uuid& id) {
    return std::dynamic_pointer_cast<material>(resourceManager::resources[id]);
}

std::weak_ptr<abstractResource> resourceManager::getResource(const uuids::uuid& id) {
    return resourceManager::resources[id];
}

void resourceManager::removeResource(const uuids::uuid& id) {
    resourceManager::resources[id]->discard();
    resourceManager::resources.erase(id);
}

const std::unordered_map<uuids::uuid, std::shared_ptr<abstractResource>>* resourceManager::getResources() {
    return &resourceManager::resources;
}
