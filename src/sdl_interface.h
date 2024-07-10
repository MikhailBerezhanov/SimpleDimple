#pragma once

#include <string>
#include <tuple>
#include <memory>
#include <list>

#include "sdl_classes.h"

namespace GameEngine 
{
    // Forward declarations
    class IWindow;
    class IRenderer;
    class ITexture;
    class ISurface;

    // Interface, handles SDL Window
    class IWindow {
    public:
        virtual ~IWindow() = default;
        /// Title
        virtual void set_title(const std::string &title) const = 0;
        virtual std::string get_title() const = 0;
        /// Size
        virtual void set_size(const Size2D &size) const = 0;
        virtual Size2D get_size() const = 0;
        virtual Size2D get_size_in_pixels() const = 0;
        virtual void set_minimum_size(const Size2D &size) const = 0;
        virtual Size2D get_minimum_size() const = 0;
        virtual void set_maximum_size(const Size2D &size) const = 0;
        virtual Size2D get_maximum_size() const = 0;
        /// Position
        virtual void set_position(const Pos2D &pos) const = 0;
        virtual Pos2D get_position() const = 0;
        /// Display index
        virtual int get_display_index() const = 0;
        /// Flags
        virtual uint32_t get_flags() const = 0;
        /// Modifiers
        virtual void set_bordered(bool bordered) const = 0;
        virtual void set_resizable(bool resizable) const = 0;
        virtual void set_always_on_top(bool on_top) const = 0;
        virtual void show() const = 0;
        virtual void hide() const = 0;
        virtual void raise() const = 0;
        virtual void maximize() const = 0;
        virtual void minimize() const = 0;
        virtual void restore() const = 0;
        /// Nested objects
        /// Renderer
        virtual std::shared_ptr<IRenderer> create_renderer(int index = -1, uint32_t flags = 0) = 0;
    };

    // Interface, handles SDL Renderer
    class IRenderer {
    public:
        virtual ~IRenderer() = default;
        /// Size
        virtual Size2D get_output_size() const = 0;
        /// Modifiers
        // set color used to draw and clear.
        virtual void set_draw_color(const RGBColor &rgba) const = 0;
        virtual RGBColor get_draw_color() const = 0;
        // clears current rendering target with the color set by set_draw_color
        virtual void clear() const = 0;
        // works like 'flush' - makes changes appear on the screen
        virtual void present() const = 0;
        // from_rect - specify a part of the texture to render
        // to_rect - where on the rendering target it should be rendered
        virtual void copy(std::shared_ptr<ITexture> &texture, const Rect *from_rect, const Rect *to_rect) const = 0;
        /// Draw
        virtual void draw_point(const Pos2D &point) const = 0;
        virtual void draw_points(const Pos2D *points) const = 0;
        virtual void draw_line(const Pos2D &start, const Pos2D &end) const = 0;
        virtual void draw_lines(const Pos2D *lines) const = 0; // todo: figure out the logic in SDL
        virtual void draw_rect(const Rect &rect) const = 0;
        virtual void draw_rects(const Rect *rects) const = 0;
        virtual void fill_rect(const Rect &rect) const = 0;
        virtual void fill_rects(const Rect *rects) const = 0;
        /// Nested objects
        /// Texture
        virtual std::shared_ptr<ITexture> create_texture(std::shared_ptr<ISurface> &surface) = 0;
        virtual std::shared_ptr<ITexture> create_texture(uint32_t format, int access, const Size2D &size) = 0;
    };

    // Interface, handles SDL Texture
    class ITexture {
    public:
        virtual ~ITexture() = default;
        virtual Size2D get_size() const = 0;
        virtual RGBColor get_color_mode() const = 0;
        virtual void set_color_mode(const RGBColor &rgb) const = 0;
    };

    // Interface, handles SDL Surface
    class ISurface {
    public:
        virtual ~ISurface() = default;
        virtual void lock() const = 0;
        virtual void unlock() const = 0;
    };
}