#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/other/stb_image.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

/// C / CPP
#include <memory>
#include <iostream>

/// classes
#include "../include/other/classes/Camera.h"
#include "../include/other/classes/Model.h"

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;

void processInput(GLFWwindow* window, Camera &camera);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
bool cameraInput = false;
float cameraSensitivity = 0.1f;
float cameraSpeed = 2.5f;
float FOV = 50;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main() {

    /// INITIALIZATION
    if(!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    if(window == nullptr)
    {
        std::cout << "Failed to create a window" << std::endl;
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -3;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    stbi_set_flip_vertically_on_load(true);

    /// IMGUI
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    /// PARAMETERS OPENGL
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cam.updateRotation();

    /// RESOURCES
    auto model = glm::mat4(1.0f);
    auto view = cam.getView();
    auto projection = glm::perspective(glm::radians(FOV), 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

    Model backpack("models/backpack/backpack.obj");

    ShaderProgram shader("shaders/model/model.vert", "shaders/model/model.frag");

    while(!glfwWindowShouldClose(window)) {
        /// INPUT
        processInput(window, cam);
        projection = glm::perspective(glm::radians(FOV), 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

        /// TIME
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /// CLEAR BUFFERS
        glBindVertexArray(0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        /// IMGUI NEW FRAME
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // camera
        ImGui::Text("Camera (%.2f, %.2f, %.2f)", cam.getPos().x, cam.getPos().y, cam.getPos().z);
        ImGui::Checkbox("Camera Input", &cameraInput);
        ImGui::InputFloat("Sensitivity", &cameraSensitivity);
        ImGui::InputFloat("Speed", &cameraSpeed);
        ImGui::SliderFloat("FOV", &FOV, 0.0f, 360.0f, "%.2f");
        // widgets...

        cam.setSensitivity(cameraSensitivity);
        cam.setSpeed(cameraSpeed);

        // SHADERS
        shader.use();

        /// RENDERING
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        view = cam.getView();


        /// SHADER UNIFORMS
        shader.setUniformM4("model", model);
        shader.setUniformM4("view", view);
        shader.setUniformM4("projection", projection);

        shader.setUniformV3("dirLight.direction", glm::vec3(0.0f, -1.0f, -1.0f));
        shader.setUniformV3("dirLight.ambient", glm::vec3(0.25f, 0.25f, 0.25f));
        shader.setUniformV3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setUniformV3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        /// DRAW CALLS

        backpack.Draw(shader);

        /// RENDER

        /// IMGUI RENDER
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /// OPENGL SWAP BUFFERS
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}

void processInput(GLFWwindow *window, Camera &camera) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        cameraInput = !cameraInput;
        if (cameraInput)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    bool keys[] = {false, false, false, false}; // W A S D
    if (cameraInput) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            keys[0] = true;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            keys[2] = true;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            keys[1] = true;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            keys[3] = true;
    }

    camera.updateDeltaTime(deltaTime);
    camera.updatePosition(keys);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (cameraInput)
        cam.updateRotation(xpos, ypos);
}