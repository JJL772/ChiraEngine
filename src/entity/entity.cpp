#include "entity.h"
#include "../utility/logger.h"

void entity::init(abstractEngine* e) {
    for (const auto& componentPair : this->components) {
        componentPair.second->init(e);
    }
}

void entity::update(abstractEngine* e) {
    for (const auto& componentPair : this->components) {
        componentPair.second->update(e);
    }
}

void entity::deinit(abstractEngine* e) {
    for (const auto& componentPair : this->components) {
        componentPair.second->deinit(e);
    }
}

void entity::addComponent(abstractEngine* e, const std::string& name, abstractComponent* component) {
    if (this->components.count(name) > 0) {
        this->components[name]->deinit(e);
        this->components[name].reset(component);
    } else {
        this->components[name] = std::unique_ptr<abstractComponent>(component);
        this->components[name]->init(e);
    }
}

abstractComponent* entity::getComponent(const std::string& name) {
    if (this->components.count(name) == 0) {
        chiraLogger::log(ERR, "entity::getComponent", "Component " + name + " is not recognized, check that you registered it properly");
        return nullptr;
    }
    return this->components.at(name).get();
}
