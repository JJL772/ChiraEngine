#pragma once

#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include "../resource/abstractResource.h"
#include "../loader/abstractMeshLoader.h"
#include "abstractComponent.h"
#include "../resource/material.h"
#include "../implementation/uuidProvider.h"

class meshComponent : public abstractComponent {
public:
    explicit meshComponent(const uuids::uuid& entityId, const std::string& name, abstractMeshLoader* loader, const std::string& filepath_, const uuids::uuid& materialId, int depthFunc_ = GL_LEQUAL, bool backfaceCulling_ = true, int cullType_ = GL_BACK);
    virtual ~meshComponent();
    void setMaterial(const uuids::uuid& materialId);
    [[nodiscard]] const uuids::uuid& getMaterial() const;
    void init() override;
    void update() override;
    void deinit() override;
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
    uuids::uuid material;
    unsigned int vboHandle = -1, vaoHandle = -1, eboHandle = -1;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};
