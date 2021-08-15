#pragma once

#include <string>

class abstractComponent {
public:
    abstractComponent(const std::string& entity_, const std::string& name_);
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void deinit() = 0;
private:
    std::string entity;
    std::string name;
};
