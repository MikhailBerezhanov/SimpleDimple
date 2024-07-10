#include <stdexcept>
#include <cstring> 

#include "sdl_renderer.h"
#include "sdl_window.h"
#include "sdl_surface.h"
#include "sdl_texture.h"

#define THROW_ERROR(message) \
    throw std::runtime_error(std::string(__func__) \
    + " " + std::string(message) \
    + ": " + std::string(SDL_GetError()));

namespace GameEngine
{
    Renderer::Renderer(SDL_Window * window, int index, Uint32 flags) 
        : m_renderer(SDL_CreateRenderer(window, index, flags))
    {
        if (! m_renderer) {
            THROW_ERROR("Unable to create renderer");
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
            THROW_ERROR("Error retrieving renderer size");
        }
        return Size2D{static_cast<size_t>(w), static_cast<size_t>(h)};
    }

    void Renderer::set_draw_color(const RGBColor &rgba) const {

        if (SDL_SetRenderDrawColor(m_renderer, rgba.r, rgba.g, rgba.b, rgba.a) < 0) {
            THROW_ERROR("Error setting renderer color");
        }
    }

    RGBColor Renderer::get_draw_color() const {

        RGBColor res;
        if (SDL_GetRenderDrawColor(m_renderer, &res.r, &res.g, &res.b, &res.a) < 0){
            THROW_ERROR("Error getting renderer color");
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
            reinterpret_cast<const SDL_Rect*>(dest_rect)) < 0) {
            THROW_ERROR("Error copying");
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

    void Renderer::draw_point(const Pos2D &point) const {
        if (SDL_RenderDrawPoint(m_renderer, point.x, point.y) < 0) {
            THROW_ERROR("Error drawing point");
        }
    }

    void Renderer::draw_points(const std::vector<Pos2D> &points) const {
        if (SDL_RenderDrawPoints(m_renderer,
                                 reinterpret_cast<const SDL_Point*>(points.data()),
                                 static_cast<int>(points.size())) < 0)
        {
            THROW_ERROR("Error drawing points");
        }
    }

    void Renderer::draw_line(const Pos2D &start, const Pos2D &end) const {
        if (SDL_RenderDrawLine(m_renderer, start.x, start.y, end.x, end.y) < 0) {
            THROW_ERROR("Error drawing line");
        }
    }

    void Renderer::draw_lines(const std::vector<Pos2D> &points) const {
        if (SDL_RenderDrawLines(m_renderer,
                                reinterpret_cast<const SDL_Point*>(points.data()),
                                static_cast<int>(points.size())) < 0)
        {
            THROW_ERROR("Error drawing lines");
        }
    }

    void Renderer::draw_rect(const Rect &rect) const {
        if (SDL_RenderDrawRect(m_renderer,
                               reinterpret_cast<const SDL_Rect *>(&rect)) < 0) {
            THROW_ERROR("Error drawing rect");
        }
    }

    void Renderer::draw_rects(const std::vector<Rect> &rects) const {
        if (SDL_RenderDrawRects(m_renderer,
                                reinterpret_cast<const SDL_Rect*>(rects.data()),
                                static_cast<int>(rects.size())) < 0)
        {
            THROW_ERROR("Error drawing rects");
        }
    }

    void Renderer::fill_rect(const Rect &rect) const {
        if (SDL_RenderFillRect(m_renderer,
                               reinterpret_cast<const SDL_Rect*>(&rect)) < 0){
            THROW_ERROR("Error filling rect");
        }
    }

    void Renderer::fill_rects(const std::vector<Rect> &rects) const {
        if (SDL_RenderFillRects(m_renderer,
                                reinterpret_cast<const SDL_Rect*>(rects.data()),
                                static_cast<int>(rects.size())) < 0)
        {
            THROW_ERROR("Error filling rects");
        }
    }

} // namespace GameEngine