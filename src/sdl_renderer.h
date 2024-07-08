#pragma once

#include <memory>

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine 
{
    class Renderer : public IRenderer
    {
        SDL_Renderer *m_renderer;
    public:
        Renderer(std::shared_ptr<IWindow> window, int index = -1, uint32_t/*SDL_RendererFlags*/ flags = 0);
        Renderer(std::shared_ptr<ISurface> surface);
        ~Renderer();

        SDL_Renderer *get_raw() const;

        // Size
        std::tuple<size_t, size_t> get_output_size() const override;
    };
};