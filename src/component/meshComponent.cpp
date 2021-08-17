#include "meshComponent.h"
#include "../resource/resourceManager.h"
#include "../core/engine.h"

meshComponent::meshComponent(const uuids::uuid& entityId, const std::string& name, abstractMeshLoader* loader, const std::string& filepath_, const uuids::uuid& materialId, int depthFunc_, bool backfaceCulling_, int cullType_):
    abstractComponent(entityId, name), model(1.0f), vertices(), indices() {
    this->depthFunc = depthFunc_;
    this->backfaceCulling = backfaceCulling_;
    this->cullType = cullType_;
    this->loader = loader;
    this->filepath = filepath_;
    this->material = materialId;
}

meshComponent::~meshComponent() {
    if (this->compiled) {
        glDeleteVertexArrays(1, &(this->vaoHandle));
        glDeleteBuffers(1, &(this->vboHandle));
        glDeleteBuffers(1, &(this->eboHandle));
    }
}

void meshComponent::setMaterial(const uuids::uuid& materialId) {
    this->material = materialId;
}

const uuids::uuid& meshComponent::getMaterial() const {
    return this->material;
}

void meshComponent::init() {
    if (this->compiled) return;

    this->loader->loadMesh(this->filepath, &this->vertices, &this->indices);

    glGenVertexArrays(1, &(this->vaoHandle));
    glGenBuffers(1, &(this->vboHandle));
    glGenBuffers(1, &(this->eboHandle));

    glBindVertexArray(this->vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (this->vertices.size() * sizeof(vertex)), &this->vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (this->indices.size() * sizeof(unsigned int)), &this->indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, norm));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, col));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uvMap));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->compiled = true;
}

void meshComponent::update() {
    resourceManager::getMaterial(this->material).lock()->use();
    resourceManager::getShader(resourceManager::getMaterial(this->material).lock()->getShader()).lock()->setUniform("m", &(this->model));

    glDepthFunc(this->depthFunc);
    if (this->backfaceCulling) {
        glEnable(GL_CULL_FACE);
        glCullFace(this->cullType);
    } else {
        glDisable(GL_CULL_FACE);
    }
    glBindVertexArray(this->vaoHandle);
    glDrawElements(GL_TRIANGLES, (int) this->indices.size(), GL_UNSIGNED_INT, nullptr);
}

void meshComponent::deinit() {
    if (this->compiled) {
        glDeleteVertexArrays(1, &(this->vaoHandle));
        glDeleteBuffers(1, &(this->vboHandle));
        glDeleteBuffers(1, &(this->eboHandle));
    }
}
