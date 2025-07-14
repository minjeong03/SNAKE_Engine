#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include "../ThirdParty/glm/glm.hpp"

class ObjectManager;
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-11
 */
class Shader;
class Texture;
class Mesh;

using GLuint = unsigned int;
using UniformValue = std::variant<
    int,
    float,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat4
>;

class Material {
    friend ObjectManager;
public:
    Material(Shader* _shader) :shader(_shader), instanceVBO(0), isInstancingEnabled(false) {}

    void SetTexture(const std::string& uniformName, Texture* texture)
    {
        textures[uniformName] = texture;
    }

    void SetUniform(const std::string& name, UniformValue value)
    {
        uniforms[name] = value;
    }

    [[nodiscard]] bool IsInstancingSupported() const;

    void EnableInstancing(bool enable, Mesh* mesh);

private:
    void Bind() const;

    void UnBind() const;

    void SendUniforms();

    void UpdateInstanceBuffer(const std::vector<glm::mat4>& transforms) const;
    Shader* shader;
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, UniformValue> uniforms;

    GLuint instanceVBO;
    bool isInstancingEnabled;
};
