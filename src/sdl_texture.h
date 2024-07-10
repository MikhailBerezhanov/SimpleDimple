#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine
{
    class Texture : public ITexture {

        SDL_Texture *m_texture;
        Texture(SDL_Renderer *renderer, SDL_Surface *surface);
        Texture(SDL_Renderer *renderer, uint32_t format, int access, const Size2D &size);
        friend class Renderer;
    public:
        
        ~Texture();

        Size2D get_size() const override;
        RGBColor get_color_mode() const override;
        void set_color_mode(const RGBColor &rgb) const override;
    };
};
