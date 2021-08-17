#pragma once

#include <string>
#include "shader.h"
#include "texture.h"
#include "material.h"

class resourceManager {
public:
    static uuids::uuid addResource(abstractResource* r);
    static std::weak_ptr<shader> getShader(const uuids::uuid& id);
    static std::weak_ptr<texture> getTexture(const uuids::uuid& id);
    static std::weak_ptr<material> getMaterial(const uuids::uuid& id);
    static std::weak_ptr<abstractResource> getResource(const uuids::uuid& id);
    static void removeResource(const uuids::uuid& id);

    static const std::unordered_map<uuids::uuid, std::shared_ptr<abstractResource>>* getResources();
private:
    static inline std::unordered_map<uuids::uuid, std::shared_ptr<abstractResource>> resources{};
};
