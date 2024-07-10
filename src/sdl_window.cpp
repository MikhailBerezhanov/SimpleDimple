#include <stdexcept>

#include "sdl_window.h"
#include "sdl_renderer.h"

namespace GameEngine 
{
    Window::Window(const std::string &title, const Size2D &size, const Pos2D &pos, uint32_t flags)
        : m_window(
            SDL_CreateWindow(
                title.c_str(), // title
                pos.x, // x pos
                pos.y, // y pos
                static_cast<int>(size.w), // width
                static_cast<int>(size.h), // height
                flags // window flags
            )
        )
    {
        if (! m_window) {
            throw std::runtime_error("Unable to create window " + title + ": " + std::string(SDL_GetError()));
        }
    }

    Window::Window(const std::string &title, const Size2D &size, bool centered, uint32_t flags)
        : Window(
            title,
            size,
            Pos2D{
                    static_cast<int>(centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED),
                    static_cast<int>(centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED)
            },
            flags)
    {}

    Window::~Window()
    {
        // destroy window
        SDL_DestroyWindow(m_window);
    }

    const SDL_Window *Window::get_raw() const
    {
        return m_window;
    }

    Size2D Window::get_size_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const
    {
        int w, h;
        sdl_func(m_window, &w, &h);
        return Size2D{static_cast<size_t>(w), static_cast<size_t>(h)};
    }

    Pos2D Window::get_pos_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const
    {
        int x, y;
        sdl_func(m_window, &x, &y);
        return Pos2D{x, y};
    }

    void Window::set_size_generic(void (*sdl_func)(SDL_Window *, int, int), const Size2D &size) const {
        sdl_func(m_window, static_cast<int>(size.w), static_cast<int>(size.h));
    }

    void Window::set_pos_generic(void (*sdl_func)(SDL_Window *, int, int), const Pos2D &pos) const {
        sdl_func(m_window, pos.x, pos.y);
    }

    void Window::set_title(const std::string &title) const
    {
        SDL_SetWindowTitle(m_window, title.c_str());
    }

    std::string Window::get_title() const
    {
        auto title = SDL_GetWindowTitle(m_window);
        return title ? title : "";
    }

    void Window::set_size(const Size2D &size) const
    {
        set_size_generic(SDL_SetWindowSize, size);
    }

    Size2D Window::get_size() const
    {
        return get_size_generic(SDL_GetWindowSize);
    }

    Size2D Window::get_size_in_pixels() const
    {
        return get_size_generic(SDL_GetWindowSizeInPixels);
    }

    void Window::set_minimum_size(const Size2D &size) const
    {
        set_size_generic(SDL_SetWindowMinimumSize, size);
    }
    
    Size2D Window::get_minimum_size() const
    {
        return get_size_generic(SDL_GetWindowMinimumSize);
    }

    void Window::set_maximum_size(const Size2D &size) const
    {
        set_size_generic(SDL_SetWindowMaximumSize, size);
    }

    Size2D Window::get_maximum_size() const
    {
        return get_size_generic(SDL_GetWindowMaximumSize);
    }

    void Window::set_position(const Pos2D &pos) const
    {
        set_pos_generic(SDL_SetWindowPosition, pos);
    }

    Pos2D Window::get_position() const
    {
        return get_pos_generic(SDL_GetWindowPosition);
    }
    
    int Window::get_display_index() const
    {
        return SDL_GetWindowDisplayIndex(m_window);
    }

    uint32_t Window::get_flags() const
    {
        return SDL_GetWindowFlags(m_window);
    }

    IWindow &Window::set_bordered(bool bordered)
    {
        SDL_SetWindowBordered(m_window, bordered ? SDL_TRUE : SDL_FALSE);
        return *this;
    }

    IWindow &Window::set_resizable(bool resizable)
    {
        SDL_SetWindowResizable(m_window, resizable ? SDL_TRUE : SDL_FALSE);
        return *this;
    }

    IWindow &Window::set_always_on_top(bool on_top)
    {
        SDL_SetWindowAlwaysOnTop(m_window, on_top ? SDL_TRUE : SDL_FALSE);
        return *this;
    }

    IWindow &Window::show()
    {
        SDL_ShowWindow(m_window);
        return *this;
    }

    IWindow &Window::hide()
    {
        SDL_HideWindow(m_window);
        return *this;
    }

    IWindow &Window::raise()
    {
        SDL_RaiseWindow(m_window);
        return *this;
    }

    IWindow &Window::maximize()
    {
        SDL_MaximizeWindow(m_window);
        return *this;
    }

    IWindow &Window::minimize()
    {
        SDL_MinimizeWindow(m_window);
        return *this;
    }

    IWindow &Window::restore()
    {
        SDL_RestoreWindow(m_window);
        return *this;
    }

    std::shared_ptr<IRenderer> Window::create_renderer(int index, uint32_t flags)
    {
        auto rend = new Renderer(m_window, index, flags);
        return std::shared_ptr<Renderer>(rend);
    }

}; // namespace GameEngine