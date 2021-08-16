#pragma once

#include <utility>

#include "abstractMaterial.h"
#include "../core/engine.h"
#include "resourceManager.h"

class phongMaterial : public abstractMaterial {
public:
    phongMaterial(unsigned int shader_, unsigned int diffuseId, unsigned int specularId) : abstractMaterial(shader_) {
        this->diffuse = resourceManager::getTexture(diffuseId);
        this->specular = resourceManager::getTexture(specularId);
        auto dif = this->diffuse.lock();
        auto spec = this->specular.lock();
        if (dif && spec) {
            dif->setTextureUnit(GL_TEXTURE0);
            spec->setTextureUnit(GL_TEXTURE1);
        } else {
            chiraLogger::log(ERR, "phongMaterial::ctor", "Diffuse texture and/or specular texture missing");
        }
    }
    void compile() override {
        auto shdr = this->shader.lock();
        if (shdr) {
            shdr->use();
            shdr->setUniform("material.diffuse", 0);
            shdr->setUniform("material.specular", 1);
        } else {
            chiraLogger::log(ERR, "phongMaterial::compile", "Shader missing");
        }
    }
    void use() override {
        auto dif = this->diffuse.lock();
        auto spec = this->specular.lock();
        auto shdr = this->shader.lock();
        if (dif && spec && shdr) {
            dif->use();
            spec->use();
            shdr->use();
        } else {
            chiraLogger::log(ERR, "phongMaterial::use", "Shader, diffuse texture and/or specular texture missing");
        }
    }
    void setShininess(float shininess = 32.0f) {
        auto shdr = this->shader.lock();
        if (shdr) {
            shdr->use();
            shdr->setUniform("material.shininess", shininess);
        } else {
            chiraLogger::log(ERR, "phongMaterial::setShininess", "Shader missing");
        }
    }
    void setLambertFactor(float lambertFactor = 1.0f) {
        auto shdr = this->shader.lock();
        if (shdr) {
            shdr->use();
            shdr->setUniform("material.lambertFactor", lambertFactor);
        } else {
            chiraLogger::log(ERR, "phongMaterial::setLambertFactor", "Shader missing");
        }
    }
private:
    std::weak_ptr<texture> diffuse;
    std::weak_ptr<texture> specular;
};
