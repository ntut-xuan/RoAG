#include <sstream>
#include <fstream>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "spdlog/spdlog.h"

#include "opengl_shader.hpp"

void Shader::Init(std::string shaderName, bool hasGeometryShader){
    spdlog::info("Shader: Init Phase");
    this->shaderName = shaderName;
    Shader::BindProgram(hasGeometryShader);
}

unsigned int Shader::LoadShader(std::string filepath, ShaderType type){
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filepath);
        std::stringstream stream;

        stream << file.rdbuf();

        file.close();

        code = stream.str();
    } catch (std::ifstream::failure& e){
        spdlog::error("Shader: Failed on loading shader file.");
    }

    const char* code_ptr = code.c_str();
    unsigned int shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &code_ptr, NULL);
    glCompileShader(shader_id);

    CheckCompileStatus(shader_id, type);

    return shader_id;
}

unsigned int Shader::LoadVertexShader(){
    spdlog::info("Shader: Load vertex shader and compile.");

    unsigned vertex_shader_id = LoadShader(SHADER_DIR + this->shaderName + ".vert", ShaderType::VERTEX);

    return vertex_shader_id;
}

unsigned int Shader::LoadFragmentShader(){
    spdlog::info("Shader: Load fragment shader and compile.");

    unsigned fragment_shader_id = LoadShader(SHADER_DIR + this->shaderName + ".frag", ShaderType::FRAGMENT);

    return fragment_shader_id;
}

unsigned int Shader::LoadGeometryShader(){
    spdlog::info("Shader: Load geometry shader and compile.");

    unsigned geometry_shader_id = LoadShader(SHADER_DIR + this->shaderName + ".geom", ShaderType::GEOMETRY);

    return geometry_shader_id;
}

void Shader::BindProgram(bool hasGeometryShader){
    spdlog::info("Shader: Load shaders and bind program.");

    unsigned int vertex_id = LoadVertexShader();
    unsigned int fragment_id = LoadFragmentShader();
    unsigned int geometry_id = 0;
    if(hasGeometryShader){
        geometry_id = LoadGeometryShader();
    }

    shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_id);
    glAttachShader(shader_program_id, fragment_id);
    if(hasGeometryShader){
        glAttachShader(shader_program_id, geometry_id);
    }
    glLinkProgram(shader_program_id);

    CheckLinkStatus();

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
    if (hasGeometryShader) {
      glDeleteShader(geometry_id);
    }

}

void Shader::CheckCompileStatus(unsigned shader_id, ShaderType type){
    int success;
    char infoLog[1024];

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(shader_id, 1024, NULL, infoLog);
        spdlog::error("Shader: Compile failed on {1} shader ID {0}. See Error message.", shader_id, (type == ShaderType::VERTEX ? "vertex" : "fragment"));
        spdlog::error("{0}", infoLog);
        exit(1);
    }
}

void Shader::CheckLinkStatus(){
    int success;
    char infoLog[1024];

    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

    if(!success){
        glGetProgramInfoLog(shader_program_id, 1024, NULL, infoLog);
        spdlog::error("Shader: Link failed on shader program. See Error message.");
        spdlog::error("{0}", infoLog);
        exit(1);
    }
}

void Shader::Use(){
    glUseProgram(shader_program_id); 
}

void Shader::SetUniformValue(const std::string &name, const glm::mat4 &mat) const {
    GLuint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::SetUniformValue(const std::string &name, const glm::mat3 &mat) const {
    GLuint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::SetUniformValue(const std::string &name, const glm::vec3 &vec) const {
    GLuint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vec));
}
void Shader::SetUniformValue(const std::string &name, const float val) const {
    GLuint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniform1f(location, val);
}
void Shader::SetUniformValue(const std::string &name, const int val) const {
    GLuint location = glGetUniformLocation(shader_program_id, name.c_str());
    glUniform1i(location, val);
}
