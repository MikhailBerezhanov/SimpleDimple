
#pragma once

#include <vector>

#include "Types.h"
#include "IGameObject.h"

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
        virtual void SetMinSize(const Size2D &size) = 0;
        virtual void SetMaxSize(const Size2D &size) = 0;
        virtual void SetBordered(bool bordered) = 0;
        virtual void SetResizable(bool resizable) = 0;
        virtual void SetAlwaysOnTop(bool on_top) = 0;

        virtual void Clear() const = 0; // clear screen
        virtual void Update() const = 0; // refresh textures (coordinates, flip, angle)
        virtual void Present() const = 0; // update changes made to screen
        /// GameObject
        virtual GameObjectId AppendObject(std::unique_ptr<IGameObject> obj) = 0;
        virtual GameObjectId AppendObject(std::unique_ptr<IGameObject> obj, bool active) = 0;
        virtual void RemoveObject(GameObjectId id) = 0;
        virtual IGameObject *GetObject(GameObjectId id) const = 0;
        virtual void SetObjectActive(GameObjectId id, bool active) = 0;
    };
}
