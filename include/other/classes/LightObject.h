#ifndef OPENGL_PLAYGROUND_LIGHTOBJECT_H
#define OPENGL_PLAYGROUND_LIGHTOBJECT_H

#include "Object.h"

class LightObject : public Object {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    LightObject(const std::shared_ptr<Model> &model, const std::shared_ptr<ShaderProgram> &shader);
    LightObject(const char* model, const char* vertShader, const char* fragShader);

    glm::vec3& getAmbient();
    glm::vec3& getDiffuse();
    glm::vec3& getSpecular();
};

#endif //OPENGL_PLAYGROUND_LIGHTOBJECT_H