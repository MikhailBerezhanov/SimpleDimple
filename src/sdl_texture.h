#ifndef SDL_TEXTURE_H
#define SDL_TEXTURE_H

#include <memory>

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine
{
    class Texture : public ITexture
    {
        SDL_Texture *m_texture;
    public:
        Texture(std::shared_ptr<IRenderer> renderer, 
            uint32_t /*SDL_PixelFormatEnum*/ format, 
            int access /*SDL_TextureAccess*/, 
            int /*pixels*/ width, 
            int /*pixels*/heigth
        );
        Texture(std::shared_ptr<IRenderer> renderer, std::shared_ptr<ISurface> surface);
        ~Texture();
    };
};

#endif