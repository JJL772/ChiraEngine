#include "mesh.h"
#include "../core/engine.h"
#include "resourceManager.h"

mesh::mesh(abstractMeshLoader* loader, const std::string& filepath_, unsigned int materialId, int depthFunc_, bool backfaceCulling_, int cullType_) : abstractResource(), model(1.0f), vertices(), indices() {
    this->depthFunc = depthFunc_;
    this->backfaceCulling = backfaceCulling_;
    this->cullType = cullType_;
    this->loader = loader;
    this->filepath = filepath_;
    this->material = resourceManager::getMaterial(materialId);
}

mesh::~mesh() {
    if (this->compiled) {
        glDeleteVertexArrays(1, &(this->vaoHandle));
        glDeleteBuffers(1, &(this->vboHandle));
        glDeleteBuffers(1, &(this->eboHandle));
    }
}

void mesh::setMaterial(unsigned int materialId) {
    this->material = resourceManager::getMaterial(materialId);
}

std::weak_ptr<abstractMaterial> mesh::getMaterial() {
    return this->material;
}

void mesh::compile() {
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

void mesh::render() {
    if (auto mat = this->material.lock()) {
        mat->use();
        if (auto shdr = mat->getShader().lock()) {
            shdr->setUniform("m", &(this->model));
        } else {
            chiraLogger::log(ERR, "mesh::render", "Shader is missing");
        }
    } else {
        chiraLogger::log(ERR, "mesh::render", "Material is missing");
        return;
    }

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

void mesh::discard() {
    if (this->compiled) {
        glDeleteVertexArrays(1, &(this->vaoHandle));
        glDeleteBuffers(1, &(this->vboHandle));
        glDeleteBuffers(1, &(this->eboHandle));
    }
}
