#pragma once

#include <string>
#include <tuple>
#include <memory>
#include <list>

namespace GameEngine 
{
    // Forward declarations
    class IWindow;
    class IRenderer;
    class ITexture;
    class ISurface;

    // Copy of SDL_Rect
    struct Rect {
        int x, y;
        int w, h;
    };

    struct RGBColor {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    struct Pos2D {
        int x;
        int y;
    };

    struct Size2D {
        size_t w;
        size_t h;
    };

    // Interface, handles SDL Window
    class IWindow {
    public:
        virtual ~IWindow() = default;
        // Title
        virtual void set_title(const std::string &title) const = 0;
        virtual std::string get_title() const = 0;
        // Size
        virtual void set_size(const Size2D &size) const = 0;
        virtual Size2D get_size() const = 0;
        virtual Size2D get_size_in_pixels() const = 0;
        virtual void set_minimum_size(const Size2D &size) const = 0;
        virtual Size2D get_minimum_size() const = 0;
        virtual void set_maximum_size(const Size2D &size) const = 0;
        virtual Size2D get_maximum_size() const = 0;
        // Position
        virtual void set_position(const Pos2D &pos) const = 0;
        virtual Pos2D get_position() const = 0;
        // Display index
        virtual int get_display_index() const = 0;
        // Flags
        virtual uint32_t get_flags() const = 0;
        // Modifiers
        virtual IWindow & set_bordered(bool bordered) = 0;
        virtual IWindow & set_resizable(bool resizable) = 0;
        virtual IWindow & set_always_on_top(bool on_top) = 0;
        virtual IWindow & show() = 0;
        virtual IWindow & hide() = 0;
        virtual IWindow & raise() = 0;
        virtual IWindow & maximize() = 0;
        virtual IWindow & minimize() = 0;
        virtual IWindow & restore() = 0;
        // Nested objects
        // Renderer
        virtual std::shared_ptr<IRenderer> create_renderer(int index, uint32_t flags) = 0;
    };

    // Interface, handles SDL Renderer
    class IRenderer {
    public:
        virtual ~IRenderer() = default;
        // Size
        virtual Size2D get_output_size() const = 0;
        // Modifiers
        virtual IRenderer & clear() = 0;
        virtual IRenderer & present() = 0;
        virtual IRenderer & copy(std::shared_ptr<ITexture> &texture, const Rect *source, const Rect *dest) = 0;
        // Nested objects
        // Texture
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
        virtual ISurface & lock() = 0;
        virtual ISurface & unlock() = 0;
    };
};