#pragma once

#include <string>
#include <vector>
#include "../ThirdParty/glm/glm.hpp"

enum class ShaderStage
{
    Vertex,
    Fragment,
    Geometry,
    TessControl,
    TessEval,
    Compute
};

class RenderManager;
class Material;

using GLuint = unsigned int;
using GLenum = unsigned int;
using FilePath = std::string;

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

    [[nodiscard]] GLuint GetProgramID() const { return programID; }

private:
    void Use() const;

    void Unuse() const;

    bool SupportsInstancing() const;

    void Link();

    void AttachFromFile(ShaderStage stage, const FilePath& filepath);

    [[nodiscard]] std::string LoadShaderSource(const FilePath& filepath);

    [[nodiscard]] GLuint CompileShader(ShaderStage stage, const std::string& source);

    void CheckSupportsInstancing();

    GLuint programID;
    std::vector<GLuint> attachedShaders;
    std::vector<ShaderStage> attachedStages;

    bool isSupportInstancing;
};
