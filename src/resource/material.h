#pragma once

#include <string>
#include <vector>
#include <memory>
#include "shader.h"
#include "abstractResource.h"

class material : public abstractResource {
public:
    explicit material(unsigned int shaderId);
    void compile() override {}
    virtual void use();
    void discard() override {}
    [[nodiscard]] unsigned int getShader() const;
protected:
    unsigned int shader;
};
