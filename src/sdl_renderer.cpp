#include <stdexcept>

#include "sdl_renderer.h"
#include "sdl_window.h"

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


}; // namespace GameEngine