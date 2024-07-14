//
// Created by andrey on 14.07.24.
//

#pragma once

#include <vector>
#include "sdl.h"

#include "IGameObjectComponent.h"
#include "RenderContext.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

namespace GameEngine {

    class RendererComponent : public IGameObjectComponent {
    private:
        class SDLHandle {
        private:
            SDL_Renderer *m_renderer = nullptr;
            explicit SDLHandle(SDL_Renderer *rend);
            ~SDLHandle() = default;
            friend class RendererComponent;
        };
        SDLHandle m_sdlHandle;
        const TransformComponent *m_transform;
        std::vector<TextureComponent *> m_textures{};
    public:
        RendererComponent(const RenderContext &context, const TransformComponent *transform);
        ~RendererComponent() = default;
        /// Renderer draw functions
        void SetDrawColor(const RGBColor &rgba);
        void DrawPoint(const Pos2D &point) const;
        void DrawPoints(const std::vector<Pos2D> &points) const;
        void DrawLine(const Pos2D &start, const Pos2D &end) const;
        // draw lines connecting a collection of points together
        void DrawLines(const std::vector<Pos2D> &points) const;
        void DrawRect(const Rect &rect) const;
        void DrawRects(const std::vector<Rect> &rects) const;
        void FillRect(const Rect &rect) const;
        void FillRects(const std::vector<Rect> &rects) const;
        RenderContext GetRenderContext() const;

        void OnUpdate() override;
    };

} // GameEngine