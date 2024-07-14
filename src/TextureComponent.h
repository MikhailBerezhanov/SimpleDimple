#pragma once

#include <string>
#include <vector>

#include "IGameObjectComponent.h"
#include "RenderContext.h"

namespace GameEngine
{

    class TextureComponent : public IGameObjectComponent {
    private:
        // intermediate class to isolate SDL properties from Window's direct access
        class SDLHandle {
            SDL_Texture *m_texture = nullptr;
            explicit SDLHandle(SDL_Renderer *renderer, const std::string &image);
            explicit SDLHandle(SDL_Renderer *renderer, const Size2D &size);
            ~SDLHandle();
            friend class TextureComponent;
        };

        SDLHandle m_sdlHandle;
        TextureComponent(const RenderContext &render_context, const std::string &image);
        TextureComponent(const RenderContext &render_context, const Size2D &size);
        friend class RendererComponent;
    public:
        TextureComponent(const TextureComponent &) = delete;
        TextureComponent& operator=(const TextureComponent&) = delete;
        ~TextureComponent();

        Size2D GetOrigSize() const;
        void SetColorMode(const RGBColor &rgb) const;
        void SetAlphaMode(uint8_t alpha) const;
        void SetPixelData(const std::vector<uint8_t> &pixelData) const;
    };

};
