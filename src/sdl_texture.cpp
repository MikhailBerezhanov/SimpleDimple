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


    }; // namespace GameEngine