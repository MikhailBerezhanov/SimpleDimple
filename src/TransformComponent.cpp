//
// Created by andrey on 14.07.24.
//

#include "TransformComponent.h"

#define INVALID_POS INT32_MIN

namespace GameEngine {

    TransformComponent::TransformComponent()
        : m_sdlHandle()
        {}

    void TransformComponent::SetPosition(const Pos2D &pos) {
        m_sdlHandle.m_rect.x = pos.x;
        m_sdlHandle.m_rect.y = pos.y;
    }

    Pos2D TransformComponent::GetPosition() const {
        return {m_sdlHandle.m_rect.x, m_sdlHandle.m_rect.y};
    }

    Size2D TransformComponent::GetSize() const {
        return {static_cast<size_t>(m_sdlHandle.m_rect.w), static_cast<size_t>(m_sdlHandle.m_rect.h)};
    }

    Rect TransformComponent::GetRect() const {
        return {m_sdlHandle.m_rect.x,
                m_sdlHandle.m_rect.y,
                m_sdlHandle.m_rect.w,
                m_sdlHandle.m_rect.h};
    }

    void TransformComponent::Resize(const Size2D &size) {
        m_sdlHandle.m_rect.w = static_cast<int>(size.w);
        m_sdlHandle.m_rect.h = static_cast<int>(size.h);
    }

    void TransformComponent::SetAngle(double angle, const Pos2D &center) {
        m_sdlHandle.m_center.x = center.x;
        m_sdlHandle.m_center.y = center.y;
        m_sdlHandle.m_angle = angle;
    }

    void TransformComponent::SetAngle(double angle) {
        // invalidate center
        m_sdlHandle.m_center = {INVALID_POS, INVALID_POS};
        m_sdlHandle.m_angle = angle;
    }

    void TransformComponent::FlipVertically() {
        m_sdlHandle.m_flip = SDL_FLIP_VERTICAL;
    }

    void TransformComponent::FlipHorizontally() {
        m_sdlHandle.m_flip = SDL_FLIP_HORIZONTAL;
    }

    const SDL_Point *TransformComponent::get_center() const {
        if (m_sdlHandle.m_center.x != INVALID_POS && m_sdlHandle.m_center.y != INVALID_POS){
            return &m_sdlHandle.m_center;
        }else{
            return nullptr;
        }
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
        m_sdlHandle.m_center = {INVALID_POS, INVALID_POS};
        m_sdlHandle.m_angle = 0.0;
        m_sdlHandle.m_flip = SDL_FLIP_NONE;
    }

    void TransformComponent::OnUpdate() {

    }
} // GameEngine