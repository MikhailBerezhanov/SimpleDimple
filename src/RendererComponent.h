
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
        class TexRect {
        private:
            std::shared_ptr<TextureComponent> m_texture = nullptr;
            SDL_Rect m_rect{};
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

            unsigned int m_line_idx = 0;
            unsigned int m_tex_in_line_idx = 0;

            Pos2D m_offset;
            int m_x_carry = 0;

            void set_texture_lines(unsigned int lines);
            void add_texture(const std::shared_ptr<TextureComponent> &tex);
            void calculate_texture_traits(const SDL_Rect *main_rect); // call once
            TexRect get_texture_and_rect(); // call for each texture
            friend class RendererComponent;
        };
        SDLHandle m_sdlHdl;
        TextureHandle m_textureHdl;
        const std::shared_ptr<const TransformComponent> m_transform;
        void update_textures();
        friend class GameObject;
    protected:
        RendererComponent(const RenderContext &context, const std::shared_ptr<const TransformComponent> &transform);
    public:
        RendererComponent(const RendererComponent &) = delete;
        RendererComponent &operator=(const RendererComponent &) = delete;
        RendererComponent(RendererComponent &&) = delete;
        RendererComponent &operator=(RendererComponent &&) = delete;
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
        void AddTexture(const std::shared_ptr<TextureComponent> &tex);
        void SetTextureRows(unsigned int rows);

        void OnUpdate() override;
    };

} // GameEngine