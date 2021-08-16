#pragma once

#include <string>
#include <vector>
#include <memory>
#include "shader.h"
#include "abstractResource.h"

class abstractMaterial : public abstractResource {
public:
    explicit abstractMaterial(const std::string& shader_);
    virtual void use() = 0;
    void discard() override {}
    virtual shader* getShader();
protected:
    std::string shaderName;
};
