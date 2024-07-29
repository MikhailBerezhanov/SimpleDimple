#pragma once

#include <memory>

namespace GameEngine
{
    enum class KeyCodes
    {
        UNSUPPORTED = 0,

        ARROW_UP,
        ARROW_LEFT,
        ARROW_DOWN,
        ARROW_RIGHT,

        W,
        A,
        S,
        D,
    };

    struct IInputEventSubscriber
    {
        virtual ~IInputEventSubscriber() = default;

        virtual void OnKeyUp(KeyCodes keyCode) = 0;
        virtual void OnKeyDown(KeyCodes keyCode) = 0;
    };

    struct IInputEventPublisher
    {
        virtual ~IInputEventPublisher() = default;

        virtual void SubscribeToInputEvents(const std::shared_ptr<IInputEventSubscriber>& sub) = 0;
        virtual void UnsubscribeFromInputEvents(const std::shared_ptr<IInputEventSubscriber>& sub) = 0;
    };
} // namespace GameEngine