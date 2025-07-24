#pragma once
#include <string>

using FilePath = std::string;

enum class TextureFilter
{
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

enum class TextureWrap
{
    ClampToEdge,
    Repeat,
    MirroredRepeat,
    ClampToBorder
};

struct TextureSettings
{
    TextureFilter minFilter = TextureFilter::Linear;
    TextureFilter magFilter = TextureFilter::Linear;
    TextureWrap wrapS = TextureWrap::ClampToEdge;
    TextureWrap wrapT = TextureWrap::ClampToEdge;
    bool generateMipmap = true;
};

class Texture
{
    friend class Material;
    friend class RenderManager;
public:
    Texture(const FilePath& path, const TextureSettings& settings = {});
    Texture(const unsigned char* data, int width_, int height_, int channels_, const TextureSettings& settings = {});
    ~Texture();
private:
    void BindToUnit(unsigned int unit) const;

    void UnBind(unsigned int unit) const;

    void GenerateTexture(const unsigned char* data, const TextureSettings& settings);
    unsigned int id;
    int width, height, channels;
};
