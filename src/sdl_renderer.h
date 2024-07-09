#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine 
{
    class Renderer : public IRenderer
    {
        SDL_Renderer *m_renderer;
        std::list<ITexture *> m_textures;
        Renderer(SDL_Window * window, int index, Uint32 flags);
        friend class Window;
    public:
        
        ~Renderer();

        const SDL_Renderer *get_raw() const;

        // Size
        std::tuple<size_t, size_t> get_output_size() const override;
        // Nested objects
        // Texture
        IRenderer & add_texture(ISurface & surface) override;
        IRenderer & add_texture(uint32_t format, int access, int width, int heigth) override;
        const std::list<ITexture*> & get_textures() const override;
    };
};