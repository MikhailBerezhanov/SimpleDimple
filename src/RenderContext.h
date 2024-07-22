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
        friend class Window;
        friend class RendererComponent;
        friend class TextureComponent;
    protected:
        RenderContext() : renderer(nullptr) {} // for testing purposes
    public:
        ~RenderContext() = default;
    };
}
