#include "entity.h"

void entity::init(engine* e) {
    for (const auto& componentPair : this->components) {
        componentPair.second->init();
    }
}

void entity::update(engine* e) {
    for (const auto& componentPair : this->components) {
        componentPair.second->update();
    }
}

void entity::deinit(engine* e) {
    for (const auto& componentPair : this->components) {
        componentPair.second->deinit();
    }
}

void entity::addComponent(engine* e, const std::string& name, abstractComponent* component) {
    if (this->components.count(name) > 0) {
        this->components[name]->deinit();
        this->components[name].reset(component);
    } else {
        this->components[name] = std::unique_ptr<abstractComponent>(component);
        this->components[name]->init();
    }
}

abstractComponent* entity::getComponent(const std::string& name) {
    if (this->components.count(name) == 0) {
        chiraLogger::log(ERR, "entity::getComponent", "Component " + name + " is not recognized, check that you registered it properly");
        return nullptr;
    }
    return this->components.at(name).get();
}
