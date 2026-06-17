#include "../../include/other/classes/Keyboard.h"

void Keyboard::KeyboardInit()
{
    for (auto i{32}; i <= 348; i++)
        keyState[i] = false;
}

bool Keyboard::GetKeyDown(GLFWwindow *window, int key)
{
    if (glfwGetKey(window, key) == GLFW_PRESS && !keyState[key]) {
        keyState[key] = true;
        return true;
    }
    else if (glfwGetKey(window, key) == GLFW_RELEASE)
    {
        keyState[key] = false;
        return false;
    }
    return false;
}
