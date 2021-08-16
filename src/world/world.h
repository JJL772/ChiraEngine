#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include "../entity/abstractCamera.h"
#include "../core/abstractEngine.h"

class world {
public:
    void init(abstractEngine* e);
    void update(abstractEngine* e);
    void deinit(abstractEngine* e);

    [[nodiscard]] abstractCamera* getPrimaryCamera() const;
    void setPrimaryCamera(abstractEngine* e, unsigned int entityId);
    void addEntity(unsigned int entityId);
    void removeEntity(unsigned int entityId);
private:
    abstractCamera* primaryCamera = nullptr;
    std::vector<unsigned int> entities;
};
