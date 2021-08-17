#pragma once

#include <string>
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "../implementation/uuidProvider.h"

class resourceManager {
public:
    static shader* getShader(const uuids::uuid& id);
    static texture* getTexture(const uuids::uuid& id);
    static mesh* getMesh(const uuids::uuid& id);
    static material* getMaterial(const uuids::uuid& id);

    static uuids::uuid addResource(abstractResource* r);
    static abstractResource* getResource(const uuids::uuid& id);
    static void removeResource(const uuids::uuid& id);

    static const std::unordered_map<uuids::uuid, std::unique_ptr<abstractResource>>* getResources();
private:
    static inline std::unordered_map<uuids::uuid, std::unique_ptr<abstractResource>> resources{};
};
