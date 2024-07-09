#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine
{
    class Surface : public ISurface
    {
        SDL_Surface *m_surface;
        friend class Renderer;
    public:
        Surface(const std::string &image_filename);
        ~Surface();

        const SDL_Surface *get_raw() const;

        ISurface & lock() override;
        ISurface & unlock() override;
    };
};