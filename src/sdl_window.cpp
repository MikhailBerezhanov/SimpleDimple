#include <stdexcept>
#include "sdl_window.h"

namespace GameEngine 
{
    Window::Window(const std::string &title, size_t width, size_t heigth, size_t pos_x, size_t pos_y, uint32_t flags) 
        : m_window(
            SDL_CreateWindow(
                title.c_str(), // title
                pos_x, // x pos
                pos_y, // y pos
                width, // width
                heigth, // heigth
                flags // window flags
            )
        )
        {
            if (! m_window) {
                throw std::runtime_error("Unable to create window " + title + ": " + std::string(SDL_GetError()));
            }
        }
    Window::Window(const std::string &title, size_t width, size_t heigth, bool centered, uint32_t flags)
        : Window(
            title,
            width,
            heigth,
            centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED,
            centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED,
            flags
        )
        {}
    Window::~Window()
    {
        SDL_DestroyWindow(m_window);
    }
    std::tuple<size_t, size_t> Window::get_size_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const
    {
        int w, h;
        sdl_func(m_window, &w, &h);
        return std::make_tuple(static_cast<size_t>(w), static_cast<size_t>(h));
    }
    std::tuple<int, int> Window::get_pos_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const
    {
        int x, y;
        sdl_func(m_window, &x, &y);
        return std::make_tuple(x, y);
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
    void Window::set_size(size_t w, size_t h) const 
    {
        SDL_SetWindowSize(m_window, w, h);
    }
    std::tuple<size_t, size_t> Window::get_size() const
    {
        return get_size_generic(SDL_GetWindowSize);
    }
    std::tuple<size_t, size_t> Window::get_size_in_pixels() const
    {
        return get_size_generic(SDL_GetWindowSizeInPixels);
    }
    void Window::set_minimum_size(size_t w, size_t h) const 
    {
        SDL_SetWindowMinimumSize(m_window, w, h);
    }
    std::tuple<size_t, size_t> Window::get_minimum_size() const
    {
        return get_size_generic(SDL_GetWindowMinimumSize);
    }
    void Window::set_maximum_size(size_t w, size_t h) const 
    {
        SDL_SetWindowMaximumSize(m_window, w, h);
    }
    std::tuple<size_t, size_t> Window::get_maximum_size() const
    {
        return get_size_generic(SDL_GetWindowMaximumSize);
    }
    void Window::set_position(int x, int y) const 
    {
        SDL_SetWindowPosition(m_window, x, y);
    }
    std::tuple<int, int> Window::get_position() const
    {
        return get_pos_generic(SDL_GetWindowPosition);
    }
    int Window::get_display_index() const
    {
        SDL_GetWindowDisplayIndex(m_window);
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
}; // namespace SDL