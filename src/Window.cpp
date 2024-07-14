#include <stdexcept>

#include "Window.h"

#define EXPECT(condition, message) do { \
    if (!(condition)) { \
        throw std::runtime_error(std::string(__func__) \
        + " " + message \
        + ": " + SDL_GetError()); \
    }} while(0)

namespace GameEngine 
{
    Window::Window(const std::string &title, const Size2D &size, const Pos2D &pos)
        : m_window(
            SDL_CreateWindow(
                title.c_str(), // title
                pos.x, // x pos
                pos.y, // y pos
                static_cast<int>(size.w), // width
                static_cast<int>(size.h), // height
                0 // window flags
            )
        )
    {
        EXPECT(m_window, "Unable to create window " + title);
        m_renderer = SDL_CreateRenderer(m_window, -1, 0);
        if (! m_renderer){
            //free window
            SDL_DestroyWindow(m_window);
            throw std::runtime_error("Unable to create renderer for " + title + ": " + SDL_GetError());
        }
    }

    Window::Window(const std::string &title, const Size2D &size, bool centered)
        : Window(
            title,
            size,
            Pos2D{
                    static_cast<int>(centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED),
                    static_cast<int>(centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED)
            })
    {}

    Window::~Window() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    Size2D Window::get_size_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const {
        int w, h;
        sdl_func(m_window, &w, &h);
        return Size2D{static_cast<size_t>(w), static_cast<size_t>(h)};
    }

    Pos2D Window::get_pos_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const {
        Pos2D pos{};
        sdl_func(m_window, &pos.x, &pos.y);
        return pos;
    }

    void Window::set_size_generic(void (*sdl_func)(SDL_Window *, int, int), const Size2D &size) const {
        sdl_func(m_window, static_cast<int>(size.w), static_cast<int>(size.h));
    }

    void Window::set_pos_generic(void (*sdl_func)(SDL_Window *, int, int), const Pos2D &pos) const {
        sdl_func(m_window, pos.x, pos.y);
    }

    void Window::Resize(const Size2D &size) const {
        set_size_generic(SDL_SetWindowSize, size);
    }

    void Window::SetPosition(const Pos2D &pos) const {
        set_pos_generic(SDL_SetWindowPosition, pos);
    }

    void Window::Show() const {
        SDL_ShowWindow(m_window);
    }

    void Window::Hide() const {
        SDL_HideWindow(m_window);
    }

    void Window::Raise() const {
        SDL_RaiseWindow(m_window);
    }

    void Window::Maximize() const {
        SDL_MaximizeWindow(m_window);
    }

    void Window::Minimize() const {
        SDL_MinimizeWindow(m_window);
    }

    void Window::Restore() const {
        SDL_RestoreWindow(m_window);
    }

    IWindow & Window::SetMinSize(const Size2D &size) {
        set_size_generic(SDL_SetWindowMinimumSize, size);
        return *this;
    }

    IWindow & Window::SetMaxSize(const Size2D &size) {
        set_size_generic(SDL_SetWindowMaximumSize, size);
        return *this;
    }

    IWindow & Window::SetBordered(bool bordered) {
        SDL_SetWindowBordered(m_window, bordered ? SDL_TRUE : SDL_FALSE);
        return *this;
    }

    IWindow & Window::SetResizable(bool resizable) {
        SDL_SetWindowResizable(m_window, resizable ? SDL_TRUE : SDL_FALSE);
        return *this;
    }

    IWindow & Window::SetAlwaysOnTop(bool on_top) {
        SDL_SetWindowAlwaysOnTop(m_window, on_top ? SDL_TRUE : SDL_FALSE);
        return *this;
    }

    void Window::Clear() const {
        EXPECT(SDL_RenderClear(m_renderer) == 0, "Unable to clear window");
    }

    void Window::Refresh() const {
        // call SDL_RenderCopy() only for active textures
//        for (auto texture : m_active_textures) {
//
//            if (texture->get_angle() != 0.0 || texture->get_flip() != SDL_FLIP_NONE) {
//                // special treatment for flip and rotation
//                EXPECT(SDL_RenderCopyEx(m_renderer, // sdl renderer
//                                        texture->get_texture(), // sdl texture
//                                        nullptr, // apply to whole texture
//                                        texture->get_rect(), // texture destination
//                                        texture->get_angle(), // rotation angle
//                                        texture->get_center(), // rotation center (if null, rotate around dstrect.w / 2, dstrect.h / 2)
//                                        texture->get_flip() // flip action
//                ) == 0, "Unable to render-copy texture");
//            }
//            else {
//                // general treatment
//                EXPECT(SDL_RenderCopy(m_renderer, // sdl renderer
//                                      texture->get_texture(), // sdl texture
//                                      nullptr, // apply to whole texture
//                                      texture->get_rect() // texture destination
//                ) == 0,
//                       "Unable to render-copy texture");
//            }
//        }
    }

    void Window::Present() const {
        SDL_RenderPresent(m_renderer);
    }

    RenderContext Window::GetRenderContext() const {
        return RenderContext(m_renderer);
    }

//    TextureId Window::AppendTexture(const std::string &image) {
//        auto tex = new Texture(m_renderer, image);
//        m_textures.emplace(m_num_textures, std::unique_ptr<Texture>(tex));
//        return m_num_textures++;
//    }
//
//    TextureId Window::AppendTexture(const Size2D &size) {
//        auto tex = std::unique_ptr<Texture>(new Texture(m_renderer, size));
//        m_textures.emplace(m_num_textures, std::move(tex));
//        return m_num_textures++;
//    }
//
//    void Window::RemoveTexture(TextureId texture_id) {
//        auto tex = m_textures.find(texture_id);
//        if (tex != m_textures.end()) {
//            m_active_textures.erase(tex->second.get());
//            m_textures.erase(tex);
//            --m_num_textures;
//        }
//    }
//
//    ITexture &Window::GetTexture(TextureId texture_id) const {
//        return *m_textures.at(texture_id);
//    }
//
//    void Window::SetTextureActive(TextureId texture_id, bool active) {
//        auto it = m_textures.find(texture_id);
//        EXPECT(it != m_textures.end(), "Texture " + std::to_string(texture_id) + " not found");
//        if (active) {
//            m_active_textures.insert(it->second.get());
//        }else {
//            m_active_textures.erase(it->second.get());
//        }
//    }


} // namespace GameEngine