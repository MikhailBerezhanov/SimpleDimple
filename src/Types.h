#pragma once

#include <cstdint>
#include <cstddef>

namespace GameEngine {
    // Copy of SDL_Rect
    struct Rect {
        int x;
        int y;
        int w;
        int h;
    };

    // 000000 - black, ffffff - white
    struct RGBColor {
        uint8_t r = 255;
        uint8_t g = 255;
        uint8_t b = 255;
        uint8_t a = 255; //alpha: 255 - opaque, 0 - transparent
    };

    struct Pos2D {
        int x;
        int y;
    };

    struct Size2D {
        size_t w;
        size_t h;
    };
}

