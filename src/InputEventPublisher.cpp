
#include "InputEventPublisher.h"

namespace GameEngine
{
    void InputEventPublisher::SubscribeToInputEvents(const std::shared_ptr<IInputEventSubscriber>& sub)
    {
        m_subscribers.push_back(sub);
    }

    void InputEventPublisher::UnsubscribeFromInputEvents(const std::shared_ptr<IInputEventSubscriber>& sub)
    {
        remove_subscriber(sub);
    }

    void InputEventPublisher::OnKeyUp(int keyCode)
    {
        call_subscribers(&IInputEventSubscriber::OnKeyUp, keyCode);
    }

    void InputEventPublisher::OnKeyDown(int keyCode)
    {
        call_subscribers(&IInputEventSubscriber::OnKeyDown, keyCode);
    }

} // namespace GameEngine
