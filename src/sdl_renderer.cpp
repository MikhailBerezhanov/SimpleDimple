#include <stdexcept>

#include "sdl_renderer.h"
#include "sdl_window.h"

namespace GameEngine
{
    Renderer::Renderer(std::shared_ptr<IWindow> window, int index, uint32_t flags) 
        : m_renderer(nullptr)
    {
        auto win = dynamic_cast<Window*>(window.get());
        if (! win) {
            throw std::runtime_error("Invalid Window instance");
        }

        m_renderer = SDL_CreateRenderer(
            win->get_raw(),
            index,
            flags
        );

        if (! m_renderer) {
            throw std::runtime_error("Unable to create renderer from window: " + std::string(SDL_GetError()));
        }
    }

    Renderer::Renderer(std::shared_ptr<ISurface> surface) 
        : m_renderer(nullptr)
    {
        //TODO: DO
    }

    Renderer::~Renderer() 
    {
        SDL_DestroyRenderer(m_renderer);
    }

    SDL_Renderer* Renderer::get_raw() const
    {
        return m_renderer;
    }


}; // namespace GameEngine