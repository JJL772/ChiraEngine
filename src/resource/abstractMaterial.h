#pragma once

#include <string>
#include <vector>
#include <memory>
#include "shader.h"
#include "abstractResource.h"

class abstractMaterial : public abstractResource {
public:
    explicit abstractMaterial(unsigned int shaderId);
    virtual void use() = 0;
    void discard() override {}
    virtual const std::weak_ptr<shader>& getShader();
protected:
    std::weak_ptr<shader> shader;
};
