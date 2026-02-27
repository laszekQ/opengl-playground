#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H
#include <string>
#include <vector>

#include "Mesh.h"
#include "ShaderProgram.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model {
public:
    Model(const char* path) {
        loadModel(path);
    }
    void Draw(ShaderProgram &shader);
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif //OPENGL_MODEL_H