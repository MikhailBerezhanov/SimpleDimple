//
// Created by andrey on 14.07.24.
//

#pragma once

#include "sdl.h"

namespace GameEngine {
    // proxy class protecting SDL pointer from unauthorized access
    class RenderContext {
    private:
        SDL_Renderer *renderer;
        explicit RenderContext(SDL_Renderer* rend) : renderer(rend){}
        ~RenderContext() = default;
        friend class Window;
        friend class RendererComponent;
        friend class TextureComponent;
    };
}
