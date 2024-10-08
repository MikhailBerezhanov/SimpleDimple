#include <stdexcept>

#include "ErrorHandling.h"
#include "Window.h"

#define EXPECT_SDL(condition, message) \
    EXPECT_MSG(condition, message << ": " << SDL_GetError())

namespace GameEngine 
{
    Window::Window(const std::string &title, const Size2D &size, const Pos2D &pos)
        : m_window(
            SDL_CreateWindow(
                title.c_str(), // title
                pos.x, // x pos
                pos.y, // y pos
                size.w, // width
                size.h, // height
                0 // window flags
            )
        )
    {
        EXPECT_SDL(m_window, "Unable to create window " + title);
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
        return {w, h};
    }

    Pos2D Window::get_pos_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const {
        int x, y;
        sdl_func(m_window, &x, &y);
        return {x, y};
    }

    void Window::set_size_generic(void (*sdl_func)(SDL_Window *, int, int), const Size2D &size) const {
        sdl_func(m_window, size.w, size.h);
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

    void Window::SetMinSize(const Size2D &size) {
        set_size_generic(SDL_SetWindowMinimumSize, size);
    }

    void Window::SetMaxSize(const Size2D &size) {
        set_size_generic(SDL_SetWindowMaximumSize, size);
    }

    void Window::SetBordered(bool bordered) {
        SDL_SetWindowBordered(m_window, bordered ? SDL_TRUE : SDL_FALSE);
    }

    void Window::SetResizable(bool resizable) {
        SDL_SetWindowResizable(m_window, resizable ? SDL_TRUE : SDL_FALSE);
    }

    void Window::SetAlwaysOnTop(bool on_top) {
        SDL_SetWindowAlwaysOnTop(m_window, on_top ? SDL_TRUE : SDL_FALSE);
    }

    void Window::Clear() const {
        EXPECT_SDL(SDL_RenderClear(m_renderer) == 0, "Unable to clear window");
    }

    void Window::Update() const {
        for (const auto o : m_activeObjects) {
            o->Update();
        }
    }

    void Window::Present() const {
        SDL_RenderPresent(m_renderer);
    }

    RenderContext Window::GetRenderContext() const {
        return RenderContext(m_renderer);
    }

    GameObjectId Window::AppendObject(const std::shared_ptr<IGameObject>& obj) {
        return AppendObject(obj, false);
    }

    GameObjectId Window::AppendObject(const std::shared_ptr<IGameObject>& obj, bool active) {
        auto it = m_gameObjects.emplace(m_objectsNum, obj);
        if (it.second) {
            auto obj_ptr = it.first->second.get();
            obj_ptr->Awake(); // 'initialize' object

            if (active) {
                if (m_activeObjects.insert(obj_ptr).second) {
                    obj_ptr->OnEnable(); // enable object
                }
            }
            return m_objectsNum++;
        }
        return it.first->first;
    }

    void Window::RemoveObject(GameObjectId id) {
        const auto &obj = m_gameObjects.find(id);
        if (obj != m_gameObjects.end()) {
           m_activeObjects.erase(obj->second.get());
           m_gameObjects.erase(obj);
           --m_objectsNum;
        }
    }

    IGameObject *Window::GetObject(GameObjectId id) const {
        return m_gameObjects.at(id).get();
    }

    void Window::SetObjectActive(GameObjectId id, bool active) {
        auto it = m_gameObjects.find(id);
        EXPECT_SDL(it != m_gameObjects.end(), "Game Object " + std::to_string(id) + " not found");
        if (active) {
            if (m_activeObjects.insert(it->second.get()).second) {
                it->second->OnEnable(); // enable object
            }
        }else {
            if (m_activeObjects.erase(it->second.get())) {
                it->second->OnDisable(); // disable object
            }
        }
    }

} // namespace GameEngine