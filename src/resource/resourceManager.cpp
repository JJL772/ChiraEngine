#include "resourceManager.h"

unsigned int resourceManager::addShader(shader* s) {
    resourceManager::shaders.emplace_back(s);
    return resourceManager::shaders.size() - 1;
}

std::weak_ptr<shader> resourceManager::getShader(unsigned int id) {
    return resourceManager::shaders[id];
}

const std::vector<std::shared_ptr<shader>>* resourceManager::getShaders() {
    return &resourceManager::shaders;
}

unsigned int resourceManager::addTexture(texture* t) {
    resourceManager::textures.emplace_back(t);
    return resourceManager::textures.size() - 1;
}

std::weak_ptr<texture> resourceManager::getTexture(unsigned int id) {
    return resourceManager::textures[id];
}

const std::vector<std::shared_ptr<texture>>* resourceManager::getTextures() {
    return &resourceManager::textures;
}

unsigned int resourceManager::addMesh(mesh* m) {
    resourceManager::meshes.emplace_back(m);
    return resourceManager::meshes.size() - 1;
}

std::weak_ptr<mesh> resourceManager::getMesh(unsigned int id) {
    return resourceManager::meshes[id];
}

const std::vector<std::shared_ptr<mesh>>* resourceManager::getMeshes() {
    return &resourceManager::meshes;
}

unsigned int resourceManager::addMaterial(abstractMaterial* m) {
    resourceManager::materials.emplace_back(m);
    return resourceManager::materials.size() - 1;
}

std::weak_ptr<abstractMaterial> resourceManager::getMaterial(unsigned int id) {
    return resourceManager::materials[id];
}

const std::vector<std::shared_ptr<abstractMaterial>>* resourceManager::getMaterials() {
    return &resourceManager::materials;
}

unsigned int resourceManager::addGenericResource(abstractResource* r) {
    resourceManager::resources.emplace_back(r);
    return resourceManager::resources.size() - 1;
}

std::weak_ptr<abstractResource> resourceManager::getGenericResource(unsigned int id) {
    return resourceManager::resources[id];
}

const std::vector<std::shared_ptr<abstractResource>>* resourceManager::getResources() {
    return &resourceManager::resources;
}
