#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

enum ShaderType {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER
};

class Shader {
private:
    std::string shaderName;
    unsigned int shader_program_id;

    unsigned int LoadShader(std::string filepath, ShaderType type);
    unsigned int LoadVertexShader();
    unsigned int LoadFragmentShader();
    unsigned int LoadGeometryShader();
    void BindProgram(bool hasGeometryShader);
    void CheckCompileStatus(unsigned int shader_id, ShaderType type);
    void CheckLinkStatus();
public:
    void Init(std::string shaderName, bool hasGeometryShader = false);
    void SetUniformValue(const std::string &name, const glm::mat4 &mat) const;
    void SetUniformValue(const std::string &name, const glm::mat3 &mat) const;
    void SetUniformValue(const std::string &name, const glm::vec3 &vec) const;
    void SetUniformValue(const std::string &name, const float val) const;
    void SetUniformValue(const std::string &name, const int val) const;
    void Use();
};

#endif
