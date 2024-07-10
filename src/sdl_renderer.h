#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine 
{
    class Renderer : public IRenderer {

        SDL_Renderer *m_renderer;
        Renderer(SDL_Window * window, int index, Uint32 flags);
        friend class Window;
    public:
        
        ~Renderer();

        /// Size
        Size2D get_output_size() const override;
        /// Modifiers
        void set_draw_color(const RGBColor &rgba) const override;
        RGBColor get_draw_color() const override;
        void clear() const override;
        void present() const override;
        void copy(std::shared_ptr<ITexture> &texture, const Rect *src_rect, const Rect *dest_rect) const override;
        // todo: copy with rotation, etc (SDL_RenderCopyEx)

        /// Draw
        void draw_point(const Pos2D &point) const override;
        void draw_points(const Pos2D *points) const override;
        void draw_line(const Pos2D &start, const Pos2D &end) const override;
        void draw_lines(const Pos2D *lines) const override;
        void draw_rect(const Rect &rect) const override;
        void draw_rects(const Rect *rects) const override;
        void fill_rect(const Rect &rect) const override;
        void fill_rects(const Rect *rects) const override;
        /// Nested objects
        /// Texture
        std::shared_ptr<ITexture> create_texture(std::shared_ptr<ISurface> &surface) override;
        std::shared_ptr<ITexture> create_texture(uint32_t format, int access, const Size2D &size) override;
    };
};