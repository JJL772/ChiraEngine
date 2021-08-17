#include "world.h"

void world::init(engine* e) {
    for (const auto& name : this->entities) {
        //e->getEntity(name)->init(e);
    }
}

void world::update(engine* e) {
    for (const auto& name : this->entities) {
        //e->getEntity(name)->update(e);
    }
}

void world::deinit(engine* e) {
    for (const auto& name : this->entities) {
        //e->getEntity(name)->update(e);
    }
}

abstractCamera* world::getPrimaryCamera() const {
    if (!this->primaryCamera) {
        chiraLogger::log(WARN, "world::getPrimaryCamera", "Must set camera in world::setPrimaryCamera first");
        return nullptr;
    }
    return this->primaryCamera;
}

void world::setPrimaryCamera(engine* e, unsigned int entityId) {
    //this->primaryCamera = (abstractCamera*) (((engine*) e)->getEntity(entityId));
}

void world::addEntity(unsigned int entityId) {
    this->entities.push_back(entityId);
}

void world::removeEntity(unsigned int entityId) {
    this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), entityId), this->entities.end());
}
