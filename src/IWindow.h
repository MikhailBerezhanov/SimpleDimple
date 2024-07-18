
#pragma once

#include <vector>

#include "Types.h"

namespace GameEngine {

    struct IWindow {
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

        virtual void Clear() const = 0; // clear screen
        virtual void Refresh() const = 0; // refresh textures (coordinates, flip, angle)
        virtual void Present() const = 0; // update changes made to screen
        /// GameObject
//        virtual TextureId AppendTexture(const std::string &image) = 0; // returns texture_id
//        virtual TextureId AppendTexture(const Size2D &size) = 0; // returns texture_id
//        virtual void RemoveTexture(TextureId texture_id) = 0;
//        virtual ITexture & GetTexture(TextureId texture_id) const = 0;
//        virtual void SetTextureActive(TextureId texture_id, bool active) = 0;
    };
}
