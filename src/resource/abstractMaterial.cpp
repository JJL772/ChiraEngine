#include "abstractMaterial.h"
#include "../core/engine.h"
#include "resourceManager.h"

abstractMaterial::abstractMaterial(unsigned int shaderId) : abstractResource() {
    this->shader = resourceManager::getShader(shaderId);
}

const std::weak_ptr<shader>& abstractMaterial::getShader() {
    return this->shader;
}
