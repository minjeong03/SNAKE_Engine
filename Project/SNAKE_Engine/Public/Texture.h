#pragma once
#include <string>

/**
 * @brief Texture filtering modes used during sampling.
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

/**
 * @brief Texture wrapping modes for UV coordinates.
 */
enum class TextureWrap
{
    ClampToEdge,
    Repeat,
    MirroredRepeat,
    ClampToBorder
};

/**
 * @brief Texture configuration options including filter, wrap, and mipmap generation.
 */
struct TextureSettings
{
    TextureFilter minFilter = TextureFilter::Linear;  ///< Minification filter.
    TextureFilter magFilter = TextureFilter::Linear;  ///< Magnification filter.
    TextureWrap wrapS = TextureWrap::ClampToEdge;     ///< Horizontal wrap mode.
    TextureWrap wrapT = TextureWrap::ClampToEdge;     ///< Vertical wrap mode.
    bool generateMipmap = true;                       ///< Whether to generate mipmaps.
};

/**
 * @brief Loads and manages an OpenGL texture object.
 *
 * Supports configuration via TextureSettings and provides access to texture ID
 * for binding to specific texture units.
 */
class Texture
{
    friend Material;

public:
    /**
     * @brief Loads a texture from file with the given settings.
     * @param path File path to the texture image.
     * @param settings Filtering and wrapping options.
     */
    Texture(const std::string& path, const TextureSettings& settings = {});

    /**
     * @brief Deletes the OpenGL texture object.
     */
    ~Texture();

private:
    /**
     * @brief Binds this texture to the specified texture unit.
     * @param unit Texture unit index.
     */
    void BindToUnit(unsigned int unit) const;

    /**
     * @brief Unbinds this texture from the specified texture unit.
     * @param unit Texture unit index.
     */
    void UnBind(unsigned int unit) const;

    unsigned int id;
    int width, height, channels;
};
