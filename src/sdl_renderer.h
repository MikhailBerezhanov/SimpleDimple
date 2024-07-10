#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine 
{
    class Renderer : public IRenderer {

        SDL_Renderer *m_renderer;
        Renderer(SDL_Window * window, int index, Uint32 flags);
        friend class Window;
    public:
        
        ~Renderer();

        const SDL_Renderer *get_raw() const;

        // Size
        Size2D get_output_size() const override;
        // Modifiers
        IRenderer & clear() override;
        IRenderer & present() override;
        IRenderer & copy(std::shared_ptr<ITexture> &texture, const Rect *source, const Rect *dest) override;
        // Nested objects
        // Texture
        std::shared_ptr<ITexture> create_texture(std::shared_ptr<ISurface> &surface) override;
        std::shared_ptr<ITexture> create_texture(uint32_t format, int access, const Size2D &size) override;
    };
};