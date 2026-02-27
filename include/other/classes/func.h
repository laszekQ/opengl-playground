#pragma once 
#include <GLFW/glfw3.h>

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

