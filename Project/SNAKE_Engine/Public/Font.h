#pragma once

#include <memory>
#include <unordered_map>
#include <string>


#include "../ThirdParty/glm/glm.hpp"
#include "../ThirdParty/freetype/include/ft2build.h"
#include FT_FREETYPE_H

#include "Texture.h"
#include "Material.h"

class Camera2D;


struct EngineContext;

struct Glyph
{
    glm::ivec2 size;
    glm::ivec2 bearing;
    uint32_t advance;
    glm::vec2 uvTopLeft;
    glm::vec2 uvBottomRight;
};

class Font
{
public:
    Font(RenderManager& engineContext, const std::string& ttfPath, uint32_t fontSize);
    ~Font();

    Material* GetMaterial() const { return material.get(); }
    Mesh* GenerateTextMesh(const std::string& text, glm::vec2 origin = { 0.0f, 0.0f });
    const Glyph& GetGlyph(char c) const;
    glm::vec2 GetTextSize(const std::string& text) const;
    uint8_t GetRenderLayer() const { return renderLayer; } 
    void SetRenderLayer(uint8_t layer) { renderLayer = layer; }

private:
    void LoadFont(const std::string& path, uint32_t fontSize);
    void BakeAtlas(RenderManager& renderManager);

private:
    FT_Library ft;
    FT_Face face;

    uint32_t fontSize;

    std::unordered_map<char, Glyph> glyphs;
    std::unique_ptr<Texture> atlasTexture;
    std::unique_ptr<Material> material;

    uint32_t atlasWidth = 0;
    uint32_t atlasHeight = 0;
    uint8_t renderLayer = 0;
};
