#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "../component/abstractComponent.h"

class entity {
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void deinit() = 0;

    void addComponent(const std::string& name, abstractComponent* component);
    abstractComponent* getComponent(const std::string& name);
private:
    std::unordered_map<std::string,std::unique_ptr<abstractComponent>> components;
};
