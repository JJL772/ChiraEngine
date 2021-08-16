#pragma once

#include <string>
#include "shader.h"
#include "texture.h"
#include "mesh.h"

class resourceManager {
public:
    static unsigned int addShader(shader* s);
    static std::weak_ptr<shader> getShader(unsigned int id);
    static const std::vector<std::shared_ptr<shader>>* getShaders();

    static unsigned int addTexture(texture* t);
    static std::weak_ptr<texture> getTexture(unsigned int id);
    static const std::vector<std::shared_ptr<texture>>* getTextures();

    static unsigned int addMesh(mesh* t);
    static std::weak_ptr<mesh> getMesh(unsigned int id);
    static const std::vector<std::shared_ptr<mesh>>* getMeshes();

    static unsigned int addMaterial(abstractMaterial* t);
    static std::weak_ptr<abstractMaterial> getMaterial(unsigned int id);
    static const std::vector<std::shared_ptr<abstractMaterial>>* getMaterials();

    static unsigned int addGenericResource(abstractResource* r);
    static std::weak_ptr<abstractResource> getGenericResource(unsigned int id);
    static const std::vector<std::shared_ptr<abstractResource>>* getResources();
private:
    static inline std::vector<std::shared_ptr<shader>> shaders{};
    static inline std::vector<std::shared_ptr<texture>> textures{};
    static inline std::vector<std::shared_ptr<mesh>> meshes{};
    static inline std::vector<std::shared_ptr<abstractMaterial>> materials{};
    static inline std::vector<std::shared_ptr<abstractResource>> resources{};
};
