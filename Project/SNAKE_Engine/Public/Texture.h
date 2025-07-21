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

public:
    Texture(const FilePath& path, const TextureSettings& settings = {});

    ~Texture();

private:
    void BindToUnit(unsigned int unit) const;

    void UnBind(unsigned int unit) const;

    unsigned int id;
    int width, height, channels;
};
