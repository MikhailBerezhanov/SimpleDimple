#include <stdexcept>

#include "sdl_surface.h"

#define THROW_ERROR(message) \
    throw std::runtime_error(std::string(__func__) \
    + " " + std::string(message) \
    + ": " + std::string(SDL_GetError()));

namespace GameEngine
{
    GameEngine::Surface::Surface(const std::string& image_filename) 
        : m_surface(IMG_Load(image_filename.c_str()))
    {
        if (! m_surface) {
            THROW_ERROR("Unable to create surface from");
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
            THROW_ERROR("Unable to lock surface");
        }
    }

    void Surface::unlock() const {
        SDL_UnlockSurface(m_surface);
    }

} // namespace GameEngine
