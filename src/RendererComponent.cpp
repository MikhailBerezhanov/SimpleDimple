
#include <stdexcept>

#include "ErrorHandling.h"
#include "Logger.h"
#include "RendererComponent.h"

#define EXPECT_SDL(condition, message) \
    EXPECT_MSG(condition, message << ": " << SDL_GetError())

namespace GameEngine {

    RendererComponent::SDLHandle::SDLHandle(SDL_Renderer *rend)
        : m_renderer(rend)
        {}

    void RendererComponent::TextureHandle::add_texture(const std::shared_ptr<TextureComponent> &tex) {
        m_textures_q.emplace(tex);
    }

    void RendererComponent::TextureHandle::calculate_texture_traits(const SDL_Rect *main_rect) {

        /// if too many lines, adjust them
        if (!m_textures_q.empty()) {
            m_texture_lines = std::min(m_texture_lines, m_textures_q.size());
        }

        /// dstrect is calculated according to
        /// the size of transform rect and the number of textures
        /// We have a number of lines and a collection of textures to fit into larger rectangle
        /// each texture's height TEX_H = H / LINES
        /// number of textures in line (max) TEX_PER_LINE = NUM_TEX / LINES + NUM_TEX % LINES
        /// each texture's width (min) = W / TEX_PER_LINE
        /// in case where we cannot distribute textures evenly by the lines, the last line's textures are widened
        const auto rem = m_texture_lines ? m_textures_q.size() % m_texture_lines : 0;
        m_tex_per_line_min = m_texture_lines ? m_textures_q.size() / m_texture_lines : 0;
        m_tex_per_line_max = m_tex_per_line_min + (rem ? 1 : 0);
        m_tex_width_min = m_tex_per_line_max ? main_rect->w / m_tex_per_line_max : 0;
        m_tex_width_max = m_tex_per_line_min ? main_rect->w / m_tex_per_line_min : 0;
        m_tex_height = m_texture_lines ? main_rect->h / m_texture_lines : 0;
        ///Padding
        m_long_lines = m_texture_lines ? m_textures_q.size() % m_texture_lines : 0;
        m_long_line_wide_textures = m_tex_per_line_max ? main_rect->w % m_tex_per_line_max : 0;
        m_short_line_wide_textures = m_tex_per_line_min ? main_rect->w % m_tex_per_line_min : 0;
        m_tall_textures = m_texture_lines ? main_rect->h % m_texture_lines : 0;
        /// Clear indexes
        m_line_idx = 0;
        m_tex_in_line_idx = 0;

        m_offset = {main_rect->x, main_rect->y};
        m_x_carry = 0;

    }

    RendererComponent::TexRect RendererComponent::TextureHandle::get_texture_and_rect() {

        TexRect res{};

        if (! m_textures_q.empty()) {

            // first m_long_lines lines get additional texture
            const bool is_long_line = m_line_idx < m_long_lines;
            // determine the number of textures in current line
            const auto tex_in_current_line = is_long_line ?
                                    m_tex_per_line_max :
                                    m_tex_per_line_min;

            // first m_tall_textures lines are 1 pixel taller
            const auto tex_height_in_this_line = m_tex_height +
                                                 (m_line_idx < m_tall_textures ? 1 : 0);
            // calculate current tex's width
            unsigned int tex_width_in_this_line = 0;
            if (is_long_line) {
                tex_width_in_this_line = m_tex_width_min +
                                         // first m_long_line_wide_textures textures get additional pixel
                                         (m_tex_in_line_idx < m_long_line_wide_textures ? 1 : 0);
            }else {
                tex_width_in_this_line = m_tex_width_max +
                                         // first m_short_line_wide_textures textures get additional pixel
                                         (m_tex_in_line_idx < m_short_line_wide_textures ? 1 : 0);
            }

            res.m_rect = {
                    static_cast<int>(m_offset.x + m_x_carry),
                    static_cast<int>(m_offset.y),
                    static_cast<int>(tex_width_in_this_line),
                    static_cast<int>(tex_height_in_this_line)
            };

            res.m_texture = m_textures_q.front();

            m_textures_q.pop();

            m_x_carry += static_cast<int>(tex_width_in_this_line);

            if(++m_tex_in_line_idx >= tex_in_current_line) {
                m_tex_in_line_idx = 0;
                m_offset.y += static_cast<int>(tex_height_in_this_line);
                m_x_carry = 0;
                ++m_line_idx;
            }
        }

        return res;
    }

    RendererComponent::RendererComponent(const RenderContext &context, const std::shared_ptr<const TransformComponent> &transform)
        : m_sdlHdl(context.m_renderer),
          m_transform(transform)
        {}

