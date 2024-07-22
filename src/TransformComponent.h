//
// Created by andrey on 14.07.24.
//

#pragma once

#include "sdl.h"
#include "IGameObjectComponent.h"

namespace GameEngine {

    class TransformComponent : public IGameObjectComponent {
    private:
        class SDLHandle {
        private:
            SDL_Point m_center{};
            double m_angle = 0.0;
            SDL_RendererFlip m_flip{};
            SDL_Rect m_rect{};
            SDLHandle(const Size2D &size);
            ~SDLHandle() = default;
            friend class TransformComponent;
        };

        SDLHandle m_sdlHandle;

        const SDL_Point *get_center() const;
        const SDL_Rect *get_rect() const;
        double get_angle() const;
        SDL_RendererFlip get_flip() const;
        void reset();
        friend class RendererComponent;
        friend class GameObject;
    protected:
        explicit TransformComponent(const Size2D &size = {});
    public:

        ~TransformComponent() = default;

        void SetPosition(const Pos2D &pos);
        Pos2D GetPosition() const;
        Size2D GetSize() const;
        Rect GetRect() const;
        void Resize(const Size2D &size);
        void SetAngle(double angle, const Pos2D &center);
        void SetAngle(double angle);
        void FlipVertically();
        void FlipHorizontally();

        void OnUpdate() override;

    };

} // GameEngine