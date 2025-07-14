#pragma once

#include <string>
#include <vector>
#include "../ThirdParty/glm/glm.hpp"
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-11
 */

class RenderManager;
class Material;

typedef unsigned int GLuint;
typedef unsigned int GLenum;

enum class ShaderStage
{
    Vertex,
    Fragment,
    Geometry,
    TessControl,
    TessEval,
    Compute
};

class Shader {
    friend Material;
    friend RenderManager;

public:
    Shader();
    ~Shader();

    void SendUniform(const std::string& name, int value) const;
    void SendUniform(const std::string& name, float value) const;
    void SendUniform(const std::string& name, const glm::vec2& value) const;
    void SendUniform(const std::string& name, const glm::vec3& value) const;
    void SendUniform(const std::string& name, const glm::vec4& value) const;
    void SendUniform(const std::string& name, const glm::mat4& value) const;

    GLuint GetProgramID() const { return programID; }



private:
    void Use() const;
    void Unuse() const;
    bool SupportsInstancing() const;

    void Link();
    void AttachFromFile(ShaderStage stage, const std::string& filepath);
    GLuint programID;
    std::vector<GLuint> attachedShaders;
    std::vector<ShaderStage> attachedStages;

    bool isSupportInstancing;

    void CheckSupportsInstancing();
    std::string LoadShaderSource(const std::string& filepath);
    GLuint CompileShader(ShaderStage stage, const std::string& source);
};