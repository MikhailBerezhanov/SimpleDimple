#pragma once

#include <cstdint>
#include <cstddef>

namespace GameEngine {

/// These types correspond to components' priority
/// and the order in which they're processed
/// GOC - GameObjectComponent
#define _GOC_TYPES_ \
    GOC_X(TRANSFORM) \
    GOC_X(RENDERER) \
    GOC_X(TEXTURE)

#define GOC_X(name) name,
    enum class GameObjectComponentType : unsigned int {
        _GOC_TYPES_
    };
#undef GOC_X
#define GOC_X(name) #name,
    constexpr const char* GameObjectComponentNames[] = {
            _GOC_TYPES_
    };
#undef GOC_X
#undef _GOC_TYPES_

    // Copy of SDL_Rect
    struct Rect {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };

    // 000000 - black, ffffff - white
    struct RGBColor {
        uint8_t r = 255;
        uint8_t g = 255;
        uint8_t b = 255;
        uint8_t a = 255; //alpha: 255 - opaque, 0 - transparent
    };

    struct Pos2D {
        int x = 0;
        int y = 0;
    };

    struct Size2D {
        int w = 0;
        int h = 0;
    };

    using GameObjectId = size_t;

    class None{};

}

