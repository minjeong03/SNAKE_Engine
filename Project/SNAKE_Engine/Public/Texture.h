#pragma once
#include <string>

/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-11
 */

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
    friend Material;

public:
    Texture(const std::string& path, const TextureSettings& settings = {});

    ~Texture();
   
    [[nodiscard]] unsigned int GetID() const { return id; }

protected:
    void BindToUnit(unsigned int unit) const;
    void UnBind(unsigned int unit) const;

private:
    unsigned int id;
    int width, height, channels;
};

