#include <stdexcept>
#include "ErrorHandling.h"
#include "TextureComponent.h"

#define EXPECT_SDL(condition, message) \
    EXPECT_MSG(condition, message << ": " << SDL_GetError())

namespace GameEngine {

    /// Nested class
    TextureComponent::SDLHandle::SDLHandle(SDL_Renderer *renderer, const std::string &image) {
        auto surface = IMG_Load(image.c_str());
        EXPECT_SDL(surface, "Unable to create surface from image");
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface); // no need for surface anymore
        EXPECT_SDL(m_texture, "Unable to create texture");
    }

    TextureComponent::SDLHandle::SDLHandle(SDL_Renderer *renderer, const GameEngine::Size2D &size)
            : m_texture(
            SDL_CreateTexture(
                    renderer,
                    SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, // todo: parameterize?
                    SDL_TextureAccess::SDL_TEXTUREACCESS_STATIC, // todo: parameterize?
                    size.w,
                    size.h
            ))
    {
        EXPECT_SDL(m_texture, "Unable to create texture");
    }

    TextureComponent::SDLHandle::~SDLHandle() {
        SDL_DestroyTexture(m_texture);
    }

    /// Texture class
    TextureComponent::TextureComponent(const RenderContext &render_context, const std::string &image)
        : m_sdlHandle(render_context.m_renderer, image)
    {}

    TextureComponent::TextureComponent(const RenderContext &render_context, const Size2D &size)
        : m_sdlHandle(render_context.m_renderer, size)
    {}

    void TextureComponent::SetColorMode(const RGBColor &rgb) const
    {
        EXPECT_SDL(SDL_SetTextureColorMod(m_sdlHandle.m_texture, rgb.r, rgb.g, rgb.b) == 0,
               "Unable to set color mode");
    }

    void TextureComponent::SetAlphaMode(uint8_t alpha) const {
        EXPECT_SDL(SDL_SetTextureAlphaMod(m_sdlHandle.m_texture, alpha) == 0,
               "Unable to set alpha mode");
    }

    void TextureComponent::SetPixelData(const std::vector<uint8_t> &pixelData) const {
        // todo: parameterize pitch?
        EXPECT_SDL(SDL_UpdateTexture(m_sdlHandle.m_texture,
                                 nullptr, // update whole texture
                                 pixelData.data(), // pixel data
                                 4 // pitch - the number of bytes in a row of pixel data, including padding (4 for RGBA)
                                 ) == 0,
               "Unable to set pixel data");
    }


    Size2D TextureComponent::GetSize() const {
        // get texture w/h
        int w, h;
        EXPECT_SDL(SDL_QueryTexture(m_sdlHandle.m_texture,
                                nullptr,
                                nullptr,
                                &w,
                                &h) == 0,
               "Unable to query texture");
        return {w, h};
    }

    SDL_Texture *TextureComponent::get_texture() const {
        return m_sdlHandle.m_texture;
    }

    void TextureComponent::OnUpdate() {
        // do nothing
    }

} // namespace GameEngine