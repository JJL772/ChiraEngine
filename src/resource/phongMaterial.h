#pragma once

#include <utility>

#include "material.h"
#include "resourceManager.h"

class phongMaterial : public material {
public:
    phongMaterial(unsigned int shader_, unsigned int diffuseId, unsigned int specularId) : material(shader_), diffuse(diffuseId), specular(specularId) {
        resourceManager::getTexture(this->diffuse)->setTextureUnit(GL_TEXTURE0);
        resourceManager::getTexture(this->specular)->setTextureUnit(GL_TEXTURE1);
    }
    void compile() override {
        class shader* s = resourceManager::getShader(this->shader);
        s->use();
        s->setUniform("material.diffuse", 0);
        s->setUniform("material.specular", 1);
    }
    void use() override {
        resourceManager::getTexture(this->diffuse)->use();
        resourceManager::getTexture(this->specular)->use();
        resourceManager::getShader(this->shader)->use();
    }
    void setShininess(float shininess = 32.0f) {
        class shader* s = resourceManager::getShader(this->shader);
        s->use();
        s->setUniform("material.shininess", shininess);
    }
    void setLambertFactor(float lambertFactor = 1.0f) {
        class shader* s = resourceManager::getShader(this->shader);
        s->use();
        s->setUniform("material.lambertFactor", lambertFactor);
    }
private:
    unsigned int diffuse;
    unsigned int specular;
};
