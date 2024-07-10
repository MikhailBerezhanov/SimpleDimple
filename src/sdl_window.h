#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine 
{
    // Implementation Window
    class Window : public IWindow {

        SDL_Window *m_window;
        // Private methods
        Size2D get_size_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const;
        Pos2D get_pos_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const;
        void set_size_generic(void (*sdl_func)(SDL_Window *, int, int), const Size2D &size) const;
        void set_pos_generic(void (*sdl_func)(SDL_Window *, int, int), const Pos2D &pos) const;

        Window(const std::string &title, const Size2D &size, const Pos2D &pos, uint32_t flags = 0);
        Window(const std::string &title, const Size2D &size, bool centered = true, uint32_t flags = 0);
    public:
        static std::shared_ptr<IWindow> create(const std::string &title, const Size2D &size, const Pos2D &pos, uint32_t flags = 0);
        static std::shared_ptr<IWindow> create(const std::string &title, const Size2D &size, bool centered = true, uint32_t flags = 0);
        ~Window();
        // Get raw pointer
        const SDL_Window *get_raw() const;
        // Title
        void set_title(const std::string &title) const override;
        std::string get_title() const override;
        // Size
        void set_size(const Size2D &size) const override;
        Size2D get_size() const override;
        Size2D get_size_in_pixels() const override;
        void set_minimum_size(const Size2D &size) const override;
        Size2D get_minimum_size() const override;
        void set_maximum_size(const Size2D &size) const override;
        Size2D get_maximum_size() const override;
        // Position
        void set_position(const Pos2D &pos) const override;
        Pos2D get_position() const override;
        // Display index
        int get_display_index() const override;
        // Flags
        uint32_t get_flags() const override;
        // Modifiers
        IWindow & set_bordered(bool bordered) override;
        IWindow & set_resizable(bool resizable) override;
        IWindow & set_always_on_top(bool on_top) override;
        IWindow & show() override;
        IWindow & hide() override;
        IWindow & raise() override; //Raise a window above other windows and set the input focus
        IWindow & maximize() override;
        IWindow & minimize() override;
        IWindow & restore() override; //Restore the size and position of a minimized or maximized window
        // Nested objects
        // Renderer
        std::shared_ptr<IRenderer> create_renderer(int index = -1, uint32_t flags = 0) override;
    };
};