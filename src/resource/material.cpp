#include "material.h"
#include "resourceManager.h"

material::material(unsigned int shaderId) : abstractResource() {
    this->shader = shaderId;
}

void material::use() {
    resourceManager::getShader(this->shader)->use();
}

unsigned int material::getShader() const {
    return this->shader;
}
