#pragma once

#include <string>
#include <tuple>
#include <memory>
#include <vector>

#include "sdl_classes.h"

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
        /// Size
        virtual void Resize(const Size2D &size) const = 0;
        /// Position
        virtual void SetPosition(const Pos2D &pos) const = 0;
        /// Window actions
        virtual void Show() const = 0;
        virtual void Hide() const = 0;
        virtual void Raise() const = 0;
        virtual void Maximize() const = 0;
        virtual void Minimize() const = 0;
        virtual void Restore() const = 0;
        /// Modifiers
        virtual IWindow & SetMinSize(const Size2D &size) = 0;
        virtual IWindow & SetMaxSize(const Size2D &size) = 0;
        virtual IWindow & SetBordered(bool bordered) = 0;
        virtual IWindow & SetResizable(bool resizable) = 0;
        virtual IWindow & SetAlwaysOnTop(bool on_top) = 0;
        /// Renderer functions
        virtual IWindow & SetDrawColor(const RGBColor &rgba) = 0;
        virtual void DrawPoint(const Pos2D &point) const = 0;
        virtual void DrawPoints(const std::vector<Pos2D> &points) const = 0;
        virtual void DrawLine(const Pos2D &start, const Pos2D &end) const = 0;
        // draw lines connecting a collection of points together
        virtual void DrawLines(const std::vector<Pos2D> &points) const = 0;
        virtual void DrawRect(const Rect &rect) const = 0;
        virtual void DrawRects(const std::vector<Rect> &rects) const = 0;
        virtual void FillRect(const Rect &rect) const = 0;
        virtual void FillRects(const std::vector<Rect> &rects) const = 0;
        virtual void Clear() const = 0; // clear screen
        virtual void Refresh() const = 0; // refresh textures (coordinates, flip, angle)
        virtual void Present() const = 0; // update changes made to screen
        /// Texture
        virtual size_t AppendTexture(const std::string &image) = 0; // returns texture_id
        virtual size_t AppendTexture(const Size2D &size) = 0; // returns texture_id
        virtual void RemoveTexture(size_t texture_id) = 0;
        virtual ITexture & GetTexture(size_t texture_id) const = 0;
        virtual void SetTextureActive(size_t texture_id, bool active) = 0;
    };

    // Interface, handles SDL Texture
    class ITexture {
    public:
        virtual ~ITexture() = default;
        virtual void SetColorMode(const RGBColor &rgb) const = 0;
        virtual void SetAlphaMode(uint8_t alpha) const = 0;
        // todo: BlendMode, ScaleMode
        // todo: Lock, Unlock
        virtual void SetPixelData(const std::vector<uint8_t> &pixel_data) const = 0;
        virtual void SetPosition(const Pos2D &pos) = 0;
        // todo: SetRect?
        virtual Pos2D GetPosition() const = 0;
        virtual Size2D GetSize() const = 0;
        virtual Rect GetRect() const = 0;
        virtual void Resize(const Size2D &size) = 0;
        virtual void Upscale(uint8_t factor) = 0;
        virtual void Downscale(uint8_t factor) = 0;
        virtual void SetAngle(double angle, const Pos2D &center) = 0;
        virtual void SetAngle(double angle) = 0;
        virtual void FlipVertically() = 0;
        virtual void FlipHorizontally() = 0;
    };
}