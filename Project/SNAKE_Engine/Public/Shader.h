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
public:
    Shader();
    ~Shader();

    void AttachFromFile(ShaderStage stage, const std::string& filepath);
    void Link();
    void Use();
    void Unuse();

    void SendUniform(const std::string& name, int value);
    void SendUniform(const std::string& name, float value);
    void SendUniform(const std::string& name, const glm::vec2& value);
    void SendUniform(const std::string& name, const glm::vec3& value);
    void SendUniform(const std::string& name, const glm::vec4& value);
    void SendUniform(const std::string& name, const glm::mat4& value);

    GLuint GetProgramID() const { return programID; }

    bool SupportsInstancing() const;


private:
    GLuint programID = 0;
    std::vector<GLuint> attachedShaders;
    std::vector<ShaderStage> attachedStages;

    bool IsSupportInstancing = false;

    void CheckSupportsInstancing();
    std::string LoadShaderSource(const std::string& filepath);
    GLuint CompileShader(ShaderStage stage, const std::string& source);
};