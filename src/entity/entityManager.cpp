#include "entityManager.h"

#include "../implementation/uuidProvider.h"

uuids::uuid entityManager::addEntity(entity* newEntity) {
    const uuids::uuid id = uuidProvider::getNewUUID();
    this->entities[id] = std::unique_ptr<entity>(newEntity);
    return id;
}

entity* entityManager::getEntity(const uuids::uuid& entityId) {
    return this->entities[entityId].get();
}

void entityManager::init(engine* e) {
    for (const auto& entity : this->entities) {
        entity.second->init(e);
    }
}

void entityManager::update(engine* e) {
    for (const auto& entity : this->entities) {
        entity.second->update(e);
    }
}

void entityManager::deinit(engine* e) {
    for (const auto& entity : this->entities) {
        entity.second->deinit(e);
    }
}
