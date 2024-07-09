#include <stdexcept>

#include "sdl_texture.h"
#include "sdl_renderer.h"

#define THROW_ERROR(message) \
    throw std::runtime_error(std::string(__func__) \
    + " " + std::string(message) \
    + ": " + std::string(SDL_GetError()));

namespace GameEngine
{
    Texture::Texture(SDL_Renderer *renderer, SDL_Surface *surface) 
        : m_texture(SDL_CreateTextureFromSurface(renderer, surface))
    {
        if (! m_texture) {
            THROW_ERROR("Unable to create texture");
        }
    }

    Texture::Texture(SDL_Renderer *renderer, uint32_t format, int access, const Size2D &size)
        : m_texture(
            SDL_CreateTexture(
                    renderer,
                    format,
                    access,
                    static_cast<int>(size.w),
                    static_cast<int>(size.h)
            )
        )
    {
        if (! m_texture) {
            THROW_ERROR("Unable to create texture");
        }
    }


    Texture::~Texture()
    {
        SDL_DestroyTexture(m_texture);
    }

    Size2D Texture::get_size() const
    {
        int w, h;
        if (SDL_QueryTexture(m_texture, nullptr, nullptr, &w, &h) < 0) {
            THROW_ERROR("Unable to query");
        }
        return Size2D{static_cast<size_t>(w), static_cast<size_t>(h)};
    }

    RGBColor Texture::get_color_mode() const
    {
        uint8_t r, g, b;
        if (SDL_GetTextureColorMod(m_texture, &r, &g, &b) < 0) {
            THROW_ERROR("Unable to query color mode");
        }
        return RGBColor{r,g,b};
    }

    void Texture::set_color_mode(const RGBColor &rgb) const
    {
        if (SDL_SetTextureColorMod(m_texture, rgb.r, rgb.g, rgb.b) < 0){
            THROW_ERROR("Unable to set color mode");
        }
    }

    std::tuple<uint32_t, int, int, int> Texture::query() const
    {
        uint32_t format;
        int access, w, h;
        if (SDL_QueryTexture(m_texture, &format, &access, &w, &h) < 0) {
            throw std::runtime_error("Unable to query: " + std::string(SDL_GetError()));
        }
        return std::make_tuple(format, access, w, h);
    }

    std::tuple<uint8_t, uint8_t, uint8_t> Texture::get_color_mode() const
    {
        uint8_t r, g, b;
        if(SDL_GetTextureColorMod(m_texture, &r, &g, &b) < 0) {
            throw std::runtime_error("Unable to query color mode: " + std::string(SDL_GetError()));
        }
        return std::make_tuple(r,g,b);
    }


} // namespace GameEngine