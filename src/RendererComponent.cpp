//
// Created by andrey on 14.07.24.
//

#include <stdexcept>

#include "RendererComponent.h"

#define EXPECT(condition, message) do { \
    if (!(condition)) { \
        throw std::runtime_error(std::string(__func__) \
        + " " + message \
        + ": " + SDL_GetError()); \
    }} while(0)

namespace GameEngine {

    RendererComponent::SDLHandle::SDLHandle(SDL_Renderer *rend)
        : m_renderer(rend)
        {}

    RendererComponent::RendererComponent(const RenderContext &context, const TransformComponent *transform)
        : m_sdlHandle(context.renderer),
        m_transform(transform)
        {}

    void RendererComponent::SetDrawColor(const RGBColor &rgba) {
        EXPECT(SDL_SetRenderDrawColor(m_sdlHandle.m_renderer, rgba.r, rgba.g, rgba.b, rgba.a) == 0,
               "Error setting renderer color");
    }

    void RendererComponent::DrawPoint(const Pos2D &point) const {
        EXPECT(SDL_RenderDrawPoint(m_sdlHandle.m_renderer, point.x, point.y) == 0,
               "Error drawing point");
    }

    void RendererComponent::DrawPoints(const std::vector<Pos2D> &points) const {
        EXPECT(SDL_RenderDrawPoints(m_sdlHandle.m_renderer,
                                    reinterpret_cast<const SDL_Point*>(points.data()),
                                    static_cast<int>(points.size())) == 0,
               "Error drawing points");
    }

    void RendererComponent::DrawLine(const Pos2D &start, const Pos2D &end) const {
        EXPECT(SDL_RenderDrawLine(m_sdlHandle.m_renderer, start.x, start.y, end.x, end.y) == 0,
               "Error drawing line");
    }

    void RendererComponent::DrawLines(const std::vector<Pos2D> &points) const {
        EXPECT(SDL_RenderDrawLines(m_sdlHandle.m_renderer,
                                   reinterpret_cast<const SDL_Point*>(points.data()),
                                   static_cast<int>(points.size())) == 0,
               "Error drawing lines");
    }

    void RendererComponent::DrawRect(const Rect &rect) const {
        EXPECT(SDL_RenderDrawRect(m_sdlHandle.m_renderer,
                                  reinterpret_cast<const SDL_Rect *>(&rect)) == 0,
               "Error drawing rect");
    }

    void RendererComponent::DrawRects(const std::vector<Rect> &rects) const {
        EXPECT(SDL_RenderDrawRects(m_sdlHandle.m_renderer,
                                   reinterpret_cast<const SDL_Rect*>(rects.data()),
                                   static_cast<int>(rects.size())) == 0,
               "Error drawing rects");
    }

    void RendererComponent::FillRect(const Rect &rect) const {
        EXPECT(SDL_RenderFillRect(m_sdlHandle.m_renderer,
                                  reinterpret_cast<const SDL_Rect*>(&rect)) == 0,
               "Error filling rect");
    }

    void RendererComponent::FillRects(const std::vector<Rect> &rects) const {
        EXPECT(SDL_RenderFillRects(m_sdlHandle.m_renderer,
                                   reinterpret_cast<const SDL_Rect*>(rects.data()),
                                   static_cast<int>(rects.size())) == 0,
               "Error filling rects");
    }

    RenderContext RendererComponent::GetRenderContext() const {
        return RenderContext(m_sdlHandle.m_renderer);
    }

    void RendererComponent::OnUpdate() {

    }

} // GameEngine