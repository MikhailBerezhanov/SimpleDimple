
#pragma once

#include "TransformComponent.h"
#include "RendererComponent.h"
#include "TextureComponent.h"

namespace GameEngine {

/// These types correspond to components' priority
/// and the order in which they're processed
#define _GAME_OBJECT_COMPONENT_TYPES_ \
    GOC_X(TRANSFORM) \
    GOC_X(RENDERER) \
    GOC_X(TEXTURE)

#define GOC_X(name) name,
    enum class GameObjectComponentType : unsigned int {
        _GAME_OBJECT_COMPONENT_TYPES_
    };
#undef GOC_X
#define GOC_X(name) #name,
    constexpr const char* GameObjectComponentNames[] = {
            _GAME_OBJECT_COMPONENT_TYPES_
    };
#undef GOC_X
#undef _GAME_OBJECT_COMPONENT_TYPES_

    /// Map components' types

    template<typename T>
    struct ComponentTypes;

    template<>
    struct ComponentTypes<TransformComponent> {
        static constexpr GameObjectComponentType type = GameObjectComponentType::TRANSFORM;
    };

    template<>
    struct ComponentTypes<RendererComponent> {
        static constexpr GameObjectComponentType type = GameObjectComponentType::RENDERER;
    };

    template<>
    struct ComponentTypes<TextureComponent> {
        static constexpr GameObjectComponentType type = GameObjectComponentType::TEXTURE;
    };


} // GameEngine