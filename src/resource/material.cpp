#include "material.h"

#include "resourceManager.h"

material::material(const uuids::uuid& shaderId) : abstractResource() {
    this->shader = shaderId;
}

void material::use() {
    resourceManager::getShader(this->shader)->use();
}

const uuids::uuid& material::getShader() const {
    return this->shader;
}
