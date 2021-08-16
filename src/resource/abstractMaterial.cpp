#include "abstractMaterial.h"
#include "../core/engine.h"

abstractMaterial::abstractMaterial(const std::string& shader_) : abstractResource() {
    this->shaderName = shader_;
}

shader* abstractMaterial::getShader() {
    return engine::getShader(this->shaderName);
}
