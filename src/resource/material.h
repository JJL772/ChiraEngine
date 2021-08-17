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
    [[nodiscard]] const uuids::uuid& getShader() const;
protected:
    uuids::uuid shader;
};
