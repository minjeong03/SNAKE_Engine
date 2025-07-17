#pragma once
#include <string>

using FilePath = std::string;
/**
 * @brief Enumerates filtering strategies for texture magnification and minification.
 *
 * @details
 * Used to determine how pixel colors are sampled when a texture is scaled.
 * Options include nearest-neighbor and linear filtering, with or without mipmapping.
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
 * @brief Enumerates how texture coordinates outside the [0,1] range are handled.
 *
 * @details
 * Used to define how textures repeat or clamp on the horizontal (S) and vertical (T) axes.
 */
enum class TextureWrap
{
    ClampToEdge,
    Repeat,
    MirroredRepeat,
    ClampToBorder
};

/**
 * @brief Stores texture sampling behavior including filter and wrap modes.
 *
 * @details
 * Defines how a texture should be filtered (minification/magnification)
 * and wrapped (for out-of-bounds coordinates).
 * Passed as a configuration object when loading a Texture.
 *
 * @code
 * TextureSettings settings;
 * settings.wrapS = TextureWrap::Repeat;
 * settings.minFilter = TextureFilter::LinearMipmapLinear;
 * @endcode
 */
struct TextureSettings
{
    TextureFilter minFilter = TextureFilter::Linear;  ///< Used when shrinking the texture.
    TextureFilter magFilter = TextureFilter::Linear;  ///< Used when enlarging the texture.
    TextureWrap wrapS = TextureWrap::ClampToEdge;     ///< Horizontal wrap behavior.
    TextureWrap wrapT = TextureWrap::ClampToEdge;     ///< Vertical wrap behavior.
    bool generateMipmap = true;                       ///< Enable mipmap generation after loading.
};

/**
 * @brief Represents an OpenGL 2D texture loaded from file.
 *
 * @details
 * This class manages OpenGL texture creation, binding, and filtering.
 * It supports both basic and custom configuration and is used by Materials
 * or directly by users in custom rendering scenarios.
 *
 * @code
 * // Basic usage
 * Texture myTexture("assets/wall.png");
 *
 * // With custom settings
 * TextureSettings settings;
 * settings.wrapS = TextureWrap::Repeat;
 * settings.minFilter = TextureFilter::LinearMipmapLinear;
 * Texture customTex("assets/ground.jpg", settings);
 * @endcode
 */
class Texture
{
    friend class Material;

public:
    /**
     * @brief Loads an image file and creates an OpenGL texture object.
     *
     * @details
     * Creates the texture with OpenGL and applies settings like wrap mode and filters.
     *
     * @param path Path to the texture image (e.g., PNG, JPG).
     * @param settings Optional configuration object.
     */
    Texture(const FilePath& path, const TextureSettings& settings = {});

    /**
     * @brief Frees GPU memory used by the texture.
     *
     * @details
     * This is called automatically upon object destruction.
     */
    ~Texture();

private:
    /**
     * @brief Binds the texture to the specified OpenGL texture unit.
     *
     * @param unit Texture unit index (0 = GL_TEXTURE0, 1 = GL_TEXTURE1, etc.)
     */
    void BindToUnit(unsigned int unit) const;

    /**
     * @brief Unbinds this texture from a specific texture unit.
     *
     * @param unit Texture unit index to unbind.
     */
    void UnBind(unsigned int unit) const;

    unsigned int id;
    int width, height, channels;
};
