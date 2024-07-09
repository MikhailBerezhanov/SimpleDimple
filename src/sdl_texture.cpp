#include <stdexcept>

#include "sdl_texture.h"
#include "sdl_renderer.h"
#include "sdl_surface.h"

namespace GameEngine
{
    Texture::Texture(std::shared_ptr<IRenderer> renderer, uint32_t format, int access, int width, int heigth) 
        : m_texture(nullptr)
    {
        auto rend = dynamic_cast<Renderer *>(renderer.get());
        if (! rend) {
            throw std::runtime_error("Invalid renderer instance");
        }

        m_texture = SDL_CreateTexture(
            rend->get_raw(), 
            format, 
            access, 
            width, 
            heigth
        );

        if (! m_texture) {
            throw std::runtime_error("Unable to create texture from renderer: " + std::string(SDL_GetError()));
        }
    }

    Texture::Texture(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ISurface> surface) 
        : m_texture(nullptr)
    {
        // TODO: DO
    }

    Texture::~Texture() 
    {
        SDL_DestroyTexture(m_texture);
    }

    SDL_Texture *Texture::get_raw() const
    {
        return m_texture;
    }


    }; // namespace GameEngine