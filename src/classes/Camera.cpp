#include "../include/other/Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 target)
{
    this->pos = pos;
    glm::vec3 direction = glm::normalize(pos - target);
    glm::vec3 right = glm::normalize(glm::cross(up, direction));
    up = glm::cross(direction, right);
    front = glm::normalize(direction);
}

void Camera::updateRotation(double xpos, double ypos)
{
    auto x = static_cast<float>(xpos);
    auto y = static_cast<float>(ypos);

    if (firstMouse)
    {
        last_x = x;
        last_y = y;
        firstMouse = false;
    }
    float xoffset = x - last_x;
    float yoffset = last_y - y;
    last_x = x;
    last_y = y;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 direction;
    direction.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    direction.y = static_cast<float>(sin(glm::radians(pitch)));
    direction.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front = glm::normalize(direction);
}

void Camera::updateRotation()
{
    glm::vec3 direction;
    direction.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    direction.y = static_cast<float>(sin(glm::radians(pitch)));
    direction.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front = glm::normalize(direction);
}


void Camera::updateDeltaTime(float delta)
{
    deltaTime = delta;
}

void Camera::updatePosition(const bool keys[5])
{
    const float speed = 2.5f * deltaTime;
    if (keys[0]) // W
        pos += speed * front;
    if (keys[1]) // A
        pos -= glm::normalize(glm::cross(front, up)) * speed;
    if (keys[2]) // S
        pos -= speed * front;
    if (keys[3]) // D
        pos += glm::normalize(glm::cross(front, up)) * speed;
}

glm::mat4 Camera::getView() const
{
    const glm::mat4 output = glm::lookAt(pos, pos + front, up);
    return output;
}

glm::vec3 Camera::getPos() const
{
    return pos;
}

glm::vec3 Camera::getFront() const
{
    return front;
}

