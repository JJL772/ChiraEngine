#pragma once

#include "texture.h"
#include "glad/gl.h"

class texture2d : public texture {
public:
    texture2d(const std::string& file, int format, int wrapModeU = GL_REPEAT, int wrapModeV = GL_REPEAT, int filterMode = GL_LINEAR, bool mipmaps = true);
    void compile() override;
    void use() override;
private:
    int format;
    int wrapModeU;
    int wrapModeV;
    int filterMode;
    bool mipmaps;
};
