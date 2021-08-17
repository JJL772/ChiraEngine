#include "material.h"

#include "resourceManager.h"

material::material(const uuids::uuid& shaderId) : abstractResource() {
    this->shaderPtr = resourceManager::getShader(shaderId).lock();
}

void material::use() {
    this->shaderPtr->use();
}

std::weak_ptr<shader> material::getShader() const {
    return this->shaderPtr;
}
