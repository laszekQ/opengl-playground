
#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H
#include "ShaderProgram.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderProgram& shader);
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif //OPENGL_MESH_H