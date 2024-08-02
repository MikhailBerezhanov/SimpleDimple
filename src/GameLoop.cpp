#include "GameLoop.h"
#include "ErrorHandling.h"

#include "IInputEvent.h"    // KeyCodes

#include "sdl.h"

namespace GameEngine
{
    constexpr KeyCodes SdlScancodeToKeyCodes(SDL_Scancode scancode)
    {
        switch (scancode)
        {
            case SDL_SCANCODE_UP: return KeyCodes::ARROW_UP;
            case SDL_SCANCODE_LEFT: return KeyCodes::ARROW_LEFT;
            case SDL_SCANCODE_DOWN: return KeyCodes::ARROW_DOWN;
            case SDL_SCANCODE_RIGHT: return KeyCodes::ARROW_RIGHT;

            case SDL_SCANCODE_W: return KeyCodes::W;
            case SDL_SCANCODE_A: return KeyCodes::A;
            case SDL_SCANCODE_S: return KeyCodes::S;
            case SDL_SCANCODE_D: return KeyCodes::D;

            default: return KeyCodes::UNSUPPORTED;
        }
    }

    GameLoop::GameLoop()
        : Logable("GameLoop")
        , InputEventPublisher()
    {
        EXPECT_MSG(SDL_Init(SDL_INIT_VIDEO) == 0, "SDL_Init failed: " << SDL_GetError());
    }

    GameLoop::~GameLoop()
    {
        SDL_Quit();
    }

    void GameLoop::SetWindow(const std::shared_ptr<IWindow>& window)
    {
        // TODO: collection of windows?
        m_window = window;
    }

    void GameLoop::Run()
    {
        EXPECT(m_window);

        LOG_INFO("Starting");

        bool isStopped = false;
        while (!isStopped)
        {
            isStopped = poll_events();

            m_window->Clear();
            m_window->Update();
            m_window->Present();
            // calculates to 60 fps
            SDL_Delay(1000 / 60);
        }

        LOG_INFO("Stopped");
    }

    void GameLoop::handle_key_events(SDL_Event &event) {
        // don't handle repeated events (pressed-and-held keys)
        if (event.key.repeat) return;
        const auto keyCode = SdlScancodeToKeyCodes(event.key.keysym.scancode);
        switch (event.type) {
            case SDL_KEYDOWN:
                // handle keydown
                OnKeyDown(keyCode);
                break;
            case SDL_KEYUP:
                // handle keyup
                OnKeyUp(keyCode);
                break;
        }
    }

    bool GameLoop::poll_events()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    return true;
                }
                // filter keyboard keys events
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                {
                    handle_key_events(event);
                }
            }
        }

        return false;
    }

} //namespace GameEngine