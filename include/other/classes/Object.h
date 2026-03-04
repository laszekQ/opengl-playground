#ifndef OPENGL_PLAYGROUND_OBJECT_H
#define OPENGL_PLAYGROUND_OBJECT_H
#include <memory>

#include "Model.h"

class Object {
    static int objCount;
    int id;

    std::shared_ptr<Model> model;
    std::shared_ptr<ShaderProgram> shader;

    glm::mat4 modelMatrix{glm::mat4(1.0f)};
    glm::mat4 translationMatrix{glm::mat4(1.0f)};
    glm::mat4 rotationMatrix{glm::mat4(1.0f)};
    glm::mat4 scaleMatrix{glm::mat4(1.0f)};

    glm::mat4 iMatrix{glm::mat4(1.0f)};

public:
    Object(const std::shared_ptr<Model> &model, const std::shared_ptr<ShaderProgram> &shader);
    Object(const char* model, const char* vertShader, const char* fragShader);
    [[nodiscard]] int getId() const;

    void render();

    void setModel(std::shared_ptr<Model> &model);
    void setModel(const char* path);
    void setShader(std::shared_ptr<ShaderProgram> &shader);
    void setShader(const char* vertShader, const char* fragShader);

    [[nodiscard]] glm::vec3 getPosition() const;
    [[nodiscard]] glm::vec3 getRotation() const;
    [[nodiscard]] glm::vec3 getScale() const;

    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    void scale(float x, float y, float z);

    void setPosition(float x, float y, float z);
    void setRotation(float angleX, float angleY, float Z);
    void setSize(float x, float y, float z);
};

#endif //OPENGL_PLAYGROUND_OBJECT_H