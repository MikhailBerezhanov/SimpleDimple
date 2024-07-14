#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "IWindow.h"
#include "TextureComponent.h"
#include "RenderContext.h"
#include "sdl.h"


namespace GameEngine 
{
    // Implementation Window
    class Window : public IWindow {
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
//        std::unordered_map<size_t, std::unique_ptr<Texture>> m_textures; // all textures
//        std::unordered_set<Texture*> m_active_textures; // textures to update
        size_t m_num_textures = 0;
        // Private methods
        Size2D get_size_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const;
        Pos2D get_pos_generic(void (*sdl_func)(SDL_Window *, int *, int *)) const;
        void set_size_generic(void (*sdl_func)(SDL_Window *, int, int), const Size2D &size) const;
        void set_pos_generic(void (*sdl_func)(SDL_Window *, int, int), const Pos2D &pos) const;
    public:
        Window(const std::string &title, const Size2D &size, const Pos2D &pos);
        Window(const std::string &title, const Size2D &size, bool centered = true);
        ~Window();

        // Size
        void Resize(const Size2D &size) const override;
        // Position
        void SetPosition(const Pos2D &pos) const override;
        // Window actions
        void Show() const override;
        void Hide() const override;
        void Raise() const override; //Raise a window above other windows and set the input focus
        void Maximize() const override;
        void Minimize() const override;
        void Restore() const override; //Restore the size and position of a minimized or maximized window
        // Modifiers
        IWindow & SetMinSize(const Size2D &size) override;
        IWindow & SetMaxSize(const Size2D &size) override;
        IWindow & SetBordered(bool bordered) override;
        IWindow & SetResizable(bool resizable) override;
        IWindow & SetAlwaysOnTop(bool on_top) override;
        void Clear() const override;
        void Refresh() const override;
        void Present() const override;
        RenderContext GetRenderContext() const;
//        TextureId AppendTexture(const std::string &image) override;
//        TextureId AppendTexture(const Size2D &size) override;
//        void RemoveTexture(TextureId texture_id) override;
//        ITexture &GetTexture(TextureId texture_id) const override;
//        void SetTextureActive(TextureId texture_id, bool active) override;
    };
};