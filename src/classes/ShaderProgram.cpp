#include "../include/other/ShaderProgram.h"
#include <fstream>
#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>

ShaderProgram::ShaderProgram()
{
    ID = glCreateProgram();
}

ShaderProgram::ShaderProgram(const char* vertex_shader, const char* fragment_shader)
{
    ID = glCreateProgram();
    
    unsigned int vertex_id = compileShader(vertex_shader, GL_VERTEX_SHADER);
    unsigned int fragment_id = compileShader(fragment_shader, GL_FRAGMENT_SHADER);

    glAttachShader(ID, vertex_id);
    glAttachShader(ID, fragment_id);
    glLinkProgram(ID);

    char log[512];
    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, log);
        std::cout << log << std::endl;
    }
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
    linked = true;
}

unsigned int ShaderProgram::attachShader(const char* shader_path, unsigned int type)
{
    unsigned int id = compileShader(shader_path, type);
    glAttachShader(ID, id);
    return id;
}

unsigned int ShaderProgram::compileShader(const char* path, unsigned int type)
{
    char log[1024], src[1024];
    int success;

    unsigned int id = glCreateShader(type);
    std::string shader_source = getSource(path);
    const char* source = shader_source.c_str();
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderSource(id, 1024, NULL, src);
        glGetShaderInfoLog(id, 1024, NULL, log);
        std::cout << log << std::endl << src << std::endl;
    }
    return id;
}

std::string ShaderProgram::getSource(const char* path)
{
    std::ifstream fin(path);
    std::stringstream ss;

    ss << fin.rdbuf();

    fin.close();

    std::string source = ss.str();

    return source;
}

void ShaderProgram::use()
{
    glUseProgram(ID);
}

void ShaderProgram::linkProgram()
{
    glLinkProgram(ID);
    linked = true;
}
bool ShaderProgram::isLinked()
{
    return linked;
}

void ShaderProgram::setUniformi(const char* name, int value)
{
    int uniform_id = glGetUniformLocation(ID, name);
    glUniform1i(uniform_id, value);
}

void ShaderProgram::setUniformf(const char* name, float value)
{
    int uniform_id = glGetUniformLocation(ID, name);
    glUniform1f(uniform_id, value);
}

void ShaderProgram::setUniformV3(const char *name, glm::vec3 value)
{
    int uniform_id = glGetUniformLocation(ID, name);
    glUniform3fv(uniform_id, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniformV3(const char *name, std::initializer_list<float> values)
{
    if (values.size() != 3)
        {
        std::cout << "Failed to set uniform vec3: " << name << std::endl;
        return;
    }
    int uniform_id = glGetUniformLocation(ID, name);
    float x = *values.begin();
    float y = *(values.begin() + 1);
    float z = *(values.begin() + 2);
    glm::vec3 vec(x, y, z);
    glUniform3fv(uniform_id, 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniformM4(const char *name, glm::mat4 value)
{
    int uniform_id = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(value));
}



unsigned int ShaderProgram::getId()
{
    return ID;
}