#pragma once

#include <memory>
#include <unordered_map>
#include "entity.h"

class entityManager {
public:
    void init(engine* e);
    void update(engine* e);
    void deinit(engine* e);
    uuids::uuid addEntity(entity* newEntity);
    entity* getEntity(const uuids::uuid& entityId);
private:
    std::unordered_map<uuids::uuid, std::unique_ptr<entity>> entities{};
};
