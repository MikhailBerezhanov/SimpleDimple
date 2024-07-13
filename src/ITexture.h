
#pragma once

#include <vector>

#include "Types.h"

namespace GameEngine {
    // Interface, handles SDL Texture
    struct ITexture {
        virtual ~ITexture() = default;
        virtual void SetColorMode(const RGBColor &rgb) const = 0;
        virtual void SetAlphaMode(uint8_t alpha) const = 0;
        // todo: BlendMode, ScaleMode
        // todo: Lock, Unlock
        virtual void SetPixelData(const std::vector<uint8_t> &pixelData) const = 0;
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