    void RendererComponent::update_textures() {

        const auto main_rect = m_transform->get_rect();
        /// calculate coordinates from rect
        m_textureHdl.calculate_texture_traits(main_rect);

        TexRect tex_rect{};
        while((tex_rect = m_textureHdl.get_texture_and_rect()).m_texture != nullptr) {

            auto texture = tex_rect.m_texture->get_texture();
            const auto dst_rect = &tex_rect.m_rect;

            if (m_transform->get_angle() != 0.0 || m_transform->get_flip() != SDL_FLIP_NONE) {
                // special treatment for flip and rotation
                EXPECT_SDL(SDL_RenderCopyEx(m_sdlHdl.m_renderer, // sdl m_renderer
                                            texture, // sdl texture
                                            nullptr, // apply to whole texture
                                            dst_rect, // texture destination
                                            m_transform->get_angle(), // rotation angle
                                            m_transform->get_center(), // rotation center (if null, rotate around dst_rect.w / 2, dst_rect.h / 2)
                                            m_transform->get_flip() // flip action
                ) == 0, "Unable to render-copy texture");
            }
            else {
                EXPECT_SDL(SDL_RenderCopy(m_sdlHdl.m_renderer, // sdl m_renderer
                                          texture, // sdl texture
                                          nullptr, // apply to whole texture
                                          dst_rect // texture destination
                ) == 0, "Unable to render-copy texture");
            }
        }
    }

    void RendererComponent::SetDrawColor(const RGBColor &rgba) {
        EXPECT_SDL(SDL_SetRenderDrawColor(m_sdlHdl.m_renderer, rgba.r, rgba.g, rgba.b, rgba.a) == 0,
               "Error setting renderer color");
    }

    void RendererComponent::DrawPoint(const Pos2D &point) const {
        const auto main_pos = m_transform->GetPosition();
        EXPECT_SDL(SDL_RenderDrawPoint(m_sdlHdl.m_renderer,
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
            sdl_points.emplace_back(SDL_Point{main_pos.x + p.x, main_pos.y + p.y});
        }
        EXPECT_SDL(SDL_RenderDrawPoints(m_sdlHdl.m_renderer,
                                    sdl_points.data(),
                                    static_cast<int>(sdl_points.size())
                                    ) == 0, "Error drawing points");
    }

    void RendererComponent::DrawLine(const Pos2D &start, const Pos2D &end) const {
        const auto main_pos = m_transform->GetPosition();
        EXPECT_SDL(SDL_RenderDrawLine(m_sdlHdl.m_renderer,
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
            sdl_points.emplace_back(SDL_Point{main_pos.x + p.x, main_pos.y + p.y});
        }
        EXPECT_SDL(SDL_RenderDrawLines(m_sdlHdl.m_renderer,
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
        EXPECT_SDL(SDL_RenderDrawRect(m_sdlHdl.m_renderer,
                                  &sdl_rect) == 0, "Error drawing rect");
    }

    void RendererComponent::DrawRects(const std::vector<Rect> &rects) const {
        const auto main_pos = m_transform->GetPosition();
        std::vector<SDL_Rect> sdl_rects{};
        sdl_rects.reserve(rects.size());
        for(const auto &r : rects) {
            sdl_rects.emplace_back(SDL_Rect{
                    main_pos.x + r.x,
                    main_pos.y + r.y,
                    r.w,
                    r.h});
        }
        EXPECT_SDL(SDL_RenderDrawRects(m_sdlHdl.m_renderer,
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
        EXPECT_SDL(SDL_RenderFillRect(m_sdlHdl.m_renderer,
                                  &sdl_rect
                                  ) == 0, "Error filling rect");
    }

    void RendererComponent::FillRects(const std::vector<Rect> &rects) const {
        const auto main_pos = m_transform->GetPosition();
        std::vector<SDL_Rect> sdl_rects{};
        sdl_rects.reserve(rects.size());
        for(const auto &r : rects) {
            sdl_rects.emplace_back(SDL_Rect{
                    main_pos.x + r.x,
                    main_pos.y + r.y,
                    r.w,
                    r.h});
        }
        EXPECT_SDL(SDL_RenderFillRects(m_sdlHdl.m_renderer,
                                   sdl_rects.data(),
                                   static_cast<int>(rects.size())
                                   ) == 0, "Error filling rects");
    }

    RenderContext RendererComponent::GetRenderContext() const {
        return RenderContext(m_sdlHdl.m_renderer);
    }

    void RendererComponent::AddTexture(const std::shared_ptr<TextureComponent> &tex) {
        m_textureHdl.add_texture(tex);
    }

    void RendererComponent::OnUpdate() {

        update_textures();

    }




} // GameEngine