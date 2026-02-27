#pragma once
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <initializer_list>

class ShaderProgram{
    unsigned int ID;
    bool linked = false;
public:
    ShaderProgram();
    ShaderProgram(const char* vertex_shader, const char* fragment_shader);
    unsigned int attachShader(const char* shader_path, unsigned int type);
    unsigned int compileShader(const char* path, unsigned int type); 
    std::string getSource(const char* path);
    void use();
    void linkProgram();
    bool isLinked();

    unsigned int getId();

    void setUniformi(const char* name, int value);
    void setUniformf(const char* name, float value);
    void setUniformV3(const char* name, glm::vec3 value);
    void setUniformV3(const char* name, std::initializer_list<float> values);
    void setUniformM4(const char* name, glm::mat4 value);
};
