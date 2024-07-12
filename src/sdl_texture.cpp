#include <stdexcept>

#include "sdl_texture.h"

#define INVALID_POS INT32_MIN

#define EXPECT(condition, message) do { \
    if (!(condition)) { \
        throw std::runtime_error(std::string(__func__) \
        + " " + message \
        + ": " + SDL_GetError()); \
    }} while(0)

namespace GameEngine
{
    Texture::Texture(SDL_Renderer *renderer, const std::string &image) {
        auto surface = IMG_Load(image.c_str());
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        EXPECT(m_texture, "Unable to create texture");
        // get texture w/h
        EXPECT(SDL_QueryTexture(m_texture, nullptr, nullptr, &m_rect.w, &m_rect.h) == 0, "Unable to query texture");
        reset_mods();
    }

    Texture::Texture(SDL_Renderer *renderer, const Size2D &size)
        : m_texture(
            SDL_CreateTexture(
                    renderer,
                    SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, // todo: parameterize?
                    SDL_TextureAccess::SDL_TEXTUREACCESS_STATIC, // todo: parameterize?
                    static_cast<int>(size.w),
                    static_cast<int>(size.h)
            )
        )
    {
        EXPECT(m_texture, "Unable to create texture");
        // get texture w/h
        m_rect.w = static_cast<int>(size.w);
        m_rect.h = static_cast<int>(size.w);
        reset_mods();
    }


    Texture::~Texture()
    {
        SDL_DestroyTexture(m_texture);
    }

    void Texture::SetColorMode(const RGBColor &rgb) const
    {
        EXPECT(SDL_SetTextureColorMod(m_texture, rgb.r, rgb.g, rgb.b) == 0,
               "Unable to set color mode");
    }

    void Texture::SetAlphaMode(uint8_t alpha) const {
        EXPECT(SDL_SetTextureAlphaMod(m_texture, alpha) == 0,
               "Unable to set alpha mode");
    }

    void Texture::SetPixelData(const std::vector<uint8_t> &pixel_data) const {
        // todo: parameterize pitch?
        EXPECT(SDL_UpdateTexture(m_texture,
                                 nullptr, // update whole texture
                                 pixel_data.data(), // pixel data
                                 4 // pitch - the number of bytes in a row of pixel data, including padding (4 for RGBA)
                                 ) == 0,
               "Unable to set pixel data");
    }

    void Texture::SetPosition(const Pos2D &pos) {
        m_rect.x = pos.x;
        m_rect.y = pos.y;
    }

    Pos2D Texture::GetPosition() const {
        return Pos2D{m_rect.x, m_rect.y};
    }

    Size2D Texture::GetSize() const {
        return Size2D{static_cast<size_t>(m_rect.w), static_cast<size_t>(m_rect.h)};
    }

    Rect Texture::GetRect() const {
        return Rect{m_rect.x, m_rect.y, m_rect.w, m_rect.h};
    }

    void Texture::Resize(const Size2D &size) {
        m_rect.w = static_cast<int>(size.w);
        m_rect.h = static_cast<int>(size.h);
    }

    void Texture::Upscale(uint8_t factor) {
        m_rect.w *= factor;
        m_rect.h *= factor;
    }

    void Texture::Downscale(uint8_t factor) {
        m_rect.w /= factor;
        m_rect.h /= factor;
    }

    void Texture::SetAngle(double angle, const Pos2D &center) {
        m_center.x = center.x;
        m_center.y = center.y;
        m_angle = angle;
    }

    void Texture::SetAngle(double angle) {
        // invalidate center
        m_center = {INVALID_POS, INVALID_POS};
        m_angle = angle;
    }

    void Texture::FlipVertically() {
        m_flip = SDL_FLIP_VERTICAL;
    }

    void Texture::FlipHorizontally() {
        m_flip = SDL_FLIP_HORIZONTAL;
    }

    /// Mods
    const SDL_Point *Texture::get_center() const {
        if (m_center.x != INVALID_POS && m_center.y != INVALID_POS){
            return &m_center;
        }else{
            return nullptr;
        }
    }

    double Texture::get_angle() const {
        return m_angle;
    }

    SDL_RendererFlip Texture::get_flip() const {
        return m_flip;
    }

    void Texture::reset_mods() {
        m_center = {INVALID_POS, INVALID_POS};
        m_angle = 0.0;
        m_flip = SDL_FLIP_NONE;
    }

} // namespace GameEngine