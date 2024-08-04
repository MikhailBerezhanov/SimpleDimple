
#pragma once

#include "TransformComponent.h"
#include "RendererComponent.h"
#include "TextureComponent.h"

namespace GameEngine {

/// These types correspond to components' priority
/// and the order in which they're processed
    enum class GameObjectComponentType : unsigned int {
        TRANSFORM,
        RENDERER,
        TEXTURE
    };

    template<typename T>
    struct ComponentTypes;

    template<>
    struct ComponentTypes<TransformComponent> {
        static constexpr GameObjectComponentType type = GameObjectComponentType::TRANSFORM;
        static constexpr const char* name = "Transform";
    };

    template<>
    struct ComponentTypes<RendererComponent> {
        static constexpr GameObjectComponentType type = GameObjectComponentType::RENDERER;
        static constexpr const char* name = "Renderer";
    };

    template<>
    struct ComponentTypes<TextureComponent> {
        static constexpr GameObjectComponentType type = GameObjectComponentType::TEXTURE;
        static constexpr const char* name = "Texture";
    };


} // GameEngine