#include <stdexcept>
#include <cstring> 

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
        SDL_DestroyRenderer(m_renderer);
    }

    Size2D Renderer::get_output_size() const
    {
        int w, h;
        if (SDL_GetRendererOutputSize(m_renderer, &w, &h) < 0) {
            throw std::runtime_error("Error retrieving renderer size: " + std::string(SDL_GetError()));
        }
        return Size2D{static_cast<size_t>(w), static_cast<size_t>(h)};
    }

    void Renderer::set_draw_color(const RGBColor &rgba) const {
        if (SDL_SetRenderDrawColor(m_renderer, rgba.r, rgba.g, rgba.b, rgba.a) < 0) {
            throw std::runtime_error("Error setting renderer color: " + std::string(SDL_GetError()));
        }
    }

    RGBColor Renderer::get_draw_color() const {
        RGBColor res;
        if (SDL_GetRenderDrawColor(m_renderer, &res.r, &res.g, &res.b, &res.a) < 0){
            throw std::runtime_error("Error getting renderer color: " + std::string(SDL_GetError()));
        }
        return res;
    }

    void Renderer::clear() const {
        SDL_RenderClear(m_renderer);
    }

    void Renderer::present() const {
        SDL_RenderPresent(m_renderer);
    }

    void Renderer::copy(std::shared_ptr<ITexture> &texture, const Rect* src_rect, const Rect* dest_rect) const {
        auto tex = dynamic_cast<Texture*>(texture.get());
        if (! tex) {
            throw std::runtime_error("Invalid texture");
        }

        if (SDL_RenderCopy(m_renderer, tex->m_texture, 
            reinterpret_cast<const SDL_Rect*>(src_rect),
            reinterpret_cast<const SDL_Rect*>(dest_rect)
            ) < 0) 
        {
            throw std::runtime_error("Error copying: " + std::string(SDL_GetError()));
        }
    }

    std::shared_ptr<ITexture> Renderer::create_texture(std::shared_ptr<ISurface> &surface)
    {
        auto surf = dynamic_cast<Surface*>(surface.get());
        auto tex = new Texture(m_renderer, surf->m_surface);
        return std::shared_ptr<Texture>(tex);
    }

    std::shared_ptr<ITexture> Renderer::create_texture(uint32_t format, int access, const Size2D &size)
    {
        auto tex = new Texture(m_renderer, format, access, size);
        return std::shared_ptr<Texture>(tex);
    }
    
} // namespace GameEngine