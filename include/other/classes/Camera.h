#pragma once
#include <glm/gtc/type_ptr.hpp>

class Camera {
    glm::vec3   pos = glm::vec3(0.0f, 0.0f, 0.0f),
                front = glm::vec3(0.0f, 0.0f, -1.0f),
                up = glm::vec3(0.0f, 1.0f, 0.0f);
    float   yaw = -90.0f,
            pitch = 0.0f,
            last_x = 400.0f,
            last_y = 300.0f,
            deltaTime = 0.0f;
    bool firstMouse = true;
    float sensitivity = 0.1f;
    float speed = 2.5f;

    public:
    Camera(glm::vec3 pos, glm::vec3 target);
    void updateDeltaTime(float delta);
    void updateRotation(double xpos, double ypos);
    void updateRotation();
    void updatePosition(const bool keys[5]);
    void setSensitivity(float sensitivity);
    void setSpeed(float speed);
    [[nodiscard]] glm::mat4 getView() const;
    [[nodiscard]] glm::vec3 getPos() const;
    [[nodiscard]] glm::vec3 getFront() const;
};