#pragma once

#include <string>
#include "../core/abstractEngine.h"

class abstractComponent {
public:
    abstractComponent(const std::string& entity_, const std::string& name_);
    virtual void init(abstractEngine* e) = 0;
    virtual void update(abstractEngine* e) = 0;
    virtual void deinit(abstractEngine* e) = 0;
private:
    std::string entity;
    std::string name;
};
