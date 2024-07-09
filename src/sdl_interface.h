#pragma once

#include <string>
#include <tuple>
#include <memory>

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
        // Title
        virtual void set_title(const std::string &title) const = 0;
        virtual std::string get_title() const = 0;
        // Size
        virtual void set_size(size_t w, size_t h) const = 0;
        virtual std::tuple<size_t, size_t> get_size() const = 0;
        virtual std::tuple<size_t, size_t> get_size_in_pixels() const = 0;
        virtual void set_minimum_size(size_t w, size_t h) const = 0;
        virtual std::tuple<size_t, size_t> get_minimum_size() const = 0;
        virtual void set_maximum_size(size_t w, size_t h) const = 0;
        virtual std::tuple<size_t, size_t> get_maximum_size() const = 0;
        // Position
        virtual void set_position(int x, int y) const = 0;
        virtual std::tuple<int, int> get_position() const = 0;
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
        virtual IWindow & add_renderer(int index, uint32_t flags) = 0;
        virtual IWindow & remove_renderer() = 0;
        virtual const IRenderer *get_renderer() const = 0;
    };

    // Interface, handles SDL Renderer
    class IRenderer {
    public:
        ~IRenderer() = default;
        // Size
        virtual std::tuple<size_t, size_t> get_output_size() const = 0;
    };

    // Interface, handles SDL Texture
    class ITexture {
    public:
        ~ITexture() = default;
    };

    // Interface, handles SDL Surface
    class ISurface {
    public:
        ~ISurface() = default;
    };
};