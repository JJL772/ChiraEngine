#ifndef BASICGAMEENGINE_MESH_H
#define BASICGAMEENGINE_MESH_H


#include <vector>
#include <string>
#include "../utility/renderable.h"
#include "../loader/meshLoader.h"

class mesh : public renderable {
public:
    explicit mesh(meshLoader* loader, const std::string& filepath, int depthFunc = GL_LEQUAL);
    virtual ~mesh();
    void compile() override;
    void discard() override;
    void render(shader* shader) override;
private:
    int depthFunc;
    meshLoader* loader;
    std::string filepath;
    unsigned int vboHandle, vaoHandle, eboHandle;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
};


#endif //BASICGAMEENGINE_MESH_H
