#include "world.h"

#include "../core/engine.h"

void world::init(abstractEngine* e) {
    auto* en = (engine*) e;
    for (const auto& name : this->entities) {
        en->getEntity(name)->init(e);
    }
}

void world::update(abstractEngine* e) {
    auto* en = (engine*) e;
    for (const auto& name : this->entities) {
        en->getEntity(name)->update(e);
    }
}

void world::deinit(abstractEngine* e) {
    auto* en = (engine*) e;
    for (const auto& name : this->entities) {
        en->getEntity(name)->update(e);
    }
}

abstractCamera* world::getPrimaryCamera() const {
    if (!this->primaryCamera) {
        chiraLogger::log(WARN, "world::getCamera", "Must set camera in world::setCamera first");
        return nullptr;
    }
    return this->primaryCamera;
}

void world::setPrimaryCamera(abstractEngine* e, const std::string& entity) {
    this->primaryCamera = (abstractCamera*) (((engine*) e)->getEntity(entity));
}

// todo: use shared pointers and weak pointers instead of throwing strings everywhere
