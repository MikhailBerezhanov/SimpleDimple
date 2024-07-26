
#include "TransformComponent.h"
#include "ErrorHandling.h"

namespace GameEngine {

    TransformComponent::SDLHandle::SDLHandle(const Size2D &size) {
        m_rect.w = size.w;
        m_rect.h = size.h;
    }

    void TransformComponent::SDLHandle::reset_center() {
        m_center = {
                m_rect.w / 2,
                m_rect.h / 2
        };
    }

    TransformComponent::TransformComponent(const Size2D &size)
        : m_sdlHandle(size)
        {
            reset();
        }

    void TransformComponent::SetPosition(const Pos2D &pos) {
        m_sdlHandle.m_rect.x = pos.x;
        m_sdlHandle.m_rect.y = pos.y;
    }

    void TransformComponent::Move(const Pos2D &pos) {
        m_sdlHandle.m_rect.x += pos.x;
        m_sdlHandle.m_rect.y += pos.y;
    }

    Pos2D TransformComponent::GetPosition() const {
        return {m_sdlHandle.m_rect.x, m_sdlHandle.m_rect.y};
    }

    Size2D TransformComponent::GetSize() const {
        return {m_sdlHandle.m_rect.w, m_sdlHandle.m_rect.h};
    }

    Rect TransformComponent::GetRect() const {
        return {m_sdlHandle.m_rect.x,
                m_sdlHandle.m_rect.y,
                m_sdlHandle.m_rect.w,
                m_sdlHandle.m_rect.h};
    }

    void TransformComponent::Resize(const Size2D &size) {
        EXPECT_MSG(size.w >= 0 && size.h >= 0,
                   "Resize impossible: invalid dimensions");
        m_sdlHandle.m_rect.w = size.w;
        m_sdlHandle.m_rect.h = size.h;
        m_sdlHandle.reset_center();
    }

    void TransformComponent::Downscale(int factor) {
        EXPECT_MSG(factor > 0, "Downscale impossible: invalid factor");
        m_sdlHandle.m_rect.w /= factor;
        m_sdlHandle.m_rect.h /= factor;
        m_sdlHandle.reset_center();
    }

    void TransformComponent::Upscale(int factor) {
        EXPECT_MSG(factor >= 0, "Upscale impossible: invalid factor");
        m_sdlHandle.m_rect.w *= factor;
        m_sdlHandle.m_rect.h *= factor;
        m_sdlHandle.reset_center();
    }

    void TransformComponent::SetCenter(const Pos2D &center) {
        m_sdlHandle.m_center.x = center.x;
        m_sdlHandle.m_center.y = center.y;
    }

    void TransformComponent::SetAngle(double angle) {
        m_sdlHandle.m_angle = std::fmod(angle, 360.0);
    }

    void TransformComponent::Rotate(double angle) {
        m_sdlHandle.m_angle = std::fmod(m_sdlHandle.m_angle + angle, 360.0);
    }

    double TransformComponent::GetAngle() const {
        return m_sdlHandle.m_angle;
    }

    Pos2D TransformComponent::GetCenter() const {
        return {m_sdlHandle.m_center.x, m_sdlHandle.m_center.y};
    }

    void TransformComponent::FlipVertically() {
        m_sdlHandle.m_flip = static_cast<SDL_RendererFlip>(m_sdlHandle.m_flip ^ SDL_FLIP_VERTICAL);
    }

    void TransformComponent::FlipHorizontally() {
        m_sdlHandle.m_flip = static_cast<SDL_RendererFlip>(m_sdlHandle.m_flip ^ SDL_FLIP_HORIZONTAL);
    }

    bool TransformComponent::IsFlippedVertically() const {
        return m_sdlHandle.m_flip & SDL_FLIP_VERTICAL;
    }

    bool TransformComponent::IsFlippedHorizontally() const {
        return m_sdlHandle.m_flip & SDL_FLIP_HORIZONTAL;
    }

    const SDL_Point *TransformComponent::get_center() const {
        return &m_sdlHandle.m_center;
    }

    const SDL_Rect *TransformComponent::get_rect() const {
        return &m_sdlHandle.m_rect;
    }

    double TransformComponent::get_angle() const {
        return m_sdlHandle.m_angle;
    }

    SDL_RendererFlip TransformComponent::get_flip() const {
        return m_sdlHandle.m_flip;
    }

    void TransformComponent::reset() {
        m_sdlHandle.reset_center();
        m_sdlHandle.m_angle = 0.0;
        m_sdlHandle.m_flip = SDL_FLIP_NONE;
    }

    void TransformComponent::OnUpdate() {
        // do nothing
    }


} // GameEngine