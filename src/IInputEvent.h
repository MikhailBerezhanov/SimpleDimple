#pragma once

#include <memory>

namespace GameEngine
{
    struct IInputEventSubscriber
    {
        virtual ~IInputEventSubscriber() = default;

        virtual void OnKeyUp(int keyCode) = 0;
        virtual void OnKeyDown(int keyCode) = 0;
    };

    struct IInputEventPublisher
    {
        virtual ~IInputEventPublisher() = default;

        virtual void SubscribeToInputEvents(const std::shared_ptr<IInputEventSubscriber>& sub) = 0;
        virtual void UnsubscribeFromInputEvents(const std::shared_ptr<IInputEventSubscriber>& sub) = 0;
    };
} // namespace GameEngine