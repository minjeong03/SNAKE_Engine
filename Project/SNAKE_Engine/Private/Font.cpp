#include "Font.h"
#include "Debug.h"
#include "EngineContext.h"
#include "RenderManager.h"
#include <sstream>
#include "gl.h"

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
                size_t dstIndex = (texHeight - 1 - y) * texWidth + offsetX + x;
                size_t srcIndex = y * g->bitmap.width + x;
                if (dstIndex < pixels.size())
                    pixels[dstIndex] = g->bitmap.buffer[srcIndex];
            }
        }

        Glyph glyph;
        glyph.size = { g->bitmap.width, g->bitmap.rows };
        glyph.bearing = { g->bitmap_left, g->bitmap_top };
        glyph.advance = g->advance.x;

        glyph.uvTopLeft = {
            (static_cast<float>(offsetX) + 0.5f) / static_cast<float>(texWidth),
            1.0f
        };
        glyph.uvBottomRight = {
            (offsetX + g->bitmap.width - 0.5f) / static_cast<float>(texWidth),
            1.0f - (static_cast<float>(g->bitmap.rows) / static_cast<float>(texHeight))
        };


        glyphs[c] = glyph;
        offsetX += g->bitmap.width;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    TextureSettings ts;

    atlasTexture = std::make_unique<Texture>(pixels.data(), texWidth, texHeight, 1, ts);
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
    int lineCount = 1;

    for (char c : text)
    {
        if (c == '\n')
        {
            maxWidth = std::max(maxWidth, currentLineWidth);
            currentLineWidth = 0.0f;
            ++lineCount;
            continue;
        }

        const Glyph& glyph = GetGlyph(c);
        currentLineWidth += (glyph.advance >> 6);
    }

    maxWidth = std::max(maxWidth, currentLineWidth);
    float totalHeight = static_cast<float>(fontSize * lineCount);

    return { maxWidth, totalHeight };
}


//TODO: change structure after update mesh constructor  
Mesh* Font::GenerateTextMesh(const std::string& text, TextAlignH alignH, TextAlignV alignV)
{
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    uint32_t indexOffset = 0;

    std::stringstream ss(text);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ss, line, '\n'))
        lines.push_back(line);

    float lineSpacing = static_cast<float>(fontSize);
    size_t lineCount = lines.size();

    std::vector<float> lineWidths;
    float maxLineWidth = 0.0f;
    for (const std::string& lineText : lines)
    {
        float lineWidth = 0.0f;
        for (char c : lineText)
            lineWidth += static_cast<float>(GetGlyph(c).advance >> 6);

        maxLineWidth = std::max(maxLineWidth, lineWidth);
        lineWidths.push_back(lineWidth);
    }

    float totalHeight = static_cast<float>(lineCount) * lineSpacing;
    float yStart = 0;
    switch (alignV)
    {
    case TextAlignV::Top:
        break;
    case TextAlignV::Middle:
        yStart += totalHeight * 0.5f - lineSpacing;
        break;
    case TextAlignV::Bottom:
        yStart += totalHeight - lineSpacing;
        break;
    }

    float yCursor = yStart;

    for (size_t i = 0; i < lineCount; ++i)
    {
        const std::string& lineText = lines[i];
        float lineWidth = lineWidths[i];

        float xCursor = 0;
        switch (alignH)
        {
        case TextAlignH::Left:
            break;
        case TextAlignH::Center:
            xCursor -= lineWidth * 0.5f;
            break;
        case TextAlignH::Right:
            xCursor -= lineWidth;
            break;
        }

        for (char c : lineText)
        {
            const Glyph& glyph = GetGlyph(c);

            float xpos = xCursor + static_cast<float>(glyph.bearing.x);
            float ypos = yCursor - static_cast<float>(glyph.size.y - glyph.bearing.y);
            float w = static_cast<float>(glyph.size.x);
            float h = static_cast<float>(glyph.size.y);

            float u0 = glyph.uvTopLeft.x;
            float v0 = glyph.uvTopLeft.y;
            float u1 = glyph.uvBottomRight.x;
            float v1 = glyph.uvBottomRight.y;


            vertices.insert(vertices.end(), {
                xpos,     ypos + h, 0.0f, u0, v0,
                xpos,     ypos,     0.0f, u0, v1,
                xpos + w, ypos,     0.0f, u1, v1,
                xpos + w, ypos + h, 0.0f, u1, v0
                });
            indices.insert(indices.end(), {
                indexOffset + 0, indexOffset + 1, indexOffset + 2,
                indexOffset + 0, indexOffset + 2, indexOffset + 3
                });

            xCursor += static_cast<float>(glyph.advance >> 6);
            indexOffset += 4;
        }

        yCursor -= lineSpacing;
    }

    return new Mesh(vertices, indices);
}


