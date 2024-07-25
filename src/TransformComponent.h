
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
            void reset_center();
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
        TransformComponent(const TransformComponent &) = delete;
        TransformComponent &operator=(const TransformComponent &) = delete;
        TransformComponent(TransformComponent &&) = delete;
        TransformComponent &operator=(TransformComponent &&) = delete;
        ~TransformComponent() = default;
        //todo: Clone()
        void SetPosition(const Pos2D &pos);
        void Move(const Pos2D &pos); // similar to SetPosition() but with relative coordinates
        Pos2D GetPosition() const;
        Size2D GetSize() const;
        Rect GetRect() const;
        void Resize(const Size2D &size);
        void Downscale(unsigned int factor);
        void Upscale(unsigned int factor);
        void SetCenter(const Pos2D &center);
        void SetAngle(double angle); // set angle (absolute)
        void Rotate(double angle); // similar to SetAngle() but angle is relative to the current one
        double GetAngle() const;
        Pos2D GetCenter() const;
        void FlipVertically();
        void FlipHorizontally();

        void OnUpdate() override;

    };

} // GameEngine