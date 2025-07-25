#include "Font.h"
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include "Debug.h"
#include "EngineContext.h"
#include "RenderManager.h"
#include "gl.h"

static std::vector<char32_t> UTF8ToCodepoints(const std::string& text)
{
    std::vector<char32_t> result;
    size_t i = 0;

    while (i < text.size())
    {
        char32_t cp = 0;
        unsigned char byte = text[i];

        if ((byte & 0b10000000) == 0)
        {
            cp = byte;
            ++i;
        }
        else if ((byte & 0b11100000) == 0b11000000)
        {
            cp = (byte & 0b00011111) << 6;
            cp |= (text[i + 1] & 0b00111111);
            i += 2;
        }
        else if ((byte & 0b11110000) == 0b11100000)
        {
            cp = (byte & 0b00001111) << 12;
            cp |= (text[i + 1] & 0b00111111) << 6;
            cp |= (text[i + 2] & 0b00111111);
            i += 3;
        }
        else if ((byte & 0b11111000) == 0b11110000)
        {
            cp = (byte & 0b00000111) << 18;
            cp |= (text[i + 1] & 0b00111111) << 12;
            cp |= (text[i + 2] & 0b00111111) << 6;
            cp |= (text[i + 3] & 0b00111111);
            i += 4;
        }
        else
        {
            ++i;
            continue;
        }

        result.push_back(cp);
    }

    return result;
}


Font::Font(RenderManager& renderManager, const std::string& ttfPath, uint32_t fontSize_)
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
}

void Font::BakeAtlas(RenderManager& renderManager)
{
    std::unordered_set<char32_t> seen;

    for (char32_t c = 0x20; c <= 0x7E; ++c)
        seen.insert(c);

    for (char32_t c = 0xA0; c <= 0xFF; ++c)
        seen.insert(c);

    for (char32_t c = 0xAC00; c <= 0xD7A3; ++c)
        seen.insert(c);

    for (char32_t c = 0x3131; c <= 0x314E; ++c)
        seen.insert(c);

    for (char32_t c = 0x314F; c <= 0x3163; ++c)
        seen.insert(c);

    std::vector<char32_t> uniqueCharacters(seen.begin(), seen.end());
    std::sort(uniqueCharacters.begin(), uniqueCharacters.end());

    const int padding = 1;
    const int glyphsPerRow = 64;
    int maxGlyphWidth = 0, maxGlyphHeight = 0;

    for (char32_t c : uniqueCharacters)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
        maxGlyphWidth = std::max(maxGlyphWidth, (int)face->glyph->bitmap.width);
        maxGlyphHeight = std::max(maxGlyphHeight, (int)face->glyph->bitmap.rows);
    }

    int cellWidth = maxGlyphWidth + padding * 2;
    int cellHeight = maxGlyphHeight + padding * 2;
    int texWidth = glyphsPerRow * cellWidth;
    int texHeight = ((int)std::ceil(uniqueCharacters.size() / (float)glyphsPerRow)) * cellHeight;

    std::vector<unsigned char> pixels(texWidth * texHeight, 0);
    int offsetX = 0, offsetY = 0;

    for (char32_t codepoint : uniqueCharacters)
    {
        if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) continue;

        FT_GlyphSlot g = face->glyph;
        int w = g->bitmap.width;
        int h = g->bitmap.rows;
        bool hasBitmap = (w > 0 && h > 0);
        if (!hasBitmap && g->advance.x == 0)
            continue; 

        if (offsetX + cellWidth > texWidth)
        {
            offsetX = 0;
            offsetY += cellHeight;
        }

        int drawX = offsetX + padding;
        int drawY = offsetY + padding;
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                int dstX = drawX + x;
                int dstY = drawY + y;
                if (dstX < texWidth && dstY < texHeight)
                {
                    pixels[dstY * texWidth + dstX] = g->bitmap.buffer[y * g->bitmap.pitch + x];
                }
            }
        }

        Glyph glyph;
        glyph.size = { w, h };
        glyph.bearing = { g->bitmap_left, g->bitmap_top };
        glyph.advance = g->advance.x;
        glyph.uvTopLeft = { (float)drawX / texWidth, (float)drawY / texHeight };
        glyph.uvBottomRight = { (float)(drawX + w) / texWidth, (float)(drawY + h) / texHeight };


    	glyphs.insert({ codepoint, glyph });

        offsetX += cellWidth;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    TextureSettings ts;
    atlasTexture = std::make_unique<Texture>(pixels.data(), texWidth, texHeight, 1, ts);

    Shader* textShader = renderManager.GetShaderByTag("internal_text");
    material = std::make_unique<Material>(textShader);
    material->SetTexture("u_FontTexture", atlasTexture.get());
    material->SetUniform("u_Color", glm::vec4(1.0f));
}

const Glyph& Font::GetGlyph(char32_t c) const
{
    auto it = glyphs.find(c);
    if (it != glyphs.end())
        return it->second;

    static const char32_t fallbackCodepoint = U'?'; 

    auto fallbackIt = glyphs.find(fallbackCodepoint);
    if (fallbackIt != glyphs.end())
        return fallbackIt->second;

    static Glyph empty{};
    return empty;
}

glm::vec2 Font::GetTextSize(const std::string& text) const
{
    float maxWidth = 0.0f;
    float currentLineWidth = 0.0f;
    int lineCount = 1;

    std::vector<char32_t> codepoints = UTF8ToCodepoints(text);
    for (char32_t c : codepoints)
    {
        if (c == U'\n')
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
        std::vector<char32_t> u32Line = UTF8ToCodepoints(lineText);
        float lineWidth = 0.0f;
        for (char32_t c : u32Line)
            lineWidth += static_cast<float>(GetGlyph(c).advance >> 6);

        maxLineWidth = std::max(maxLineWidth, lineWidth);
        lineWidths.push_back(lineWidth);
    }

    float totalHeight = static_cast<float>(lineCount) * lineSpacing;
    float yStart = 0;
    if (alignV == TextAlignV::Middle)
        yStart += totalHeight * 0.5f - lineSpacing;
    else if (alignV == TextAlignV::Bottom)
        yStart += totalHeight - lineSpacing;

    float yCursor = yStart;

    for (size_t i = 0; i < lineCount; ++i)
    {
        const std::string& lineText = lines[i];
        float lineWidth = lineWidths[i];

        float xCursor = 0;
        if (alignH == TextAlignH::Center)
            xCursor -= lineWidth * 0.5f;
        else if (alignH == TextAlignH::Right)
            xCursor -= lineWidth;

        std::vector<char32_t> u32 = UTF8ToCodepoints(lineText);
        for (char32_t c : u32)
        {
            const Glyph& glyph = GetGlyph(c);
            float xpos = xCursor + (float)glyph.bearing.x;
            float ypos = yCursor - (float)(glyph.size.y - glyph.bearing.y);
            float w = (float)glyph.size.x;
            float h = (float)glyph.size.y;

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
