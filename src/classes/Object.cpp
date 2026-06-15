#include "../../include/other/classes/Object.h"

#include <iostream>
#include <ostream>

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

glm::vec3 Object::getPosition() const {
    glm::vec3 pos(translationMatrix[3][0],
                  translationMatrix[3][1],
                  translationMatrix[3][2]);
    return pos;
}

glm::vec3 Object::getRotation() const { // TODO: figure out why the values appear halved
    glm::quat quaternionRotation = glm::quat_cast(rotationMatrix);
    glm::vec3 rot(glm::degrees(quaternionRotation.x),
                    glm::degrees(quaternionRotation.y),
                    glm::degrees(quaternionRotation.z));
    return rot;
}

glm::vec3 Object::getScale() const
{
    glm::vec3 siz = glm::vec3(scaleMatrix[0][0],
                  scaleMatrix[1][1],
                  scaleMatrix[2][2]);
    return siz;
}

void Object::translate(float x, float y, float z) {
    this->translationMatrix = glm::translate(translationMatrix, glm::vec3(x, y, z));
}

void Object::rotate(float angle, float x, float y, float z) {
    this->rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle), glm::vec3(x, y, z));
}

void Object::scale(float x, float y, float z) {
    this->scaleMatrix = glm::scale(scaleMatrix, glm::vec3(x, y, z));
}

void Object::setPosition(float x, float y, float z)
{
    translationMatrix = glm::translate(iMatrix, glm::vec3(x, y, z));
}

void Object::setRotation(float angleX, float angleY, float angleZ)
{
    rotationMatrix = glm::rotate(iMatrix, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Object::setSize(float x, float y, float z)
{
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(x, y, z));
}

