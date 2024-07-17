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
        const auto main_pos = m_transform->GetPosition();
        EXPECT(SDL_RenderDrawPoint(m_sdlHandle.m_renderer,
                                   main_pos.x + point.x,
                                   main_pos.y + point.y
                                   ) == 0,
               "Error drawing point");
    }

    void RendererComponent::DrawPoints(const std::vector<Pos2D> &points) const {
        const auto main_pos = m_transform->GetPosition();
        std::vector<SDL_Point> sdl_points{};
        sdl_points.reserve(points.size());
        for (const auto &p : points) {
            sdl_points.emplace_back(main_pos.x + p.x, main_pos.y + p.y);
        }
        EXPECT(SDL_RenderDrawPoints(m_sdlHandle.m_renderer,
                                    sdl_points.data(),
                                    static_cast<int>(sdl_points.size())
                                    ) == 0, "Error drawing points");
    }

    void RendererComponent::DrawLine(const Pos2D &start, const Pos2D &end) const {
        const auto main_pos = m_transform->GetPosition();
        EXPECT(SDL_RenderDrawLine(m_sdlHandle.m_renderer,
                                  main_pos.x + start.x,
                                  main_pos.y + start.y,
                                  main_pos.x + end.x,
                                  main_pos.y + end.y
                                  ) == 0, "Error drawing line");
    }

    void RendererComponent::DrawLines(const std::vector<Pos2D> &points) const {
        const auto main_pos = m_transform->GetPosition();
        std::vector<SDL_Point> sdl_points{};
        sdl_points.reserve(points.size());
        for (const auto &p : points) {
            sdl_points.emplace_back(main_pos.x + p.x, main_pos.y + p.y);
        }
        EXPECT(SDL_RenderDrawLines(m_sdlHandle.m_renderer,
                                   sdl_points.data(),
                                   static_cast<int>(points.size())
                                   ) == 0, "Error drawing lines");
    }

    void RendererComponent::DrawRect(const Rect &rect) const {
        const auto main_pos = m_transform->GetPosition();
        const SDL_Rect sdl_rect {
            main_pos.x,
            main_pos.y,
            rect.w,
            rect.h
        };
        EXPECT(SDL_RenderDrawRect(m_sdlHandle.m_renderer,
                                  &sdl_rect) == 0, "Error drawing rect");
    }

    void RendererComponent::DrawRects(const std::vector<Rect> &rects) const {
        const auto main_pos = m_transform->GetPosition();
        std::vector<SDL_Rect> sdl_rects{};
        sdl_rects.reserve(rects.size());
        for(const auto &r : rects) {
            sdl_rects.emplace_back(
                    main_pos.x + r.x,
                    main_pos.y + r.y,
                    r.w,
                    r.h);
        }
        EXPECT(SDL_RenderDrawRects(m_sdlHandle.m_renderer,
                                   sdl_rects.data(),
                                   static_cast<int>(rects.size())
                                   ) == 0, "Error drawing rects");
    }

    void RendererComponent::FillRect(const Rect &rect) const {
        const auto main_pos = m_transform->GetPosition();
        const SDL_Rect sdl_rect {
                main_pos.x,
                main_pos.y,
                rect.w,
                rect.h
        };
        EXPECT(SDL_RenderFillRect(m_sdlHandle.m_renderer,
                                  &sdl_rect
                                  ) == 0, "Error filling rect");
    }

    void RendererComponent::FillRects(const std::vector<Rect> &rects) const {
        const auto main_pos = m_transform->GetPosition();
        std::vector<SDL_Rect> sdl_rects{};
        sdl_rects.reserve(rects.size());
        for(const auto &r : rects) {
            sdl_rects.emplace_back(
                    main_pos.x + r.x,
                    main_pos.y + r.y,
                    r.w,
                    r.h);
        }
        EXPECT(SDL_RenderFillRects(m_sdlHandle.m_renderer,
                                   sdl_rects.data(),
                                   static_cast<int>(rects.size())
                                   ) == 0, "Error filling rects");
    }

    RenderContext RendererComponent::GetRenderContext() const {
        return RenderContext(m_sdlHandle.m_renderer);
    }

    void RendererComponent::AddTexture(const TextureComponent *tex) {
        m_textures_q.push(tex);
    }

    void RendererComponent::OnUpdate() {

        /// dstrect is calculated according to
        /// the size of transform rect and the number of textures
        /// We have a number of lines and a collection of textures to fit into larger rectangle
        /// each texture's height TEX_H = H / LINES
        /// number of textures in line (max) TEX_PER_LINE = NUM_TEX / LINES + NUM_TEX % LINES
        /// each texture's width (min) = W / TEX_PER_LINE
        /// in case where we cannot distribute textures evenly by the lines, the last line's textures are widened

        /// if too many lines, adjust them
        if (m_texture_lines > m_textures_q.size()) {
            m_texture_lines = m_textures_q.size();
        }

        const auto main_rect = m_transform->get_rect();
        const auto tex_per_line_min = m_textures_q.size() / m_texture_lines;
        const auto tex_per_line_max =
                tex_per_line_min +
                m_textures_q.size() % m_texture_lines ? 1 : 0;
        const auto tex_width_min = static_cast<int>(main_rect->w / tex_per_line_max);
        const auto tex_width_max = static_cast<int>(main_rect->w / tex_per_line_min);
        const auto tex_height = static_cast<int>(main_rect->h / m_texture_lines);

        ///Remainders
        auto tex_per_line_remainder = m_textures_q.size() % m_texture_lines;
        auto tex_width_min_remainder = static_cast<int>(main_rect->w % tex_per_line_max);
        auto tex_width_max_remainder = static_cast<int>(main_rect->w % tex_per_line_min);
        auto tex_height_remainder = static_cast<int>(main_rect->h % m_texture_lines);

        int line_idx = 0;
        int tex_in_line_idx = 0;

        while(! m_textures_q.empty()) {
            const auto tex = m_textures_q.front();
            m_textures_q.pop();

            // first tex_per_line_remainder lines get additional texture
            const bool is_long_line = (line_idx < tex_per_line_remainder);
            const auto tex_in_this_line = is_long_line ? tex_per_line_max : tex_per_line_min;
            // first tex_height_remainder lines are 1 pixel taller
            const auto tex_height_in_this_line = tex_height + (line_idx < tex_height_remainder ? 1 : 0);
            // calculate current tex's width
            int tex_width_in_this_line;
            if (is_long_line) {
                tex_width_in_this_line = tex_width_min +
                        // first remainder textures get additional pixel
                        (tex_in_line_idx < tex_width_min_remainder ? 1 : 0);
            }else {
                tex_width_in_this_line = tex_width_max +
                        // first remainder textures get additional pixel
                        (tex_in_line_idx < tex_width_max_remainder ? 1 : 0);
            }

            /// Recalculate rect
            const SDL_Rect dstrect {
                    main_rect->x + tex_in_line_idx * tex_width_in_this_line,
                    main_rect->y + line_idx * tex_height_in_this_line,
                    tex_width_in_this_line,
                    tex_height_in_this_line
            };

            if(++tex_in_line_idx >= tex_in_this_line) {
                tex_in_line_idx = 0;
                ++line_idx;
            }

            if (m_transform->get_angle() != 0.0 || m_transform->get_flip() != SDL_FLIP_NONE) {
                // special treatment for flip and rotation
                EXPECT(SDL_RenderCopyEx(m_sdlHandle.m_renderer, // sdl renderer
                                      tex->get_texture(), // sdl texture
                                      nullptr, // apply to whole texture
                                      &dstrect, // texture destination
                                      m_transform->get_angle(), // rotation angle
                                      m_transform->get_center(), // rotation center (if null, rotate around dstrect.w / 2, dstrect.h / 2)
                                      m_transform->get_flip()) // flip action
                                      == 0, "Unable to render-copy texture");
            }
            else {
                EXPECT(SDL_RenderCopy(m_sdlHandle.m_renderer, // sdl renderer
                                      tex->get_texture(), // sdl texture
                                      nullptr, // apply to whole texture
                                      m_transform->get_rect() // texture destination
                                      ) == 0, "Unable to render-copy texture");
            }

        }
    }

} // GameEngine