#include <stdexcept>

#include "sdl_renderer.h"
#include "sdl_window.h"
#include "sdl_surface.h"
#include "sdl_texture.h"

namespace GameEngine
{
    Renderer::Renderer(SDL_Window * window, int index, Uint32 flags) 
        : m_renderer(SDL_CreateRenderer(window, index, flags))
    {
        if (! m_renderer) {
            throw std::runtime_error("Unable to create renderer: " + std::string(SDL_GetError()));
        }
    }

    Renderer::~Renderer()
    {
        // destroy textures
        for(auto p : m_textures) {
            delete p;
        }
        m_textures.clear();

        SDL_DestroyRenderer(m_renderer);
    }

    const SDL_Renderer* Renderer::get_raw() const
    {
        return m_renderer;
    }

    std::tuple<size_t, size_t> Renderer::get_output_size() const
    {
        int w, h;
        if (SDL_GetRendererOutputSize(m_renderer, &w, &h) < 0) {
            throw std::runtime_error("Error retrieving renderer size: " + std::string(SDL_GetError()));
        }
        return std::make_tuple(static_cast<size_t>(w), static_cast<size_t>(h));
    }

    IRenderer& Renderer::add_texture(ISurface& surface)
    {
        auto raw_surface = dynamic_cast<Surface&>(surface).m_surface;
        m_textures.push_back(new Texture(m_renderer, raw_surface));
        return *this;
    }

    IRenderer& Renderer::add_texture(uint32_t format, int access, int width, int heigth)
    {
        m_textures.push_back(new Texture(m_renderer, format, access, width, heigth));
        return *this;
    }

    const std::list<ITexture*> & Renderer::get_textures() const
    {
        return m_textures;
    }


}; // namespace GameEngine