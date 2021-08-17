#include "resourceManager.h"

shader* resourceManager::getShader(const uuids::uuid& id) {
    return (shader*) resourceManager::resources[id].get();
}

texture* resourceManager::getTexture(const uuids::uuid& id) {
    return (texture*) resourceManager::resources[id].get();
}

mesh* resourceManager::getMesh(const uuids::uuid& id) {
    return (mesh*) resourceManager::resources[id].get();
}

material* resourceManager::getMaterial(const uuids::uuid& id) {
    return (material*) resourceManager::resources[id].get();
}

uuids::uuid resourceManager::addResource(abstractResource* r) {
    r->compile();
    const uuids::uuid id = uuidProvider::getNewUUID();
    resourceManager::resources[id] = std::unique_ptr<abstractResource>(r);
    return id;
}

abstractResource* resourceManager::getResource(const uuids::uuid& id) {
    return resourceManager::resources[id].get();
}

const std::unordered_map<uuids::uuid, std::unique_ptr<abstractResource>>* resourceManager::getResources() {
    return &resourceManager::resources;
}

void resourceManager::removeResource(const uuids::uuid& id) {
    resourceManager::resources[id]->discard();
    resourceManager::resources.erase(id);
}
