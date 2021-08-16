#pragma once

#include <string>
#include "shader.h"
#include "texture.h"
#include "mesh.h"

class resourceManager {
public:
    static unsigned int addShader(shader* s);
    static shader* getShader(unsigned int id);
    static const std::vector<std::unique_ptr<shader>>* getShaders();

    static unsigned int addTexture(texture* t);
    static texture* getTexture(unsigned int id);
    static const std::vector<std::unique_ptr<texture>>* getTextures();

    static unsigned int addMesh(mesh* t);
    static mesh* getMesh(unsigned int id);
    static const std::vector<std::unique_ptr<mesh>>* getMeshes();

    static unsigned int addMaterial(material* t);
    static material* getMaterial(unsigned int id);
    static const std::vector<std::unique_ptr<material>>* getMaterials();

    static unsigned int addGenericResource(abstractResource* r);
    static abstractResource* getGenericResource(unsigned int id);
    static const std::vector<std::unique_ptr<abstractResource>>* getResources();
private:
    static inline std::vector<std::unique_ptr<shader>> shaders{};
    static inline std::vector<std::unique_ptr<texture>> textures{};
    static inline std::vector<std::unique_ptr<mesh>> meshes{};
    static inline std::vector<std::unique_ptr<material>> materials{};
    static inline std::vector<std::unique_ptr<abstractResource>> resources{};
};
