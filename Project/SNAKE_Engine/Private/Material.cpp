#include "Material.h"
#include "Shader.h"
#include "Texture.h"

void Material::Bind() const
{
    m_shader->Use();

    int unit = 0;
    for (const auto& [uniformName, tex] : m_textures) {
        if (!tex) continue;
        tex->BindToUnit(unit);
        m_shader->SendUniform(uniformName, unit);
        ++unit;
    }

    for (const auto& [name, value] : m_uniforms) {
        std::visit([&](auto&& val) {
            m_shader->SendUniform(name, val);
            }, value);
    }
}
