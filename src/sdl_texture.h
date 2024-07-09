#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine
{
    class Texture : public ITexture
    {
        SDL_Texture *m_texture;
        Texture(SDL_Renderer *renderer, SDL_Surface *surface);
        Texture(SDL_Renderer *renderer, uint32_t format, int access, int width, int heigth);
        friend class Renderer;
    public:
        
        ~Texture();

        const SDL_Texture *get_raw() const;

        std::tuple<uint32_t, int, int, int> query() const override;
        std::tuple<uint8_t, uint8_t, uint8_t> get_color_mode() const override;

    };
};
