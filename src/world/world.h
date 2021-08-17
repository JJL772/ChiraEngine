#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "../entity/abstractCamera.h"
#include "../core/engine.h"

class engine;

class world {
public:
    void init(engine* e);
    void update(engine* e);
    void deinit(engine* e);

    [[nodiscard]] abstractCamera* getPrimaryCamera() const;
    void setPrimaryCamera(engine* e, unsigned int entityId);
    void addEntity(unsigned int entityId);
    void removeEntity(unsigned int entityId);
private:
    abstractCamera* primaryCamera = nullptr;
    std::vector<unsigned int> entities;
};
