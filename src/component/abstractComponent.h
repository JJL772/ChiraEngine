#pragma once

#include <uuid.h>
#include <string>

class abstractComponent {
public:
    abstractComponent(const uuids::uuid& entityId, const std::string& name_);
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void deinit() = 0;
    [[nodiscard]] const uuids::uuid& getEntity() const;
    const std::string& getName();
private:
    uuids::uuid entity;
    std::string name;
};
