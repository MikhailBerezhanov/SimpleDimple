#include <stdexcept>

#include "sdl_texture.h"
#include "sdl_renderer.h"
#include "sdl_surface.h"

namespace GameEngine
{
    Texture::Texture(SDL_Renderer *renderer, SDL_Surface *surface) 
        : m_texture(SDL_CreateTextureFromSurface(renderer, surface))
    {
        if (! m_texture) {
            throw std::runtime_error("Unable to create texture: " + std::string(SDL_GetError()));
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
            throw std::runtime_error("Unable to create texture: " + std::string(SDL_GetError()));
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
            throw std::runtime_error("Unable to query: " + std::string(SDL_GetError()));
        }
        return Size2D{static_cast<size_t>(w), static_cast<size_t>(h)};
    }

    RGBColor Texture::get_color_mode() const
    {
        uint8_t r, g, b;
        if (SDL_GetTextureColorMod(m_texture, &r, &g, &b) < 0) {
            throw std::runtime_error("Unable to query color mode: " + std::string(SDL_GetError()));
        }
        return RGBColor{r,g,b};
    }

    void Texture::set_color_mode(const RGBColor &rgb) const
    {
        if (SDL_SetTextureColorMod(m_texture, rgb.r, rgb.g, rgb.b) < 0){
            throw std::runtime_error("Unable to set color mode: " + std::string(SDL_GetError()));
        }
    }


}; // namespace GameEngine