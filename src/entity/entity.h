#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "../component/abstractComponent.h"
#include "../core/engine.h"

class entity {
public:
    virtual void init(engine* e);
    virtual void update(engine* e);
    virtual void deinit(engine* e);
    void addComponent(engine* e, const std::string& name, abstractComponent* component);
    abstractComponent* getComponent(const std::string& name);
private:
    std::unordered_map<std::string,std::unique_ptr<abstractComponent>> components;
};
