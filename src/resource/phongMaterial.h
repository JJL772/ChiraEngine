#pragma once

#include <utility>

#include "material.h"
#include "resourceManager.h"

class phongMaterial : public material {
public:
    phongMaterial(const uuids::uuid& shader_, const uuids::uuid& diffuseId, const uuids::uuid& specularId):
        material(shader_), diffuse(resourceManager::getTexture(diffuseId)), specular(resourceManager::getTexture(specularId)) {
        this->diffuse->setTextureUnit(GL_TEXTURE0);
        this->specular->setTextureUnit(GL_TEXTURE1);
    }
    void compile() override {
        this->shaderPtr->use();
        this->shaderPtr->setUniform("material.diffuse", 0);
        this->shaderPtr->setUniform("material.specular", 1);
    }
    void use() override {
        this->diffuse->use();
        this->specular->use();
        this->shaderPtr->use();
    }
    void setShininess(float shininess = 32.0f) {
        this->shaderPtr->use();
        this->shaderPtr->setUniform("material.shininess", shininess);
    }
    void setLambertFactor(float lambertFactor = 1.0f) {
        this->shaderPtr->use();
        this->shaderPtr->setUniform("material.lambertFactor", lambertFactor);
    }
private:
    std::shared_ptr<texture> diffuse;
    std::shared_ptr<texture> specular;
};
