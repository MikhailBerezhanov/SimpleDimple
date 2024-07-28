#pragma once

#include "IWindow.h"

#include <memory>

namespace GameEngine
{
    struct IGameLoop
    {
        virtual ~IGameLoop() = default;

        virtual void SetWindow(const std::shared_ptr<IWindow>& window) = 0;
        virtual void Run() = 0;
    };
} // namespace GameEngine
