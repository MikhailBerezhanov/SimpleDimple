#include <stdexcept>

#include "sdl_surface.h"

namespace GameEngine
{
    GameEngine::Surface::Surface(const std::string& image_filename) 
        : m_surface(IMG_Load(image_filename.c_str()))
    {
        if (! m_surface) {
            throw std::runtime_error("Unable to create surface from " + image_filename + ": " + std::string(SDL_GetError()));
        }
    }

    Surface::~Surface() 
    {
        SDL_FreeSurface(m_surface);
    }
    }; // namespace GameEngine
