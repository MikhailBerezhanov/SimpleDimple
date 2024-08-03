#pragma once

#include "IGameLoop.h"

#include "Logger.h"
#include "InputEventPublisher.h"

#include "sdl.h"

#include <memory>

namespace GameEngine
{
    class GameLoop : private Logable,
                     public InputEventPublisher,
                     public IGameLoop
    {
    private:
        std::shared_ptr<IWindow> m_window;

    private:
        bool poll_events();
        void handle_key_events(const SDL_Event &event);
    public:
        GameLoop();
        ~GameLoop();

        // IGameLoop
        void SetWindow(const std::shared_ptr<IWindow>& window) override;
        void Run() override;
    };
} // namespace GameEngine