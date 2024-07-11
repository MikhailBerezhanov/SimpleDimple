#pragma once

#include <unordered_map>
#include <unordered_set>

#include "sdl.h"
#include "sdl_interface.h"
#include "sdl_texture.h"

namespace GameEngine 
{
    // Implementation Window
    class Window : public IWindow {
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        std::unordered_map<size_t, std::unique_ptr<Texture>> m_textures; // all textures
        std::unordered_set<Texture*> m_active_textures; // textures to update
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
        // Renderer functions
        IWindow &SetDrawColor(const RGBColor &rgba) override;
        void DrawPoint(const Pos2D &point) const override;
        void DrawPoints(const std::vector<Pos2D> &points) const override;
        void DrawLine(const Pos2D &start, const Pos2D &end) const override;
        void DrawLines(const std::vector<Pos2D> &points) const override;
        void DrawRect(const Rect &rect) const override;
        void DrawRects(const std::vector<Rect> &rects) const override;
        void FillRect(const Rect &rect) const override;
        void FillRects(const std::vector<Rect> &rects) const override;
        void Clear() const override;
        void Refresh() const override;
        void Present() const override;
        size_t AppendTexture(const std::string &image) override;
        size_t AppendTexture(const Size2D &size) override;

        void RemoveTexture(size_t texture_id) override;

        ITexture &GetTexture(size_t texture_id) const override;

        void SetTextureActive(size_t texture_id, bool active) override;
    };
};