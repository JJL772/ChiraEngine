#pragma once

#include <string>
#include <vector>
#include <memory>
#include "shader.h"
#include "abstractResource.h"
#include "../implementation/uuidProvider.h"

class material : public abstractResource {
public:
    explicit material(const uuids::uuid& shaderId);
    void compile() override {}
    virtual void use();
    void discard() override {}
    [[nodiscard]] std::weak_ptr<shader> getShader() const;
protected:
    std::shared_ptr<shader> shaderPtr;
};
