#include "Font.h"
#include "Debug.h"
#include "EngineContext.h"
#include "RenderManager.h"

Font::Font(RenderManager& renderManager,const std::string& ttfPath, uint32_t fontSize_)
{
    LoadFont(ttfPath, fontSize_);
    BakeAtlas(renderManager);
    fontSize = fontSize_;
}

Font::~Font()
{
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Font::LoadFont(const std::string& path, uint32_t fontSize)
{
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("Failed to init FreeType");

    if (FT_New_Face(ft, path.c_str(), 0, &face))
        throw std::runtime_error("Failed to load font: " + path);

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    atlasWidth = 0;
    atlasHeight = 0;
    for (unsigned char c = 32; c < 127; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        atlasWidth += face->glyph->bitmap.width;
        atlasHeight = std::max(atlasHeight, face->glyph->bitmap.rows);
    }
}

void Font::BakeAtlas(RenderManager& renderManager)
{
    uint32_t texWidth = atlasWidth;
    uint32_t texHeight = atlasHeight;

    std::vector<unsigned char> pixels(texWidth * texHeight, 0);
    uint32_t offsetX = 0;

    for (unsigned char c = 32; c < 127; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        FT_GlyphSlot g = face->glyph;

        for (uint32_t y = 0; y < g->bitmap.rows; ++y)
        {
            for (uint32_t x = 0; x < g->bitmap.width; ++x)
            {
                pixels[(y * texWidth) + offsetX + x] = g->bitmap.buffer[y * g->bitmap.width + x];
            }
        }

        Glyph glyph;
        glyph.size = { g->bitmap.width, g->bitmap.rows };
        glyph.bearing = { g->bitmap_left, g->bitmap_top };
        glyph.advance = g->advance.x;
        glyph.uvTopLeft = { (offsetX + g->bitmap.width) / float(texWidth), 0.0f };
        glyph.uvBottomRight = { offsetX / float(texWidth), g->bitmap.rows / float(texHeight) };

        glyphs[c] = glyph;
        offsetX += g->bitmap.width;
    }
    atlasTexture = std::make_unique<Texture>(pixels.data(), texWidth, texHeight,1);
    Shader* textShader = renderManager.GetShaderByTag("internal_text");
    material = std::make_unique<Material>(textShader);
    material->SetTexture("u_FontTexture", atlasTexture.get());
    material->SetUniform("u_Color", glm::vec4(1.0f));
}

const Glyph& Font::GetGlyph(char c) const
{
    auto it = glyphs.find(c);
    if (it != glyphs.end()) return it->second;
    return glyphs.at('?'); // fallback
}

glm::vec2 Font::GetTextSize(const std::string& text) const
{
    float maxWidth = 0.0f;
    float currentLineWidth = 0.0f;
    float totalHeight = static_cast<float>(fontSize);

    for (char c : text)
    {
        if (c == '\n')
        {
            maxWidth = std::max(maxWidth, currentLineWidth);
            currentLineWidth = 0.0f;
            totalHeight += static_cast<float>(fontSize);
            continue;
        }

        const Glyph& glyph = GetGlyph(c);
        currentLineWidth += (glyph.advance >> 6);
    }

    maxWidth = std::max(maxWidth, currentLineWidth);
    return { maxWidth, totalHeight };
}

//TODO: change structure after update mesh constructor  
Mesh* Font::GenerateTextMesh(const std::string& text, glm::vec2 origin)
{
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    uint32_t indexOffset = 0;
    glm::vec2 size = GetTextSize(text);
    origin -= size * 0.5f;

    float xCursor = origin.x;
    float yCursor = origin.y;

    float lineSpacing = static_cast<float>(fontSize);

    for (char c : text)
    {
        if (c == '\n')
        {
            xCursor = origin.x;
            yCursor -= lineSpacing;
            continue;
        }

        const Glyph& glyph = GetGlyph(c);
        float xpos = xCursor + static_cast<float>(glyph.bearing.x);
        float ypos = yCursor - static_cast<float>(glyph.size.y - glyph.bearing.y);
        float w = static_cast<float>(glyph.size.x);
        float h = static_cast<float>(glyph.size.y);

        float u0 = glyph.uvTopLeft.x;
        float v0 = glyph.uvTopLeft.y;
        float u1 = glyph.uvBottomRight.x;
        float v1 = glyph.uvBottomRight.y;
        std::swap(v0, v1);
        std::swap(u0, u1);

        vertices.insert(vertices.end(), {
            xpos,     ypos + h, 0.0f, u0, v1,  // top-left
            xpos,     ypos,     0.0f, u0, v0,  // bottom-left
            xpos + w, ypos,     0.0f, u1, v0,  // bottom-right
            xpos + w, ypos + h, 0.0f, u1, v1   // top-right
            });

        indices.insert(indices.end(), {
            indexOffset + 0, indexOffset + 1, indexOffset + 2,
            indexOffset + 0, indexOffset + 2, indexOffset + 3
            });

        xCursor += static_cast<float>(glyph.advance >> 6);
        indexOffset += 4;
    }

    return new Mesh(vertices, indices);
}

