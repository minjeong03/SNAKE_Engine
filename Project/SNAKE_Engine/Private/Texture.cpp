#include "RenderManager.h"
#include <iostream>
#include "glad/gl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Debug.h"
#include "stb_image.h"

//used anonymous namespace to hide these functions from other files
namespace
{
    GLint ConvertFilter(TextureFilter filter)
    {
        switch (filter)
        {
        case TextureFilter::Nearest:              return GL_NEAREST;
        case TextureFilter::Linear:               return GL_LINEAR;
        case TextureFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilter::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilter::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
        }
        return GL_LINEAR;
    }

    GLint ConvertWrap(TextureWrap wrap)
    {
        switch (wrap)
        {
        case TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case TextureWrap::Repeat:         return GL_REPEAT;
        case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case TextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
        }
        return GL_CLAMP_TO_EDGE;
    }
}

Texture::Texture(const std::string& path, const TextureSettings& settings):id(0),width(0),height(0),channels(0)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        SNAKE_ERR("Failed to load texture: " << path);
        return;
    }

    GLenum format = (channels == 4) ? GL_RGBA :
        (channels == 3) ? GL_RGB :
        (channels == 1) ? GL_RED : GL_RGB;

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    glTextureStorage2D(id, 1, format == GL_RGBA ? GL_RGBA8 : GL_RGB8, width, height);

    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, ConvertFilter(settings.minFilter));
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, ConvertFilter(settings.magFilter));
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, ConvertWrap(settings.wrapS));
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, ConvertWrap(settings.wrapT));

    glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

    if (settings.generateMipmap)
        glGenerateTextureMipmap(id);

    stbi_image_free(data);
}

Texture::~Texture()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
    }
}

void Texture::BindToUnit(unsigned int unit) const
{
    glBindTextureUnit(unit, id);
}

void Texture::UnBind(unsigned int unit) const
{
    glBindTextureUnit(unit, 0);
}
