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

    Texture::Texture(SDL_Renderer *renderer, uint32_t format, int access, int width, int heigth) 
        : m_texture(
            SDL_CreateTexture(
                renderer, 
                format,
                access, 
                width, 
                heigth
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

    const SDL_Texture *Texture::get_raw() const
    {
        return m_texture;
    }

    std::tuple<size_t, size_t> Texture::get_size() const
    {
        int w, h;
        if (SDL_QueryTexture(m_texture, nullptr, nullptr, &w, &h) < 0) {
            throw std::runtime_error("Unable to query: " + std::string(SDL_GetError()));
        }
        return std::make_tuple(w, h);
    }

    std::tuple<uint8_t, uint8_t, uint8_t> Texture::get_color_mode() const
    {
        uint8_t r, g, b;
        if (SDL_GetTextureColorMod(m_texture, &r, &g, &b) < 0) {
            throw std::runtime_error("Unable to query color mode: " + std::string(SDL_GetError()));
        }
        return std::make_tuple(r,g,b);
    }

    void Texture::set_color_mode(uint8_t r, uint8_t g, uint8_t b) const 
    {
        if (SDL_SetTextureColorMod(m_texture, r, g, b) < 0){
            throw std::runtime_error("Unable to set color mode: " + std::string(SDL_GetError()));
        }
    }


    }; // namespace GameEngine