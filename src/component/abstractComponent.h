#pragma once

#include <string>
#include "../core/engine.h"

class abstractComponent {
public:
    abstractComponent(const std::string& entity_, const std::string& name_);
    virtual void init(engine* e) = 0;
    virtual void update(engine* e) = 0;
    virtual void deinit(engine* e) = 0;
private:
    std::string entity;
    std::string name;
};
