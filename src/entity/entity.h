#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "../component/abstractComponent.h"

class entity {
public:
    virtual void init(abstractEngine* e);
    virtual void update(abstractEngine* e);
    virtual void deinit(abstractEngine* e);

    void addComponent(abstractEngine* e, const std::string& name, abstractComponent* component);
    abstractComponent* getComponent(const std::string& name);
private:
    std::unordered_map<std::string,std::unique_ptr<abstractComponent>> components;
};
