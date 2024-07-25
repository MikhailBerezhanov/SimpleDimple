#pragma once

#include "ErrorHandling.h"

#include <iostream>

#include <mutex>
#include <vector>
#include <memory>
#include <algorithm>


namespace GameEngine
{

struct IInputEvent
{
    virtual ~IInputEvent() = default;

    virtual void OnKeyPress(int keyCode) = 0;
};

struct IInputEventPublisher
{
    virtual ~IInputEventPublisher() = default;

    virtual void SubscribeToInputEvents(const std::shared_ptr<IInputEvent>& sub) = 0;
    virtual void UnsubscribeFromInputEvents(const std::shared_ptr<IInputEvent>& sub) = 0;
};

class InputEventPublisher : public IInputEvent, public IInputEventPublisher
{
private:
    std::mutex m_mtx;
    std::vector<std::weak_ptr<IInputEvent>> m_subscribers;

private:
    auto remove_subscriber(const std::shared_ptr<IInputEvent>& sub)
    {
        auto it = std::remove_if(m_subscribers.begin(), m_subscribers.end(),
            [&sub](const std::weak_ptr<IInputEvent>& wp)
            {
                return !sub.owner_before(wp) && !wp.owner_before(sub);
            });

        return m_subscribers.erase(it, m_subscribers.end());
    }

    template <typename Callable, typename ...Args>
    void call_subscribers(Callable c, Args&& ... args)
    {
        const std::unique_lock lock(m_mtx);

        for (auto it = m_subscribers.begin(); it != m_subscribers.end();)
        {
            try
            {
                const auto sp = it->lock();

                if (sp)
                {
                    ++it;
                    std::invoke(c, sp, std::forward<Args>(args)...);
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
    void SubscribeToInputEvents(const std::shared_ptr<IInputEvent>& sub) override
    {
        const std::unique_lock lock(m_mtx);
        m_subscribers.push_back(sub);
    }

    void UnsubscribeFromInputEvents(const std::shared_ptr<IInputEvent>& sub) override
    {
        const std::unique_lock lock(m_mtx);
        remove_subscriber(sub);
    }

    // IInputEvent
    void OnKeyPress(int keyCode) override
    {
        call_subscribers(&IInputEvent::OnKeyPress, keyCode);
    }
};

} // namespace