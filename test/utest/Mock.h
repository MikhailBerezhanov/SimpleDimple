#pragma once

#include <IInputEvent.h>

#include <gmock/gmock.h>

namespace GameEngine::Testing
{
    struct InputEventSubscriberMock : GameEngine::IInputEventSubscriber
    {
        MOCK_METHOD(void, OnKeyUp, (GameEngine::KeyCodes), (override));
        MOCK_METHOD(void, OnKeyDown, (GameEngine::KeyCodes), (override));
    };
}