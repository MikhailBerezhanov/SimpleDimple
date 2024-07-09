#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine 
{
    // Implementation Window
    class Window : public IWindow {
        SDL_Window *m_window;
        IRenderer *m_renderer = nullptr;
        // Private methods
        std::tuple<size_t, size_t> get_size_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const;
        std::tuple<int, int> get_pos_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const;
    public:
        Window(const std::string &title, size_t width, size_t heigth, size_t pos_x, size_t pos_y, uint32_t flags /*SDL_WindowFlags*/ = 0);
        Window(const std::string &title, size_t width, size_t heigth, bool centered = true, uint32_t flags /*SDL_WindowFlags*/ = 0);
        ~Window();
        // Get raw pointer
        SDL_Window *get_raw() const;
        // Title
        void set_title(const std::string &title) const override;
        std::string get_title() const override;
        // Size
        void set_size(size_t w, size_t h) const override;
        std::tuple<size_t, size_t> get_size() const override;
        std::tuple<size_t, size_t> get_size_in_pixels() const override;
        void set_minimum_size(size_t w, size_t h) const override;
        std::tuple<size_t, size_t> get_minimum_size() const override;
        void set_maximum_size(size_t w, size_t h) const override;
        std::tuple<size_t, size_t> get_maximum_size() const override;
        // Position
        void set_position(int x, int y) const override;
        std::tuple<int, int> get_position() const override;
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
        IWindow & add_renderer(int index = -1, uint32_t flags = 0) override;
        IWindow & remove_renderer() override;
        const IRenderer *get_renderer() const override;
    };
};