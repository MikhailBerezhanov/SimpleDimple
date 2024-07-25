
#pragma once

#include "sdl.h"

namespace GameEngine {
    // proxy class protecting SDL pointer from unauthorized access
    class RenderContext {
    private:
        SDL_Renderer *m_renderer;
        explicit RenderContext(SDL_Renderer* rend) : m_renderer(rend){}
        friend class Window;
        friend class RendererComponent;
        friend class TextureComponent;
    protected:
        RenderContext() : m_renderer(nullptr) {} // for testing purposes
    public:
        ~RenderContext() = default;
    };
}
