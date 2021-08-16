#pragma once

#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include "../resource/abstractResource.h"
#include "../loader/abstractMeshLoader.h"
#include "../resource/material.h"

class mesh : public abstractResource {
public:
    explicit mesh(abstractMeshLoader* loader, const std::string& filepath_, unsigned int materialId, int depthFunc_ = GL_LEQUAL, bool backfaceCulling_ = true, int cullType_ = GL_BACK);
    virtual ~mesh();
    void setMaterial(unsigned int materialId);
    [[nodiscard]] unsigned int getMaterial() const;
    void compile() override;
    void render();
    void discard() override;
    glm::mat4* getModel() {
        return &(this->model);
    };
protected:
    glm::mat4 model;
private:
    int depthFunc;
    bool backfaceCulling;
    int cullType;
    bool compiled = false;
    abstractMeshLoader* loader;
    std::string filepath;
    unsigned int material, vboHandle = -1, vaoHandle = -1, eboHandle = -1;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};
