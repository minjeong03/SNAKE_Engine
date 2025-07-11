#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include "../ThirdParty/glm/glm.hpp"
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-11
 */
class Shader;
class Texture;

using UniformValue = std::variant<
    int,
    float,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat4
>;

class Material {
public:
    Material(Shader* shader) :m_shader(shader)
    {
    }

    void SetTexture(const std::string& uniformName, Texture* texture)
    {
        m_textures[uniformName] = texture;
    }

    void SetUniform(const std::string& name, UniformValue value)
    {
        m_uniforms[name] = value;
    }

    void Bind() const;

private:
    Shader* m_shader;
    std::unordered_map<std::string, Texture*> m_textures;
    std::unordered_map<std::string, UniformValue> m_uniforms;
};
