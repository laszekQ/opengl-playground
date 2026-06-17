#ifndef OPENGL_PLAYGROUND_KEYBOARD_H
#define OPENGL_PLAYGROUND_KEYBOARD_H

#include <map>
#include <GLFW/glfw3.h>

class Keyboard
{
    inline static std::map<int, bool> keyState; //32 -348

    public:
        Keyboard() = default;

        static void KeyboardInit();
        static bool GetKeyDown(GLFWwindow *window, int key);
};

#endif //OPENGL_PLAYGROUND_KEYBOARD_H
