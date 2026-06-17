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
#include "../include/other/classes/Object.h"
#include "../include/other/classes/Keyboard.h"

/// misc
#include "../include/other/misc.h"

constexpr unsigned int WINDOW_WIDTH = 1280;
constexpr unsigned int WINDOW_HEIGHT = 720;
constexpr unsigned int DEBUG_WIN_WIDTH = 300;

void processInput(GLFWwindow* window, Camera &camera);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
bool cameraInput = false;
float cameraSensitivity = 0.1f;
float cameraSpeed = 2.5f;
float FOV = 50;
bool depthTesting = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame;

float pos[3] = {0, 0, 0};
float rot[3] = {0, 0, 0};
float siz[3] = {1.0f, 1.0f, 1.0f};

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

    Keyboard::KeyboardInit();

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

    //Model backpack("models/backpack/backpack.obj");

    auto shader = std::make_shared<ShaderProgram>("shaders/model/model.vert", "shaders/model/model.frag");

    std::vector<std::shared_ptr<Object>> objects;
    objects.push_back(std::make_shared<Object>(std::make_shared<Model>("models/backpack/backpack.obj"), shader));
    objects.push_back(std::make_shared<Object>(std::make_shared<Model>("models/backpack/backpack.obj"), shader));

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
        ImGui::BeginChild("Camera", ImVec2(DEBUG_WIN_WIDTH, 130), true);
        ImGui::Text("Camera (%.2f, %.2f, %.2f)", cam.getPos().x, cam.getPos().y, cam.getPos().z);
        ImGui::Checkbox("Camera Input", &cameraInput);
        ImGui::InputFloat("Sens.", &cameraSensitivity);
        ImGui::InputFloat("Speed", &cameraSpeed);
        ImGui::SliderFloat("FOV", &FOV, 0.0f, 360.0f, "%.2f");
        if (ImGui::Checkbox("Depth testing", &depthTesting))
            shader->setUniformi("depthTesting", depthTesting);
        ImGui::EndChild();

        // directional light
        ImGui::BeginChild("Directional light", ImVec2(DEBUG_WIN_WIDTH, 100), true);
        ImGui::Text("DirLight (%.2f, %.2f, %.2f)", dirLightPos[0], dirLightPos[1], dirLightPos[2]);
        ImGui::ColorEdit3("Ambient", dirLightAmb);
        ImGui::ColorEdit3("Diffuse", dirLightDiff);
        ImGui::ColorEdit3("Specular", dirLightSpec);
        ImGui::EndChild();

        // object list
        const char* objNames[] = { "Backpack" , "Backpack1"};
        static int selectedItem = 0;

        ImGui::BeginChild("Object List", ImVec2(DEBUG_WIN_WIDTH, 100), true);
        if (ImGui::BeginListBox("Objects", ImVec2(DEBUG_WIN_WIDTH / 1.5, 150)))
        {
            for (int n = 0; n < IM_ARRAYSIZE(objNames); n++)
            {
                const bool isSelected = (selectedItem == n);

                if (ImGui::Selectable(objNames[n], isSelected))
                {
                    selectedItem = n;
                    const std::shared_ptr<Object>& selectedObj = objects[selectedItem];
                    pos[0] = selectedObj->getPosition().x;
                    pos[1] = selectedObj->getPosition().y;
                    pos[2] = selectedObj->getPosition().z;

                    rot[0] = selectedObj->getRotation().x;
                    rot[1] = selectedObj->getRotation().y;
                    rot[2] = selectedObj->getRotation().z;

                    siz[0] = selectedObj->getScale().x;
                    siz[1] = selectedObj->getScale().y;
                    siz[2] = selectedObj->getScale().z;
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        ImGui::EndChild();

        std::shared_ptr<Object> selectedObj = objects[selectedItem];

        // object inspector (single)
        ImGui::BeginChild("Object inspector", ImVec2(220, 100), true);
        ImGui::Text("Object %i (%.2f, %.2f, %.2f)", selectedObj->getId(),
                                                        selectedObj->getPosition().x,
                                                        selectedObj->getPosition().y,
                                                        selectedObj->getPosition().z);
        ImGui::Text("Rotation (%.2f, %.2f, %.2f)",  selectedObj->getRotation().x,
                                                        selectedObj->getRotation().y,
                                                        selectedObj->getRotation().z);
        ImGui::Text("Scale (%.2f, %.2f, %.2f)",     selectedObj->getScale().x,
                                                        selectedObj->getScale().y,
                                                        selectedObj->getScale().z);

        ImGui::SliderFloat3("Position", pos, -10.0f, 10.0f, "%.2f", true);
        ImGui::SliderFloat3("Rotation", rot, -180.0f, 180.0f, "%.2f", true);
        ImGui::SliderFloat3("Scale", siz, -10.0f, 10.0f, "%.2f", true);
        ImGui::EndChild();

        selectedObj->setPosition(pos[0], pos[1], pos[2]);
        selectedObj->setRotation(rot[0], rot[1], rot[2]);
        selectedObj->setSize(siz[0], siz[1], siz[2]);

        // widgets...

        cam.setSensitivity(cameraSensitivity);
        cam.setSpeed(cameraSpeed);

        // SHADERS
        shader->use();

        /// RENDERING
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        view = cam.getView();

        /// SHADER UNIFORMS
        //shader.setUniformM4("model", model);
        shader->setUniformM4("view", view);
        shader->setUniformM4("projection", projection);

        shader->setUniformV3("dirLight.direction", glm::vec3(dirLightPos[0], dirLightPos[1], dirLightPos[2]));
        shader->setUniformV3("dirLight.ambient", glm::vec3(dirLightAmb[0], dirLightAmb[1], dirLightAmb[2]));
        shader->setUniformV3("dirLight.diffuse", glm::vec3(dirLightDiff[0], dirLightDiff[1], dirLightDiff[2]));
        shader->setUniformV3("dirLight.specular", glm::vec3(dirLightSpec[0], dirLightSpec[1], dirLightSpec[2]));

        shader->setUniformV3("viewPos", cam.getPos());

        shader->setUniformf("material.shininess", 1.0f);

        /// DRAW CALLS

        for (const auto obj: objects) {
            obj->render();
        }

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

    if (Keyboard::GetKeyDown(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

    if (Keyboard::GetKeyDown(window, GLFW_KEY_V)) {
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