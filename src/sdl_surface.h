#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine
{
    class Surface : public ISurface {

        SDL_Surface *m_surface;
        Surface(const std::string &image_filename);
        friend class Renderer;
    public:

        static std::shared_ptr<ISurface> create(const std::string &image_filename);

        ~Surface();

        void lock() const override;
        void unlock() const override;
    };
};