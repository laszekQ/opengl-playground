#include "../../include/other/classes/Object.h"

int Object::objCount = 0;

Object::Object(const std::shared_ptr<Model> &model, const std::shared_ptr<ShaderProgram> &shader) {
    this->model = model;
    this->shader = shader;
    modelMatrix = glm::mat4(1.0f);
    id = objCount++;
}

Object::Object(const char *model, const char *vertShader, const char *fragShader) {
    this->model = std::make_shared<Model>(model);
    this->shader = std::make_shared<ShaderProgram>(vertShader, fragShader);
    modelMatrix = glm::mat4(1.0f);
    id = objCount++;
}

int Object::getId() const {
    return id;
}

void Object::render() {
    shader->use();
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    shader->setUniformM4("model", modelMatrix);
    model->Draw(*shader);
}

void Object::setModel(std::shared_ptr<Model> &model) {
    this->model = model;
}

void Object::setModel(const char *path) {
    model = std::make_shared<Model>(path);
}

void Object::setShader(std::shared_ptr<ShaderProgram> &shader) {
    this->shader = shader;
}

void Object::setShader(const char *vertShader, const char *fragShader) {
    this->shader = std::make_shared<ShaderProgram>(vertShader, fragShader);
}

void Object::translate(float x, float y, float z) {
    this->translationMatrix = glm::translate(iMatrix, glm::vec3(x, y, z));
}

void Object::rotate(float angle, float x, float y, float z) {
    this->rotationMatrix = glm::rotate(iMatrix, angle, glm::vec3(x, y, z));
}

void Object::scale(float x, float y, float z) {
    this->scaleMatrix = glm::scale(iMatrix, glm::vec3(x, y, z));
}