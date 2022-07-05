#include "Texture.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <unordered_map>

namespace Coel {
    struct TextureCache {
        std::unordered_map<std::string, Texture> textures;
    };

    static TextureCache cache;

    static inline void toGlFormat(const TextureFormat format, uint32_t fmtData[3]) {
        switch (format) {
        case RED: fmtData[0] = GL_RED, fmtData[1] = GL_RED, fmtData[2] = GL_UNSIGNED_BYTE; break;
        case REDF: fmtData[0] = GL_RED, fmtData[1] = GL_RED, fmtData[2] = GL_FLOAT; break;
        case RGB: fmtData[0] = GL_RGB8, fmtData[1] = GL_RGB, fmtData[2] = GL_UNSIGNED_BYTE; break;
        case RGBA: fmtData[0] = GL_RGBA8, fmtData[1] = GL_RGBA, fmtData[2] = GL_UNSIGNED_BYTE; break;
        case RGB16F: fmtData[0] = GL_RGB16F, fmtData[1] = GL_RGB, fmtData[2] = GL_FLOAT; break;
        case RGBA16F: fmtData[0] = GL_RGBA16F, fmtData[1] = GL_RGBA, fmtData[2] = GL_FLOAT; break;
        case RGB32F: fmtData[0] = GL_RGB32F, fmtData[1] = GL_RGB, fmtData[2] = GL_FLOAT; break;
        case RGBA32F: fmtData[0] = GL_RGBA32F, fmtData[1] = GL_RGBA, fmtData[2] = GL_FLOAT; break;
        case Depth: fmtData[0] = GL_DEPTH_COMPONENT, fmtData[1] = GL_DEPTH_COMPONENT, fmtData[2] = GL_FLOAT; break;
        default: break;
        }
    }

    static inline constexpr GLenum toGlFilter(const TextureFilter mode) {
        switch (mode) {
        case Linear: return GL_LINEAR;
        default: return GL_NEAREST;
        }
    }
    static inline constexpr GLenum toGlWrap(const TextureWrap mode) {
        switch (mode) {
        case ClampToEdge: return GL_CLAMP_TO_EDGE;
        case ClampToBorder: return GL_CLAMP_TO_BORDER;
        default: return GL_REPEAT;
        }
    }

    TextureRef load(const char *const filepath) {
        auto filepath_str = std::string(filepath);
        auto tex_iter = cache.textures.find(filepath_str);
        if (tex_iter == cache.textures.end()) {
            // not found, create a new one
            Texture result;
            create(result, filepath);
            return &(cache.textures[filepath_str] = result);
        } else {
            // found, return the existing texture
            return &(*tex_iter).second;
        }
    }

    void create(Texture &tex, const char *const filepath) {
        int32_t channels;
        stbi_set_flip_vertically_on_load(false);
        stbi_uc *data = stbi_load(filepath, &tex.size.x, &tex.size.y, &channels, 0);
        TextureFormat format{};
        switch (channels) {
        case 1: format = RED; break;
        case 3: format = RGB; break;
        case 4: format = RGBA; break;
        default: break;
        }
        create(tex, tex.size, format, data);
        stbi_image_free(data);
    }

    void create(Texture &tex, const glm::ivec2 &size, const TextureFormat format, const uint8_t *const data) {
        tex.size = size;
        toGlFormat(format, tex.fmtData);
        glGenTextures(1, &tex.id);
        glBindTexture(GL_TEXTURE_2D, tex.id);
        glTexImage2D(GL_TEXTURE_2D, 0, tex.fmtData[0], tex.size.x, tex.size.y, 0, tex.fmtData[1], tex.fmtData[2], data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void create(Texture3D &tex, const glm::ivec3 &size, const TextureFormat format, const uint8_t *const data) {
        tex.size = size;
        toGlFormat(format, tex.fmtData);
        glGenTextures(1, &tex.id);
        glBindTexture(GL_TEXTURE_3D, tex.id);
        glTexImage3D(GL_TEXTURE_3D, 0, tex.fmtData[0], tex.size.x, tex.size.y, tex.size.z, 0, tex.fmtData[1], tex.fmtData[2], data);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void bind(const Texture &tex, const int32_t slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, tex.id);
    }
    void bind(const Texture3D &tex, const int32_t slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_3D, tex.id);
    }

    void setData(const Texture &tex, const uint8_t *const data) {
        glTextureSubImage2D(tex.id, 0, 0, 0, tex.size.x, tex.size.y, tex.fmtData[1], tex.fmtData[2], data);
    }

    void setMinFilter(Texture &tex, const TextureFilter mode) {
        glTextureParameteri(tex.id, GL_TEXTURE_MIN_FILTER, toGlFilter(mode));
    }
    void setMagFilter(Texture &tex, const TextureFilter mode) {
        glTextureParameteri(tex.id, GL_TEXTURE_MAG_FILTER, toGlFilter(mode));
    }
    void setWrapMode(Texture &tex, const TextureWrap mode) {
        glTextureParameteri(tex.id, GL_TEXTURE_WRAP_S, toGlWrap(mode));
        glTextureParameteri(tex.id, GL_TEXTURE_WRAP_T, toGlWrap(mode));
    }
    void setBorderColor(Texture &, const glm::vec4 &color) {
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, reinterpret_cast<const float *>(&color));
    }
    void destroy(Texture &tex) {
        glDeleteTextures(1, &tex.id); //
    }
} // namespace Coel
