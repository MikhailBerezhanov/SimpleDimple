#pragma once

#include "sdl_interface.h"
#include "sdl.h"

namespace GameEngine
{

    class Texture : public ITexture {
        // intermediate class to isolate SDL properties from direct access for Window
        class SDLHandle {
            SDL_Texture *m_texture;
            // mods
            SDL_Point m_center{};
            double m_angle{};
            SDL_RendererFlip m_flip{};
            // texture rect
            SDL_Rect m_rect{};
            explicit SDLHandle(SDL_Renderer *renderer, const std::string &image);
            explicit SDLHandle(SDL_Renderer *renderer, const Size2D &size);
            ~SDLHandle();
            friend class Texture;
        };

        SDLHandle m_sdl_handle;
        SDL_Texture *get_texture() const;
        const SDL_Rect *get_rect() const;
        const SDL_Point *get_center() const;
        double get_angle() const;
        SDL_RendererFlip get_flip() const;
        void reset_mods();
        Texture(SDL_Renderer *renderer, const std::string &image);
        Texture(SDL_Renderer *renderer, const Size2D &size);
        friend class Window;
    public:
        Texture(const Texture &) = delete;
        Texture& operator=(const Texture&) = delete;
        ~Texture();

        void SetColorMode(const RGBColor &rgb) const override;
        void SetAlphaMode(uint8_t alpha) const override;
        void SetPixelData(const std::vector<uint8_t> &pixel_data) const override;
        void SetPosition(const Pos2D &pos) override;
        Pos2D GetPosition() const override;
        Size2D GetSize() const override;
        Rect GetRect() const override;
        void Resize(const Size2D &size) override;
        void Upscale(uint8_t factor) override;
        void Downscale(uint8_t factor) override;
        void SetAngle(double angle, const Pos2D &center) override;
        void SetAngle(double angle) override;
        void FlipVertically() override;
        void FlipHorizontally() override;
    };

};
