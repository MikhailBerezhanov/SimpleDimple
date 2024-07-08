#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine
{
    class Surface : public ISurface
    {
        SDL_Surface *m_surface;
    public:
        Surface(const std::string &image_filename);
        ~Surface();
    };
};