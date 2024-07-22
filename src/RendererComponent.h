//
// Created by andrey on 14.07.24.
//

#pragma once

#include <memory>
#include <vector>
#include <queue>
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
        class TextureHandle {
        private:
            TextureHandle() = default;
            ~TextureHandle() = default;
            std::queue<std::shared_ptr<TextureComponent>> m_textures_q{};
            size_t m_texture_lines = 1;

            unsigned int m_tex_per_line_min = 0;
            unsigned int m_tex_per_line_max = 0;
            unsigned int m_tex_width_min = 0;
            unsigned int m_tex_width_max = 0;
            unsigned int m_tex_height = 0;

            unsigned int m_long_lines = 0;
            unsigned int m_long_line_wide_textures = 0;
            unsigned int m_short_line_wide_textures = 0;
            unsigned int m_tall_textures = 0;

            void add_texture(std::shared_ptr<TextureComponent> &&tex);
            void adjust_lines_num();
            std::queue<std::shared_ptr<TextureComponent>> *get_textures_queue();
            void calculate_texture_traits(const SDL_Rect *main_rect);
            SDL_Rect calculate_texture_in_line(const SDL_Rect *main_rect,
                                               unsigned int line_idx,
                                               unsigned int tex_in_line_idx,
                                               unsigned int *tex_in_this_line);
            friend class RendererComponent;
        };
        SDLHandle m_sdlHdl;
        TextureHandle m_textureHdl;
        const TransformComponent *m_transform;
        friend class GameObject;
    protected:
        RendererComponent(const RenderContext &context, const TransformComponent *transform);
    public:
        RendererComponent(const RendererComponent &) = delete;
        RendererComponent &operator=(const RendererComponent &) = delete;
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
        void AddTexture(std::shared_ptr<TextureComponent> tex);

        void OnUpdate() override;
    };

} // GameEngine