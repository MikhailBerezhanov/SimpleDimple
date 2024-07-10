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

    std::shared_ptr<ISurface> Surface::create(const std::string &image_filename) {
        return std::shared_ptr<ISurface>(new Surface(image_filename));
    }

    void Surface::lock() const {
        if (SDL_LockSurface(m_surface) < 0) {
            throw std::runtime_error("Unable to lock surface: " + std::string(SDL_GetError()));
        }
    }

    void Surface::unlock() const {
        SDL_UnlockSurface(m_surface);
    }

} // namespace GameEngine
