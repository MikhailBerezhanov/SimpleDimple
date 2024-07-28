#pragma once

#include "ErrorHandling.h"
#include "IInputEvent.h"

#include <algorithm>
#include <vector>
#include <functional>

namespace GameEngine
{
    class InputEventPublisher : public IInputEventSubscriber, public IInputEventPublisher
    {
    private:
        std::vector<std::weak_ptr<IInputEventSubscriber>> m_subscribers;

    private:
        auto remove_subscriber(const std::shared_ptr<IInputEventSubscriber>& sub)
        {
            auto it = std::remove_if(m_subscribers.begin(), m_subscribers.end(),
                [&sub](const std::weak_ptr<IInputEventSubscriber>& wp)
                {
                    return !sub.owner_before(wp) && !wp.owner_before(sub);
                });

            return m_subscribers.erase(it, m_subscribers.end());
        }

        template <typename Callable, typename ...Args>
        void call_subscribers(Callable c, Args&& ... args)
        {
            for (auto it = m_subscribers.begin(); it != m_subscribers.end();)
            {
                try
                {
                    const auto sp = it->lock();

                    if (sp)
                    {
                        ++it;
                        std::invoke(c, sp.get(), std::forward<Args>(args)...);
                    }
                    else
                    {
                        // Subscriber has expired - remove
                        it = remove_subscriber(sp);
                    }
                }
                catch (const std::exception&)
                {
                    HANDLE_EXCEPTION_MSG("Subscriber throws");
                }
            }
        }

    public:
        // IInputEventPublisher
        void SubscribeToInputEvents(const std::shared_ptr<IInputEventSubscriber>& sub) override;
        void UnsubscribeFromInputEvents(const std::shared_ptr<IInputEventSubscriber>& sub) override;

        // IInputEventSubscriber
        void OnKeyUp(int keyCode) override;
        void OnKeyDown(int keyCode) override;
    };
} // namespace GameEngine