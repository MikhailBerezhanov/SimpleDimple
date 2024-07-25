#pragma once

#include <InputEvent.h>

#include <gmock/gmock.h>

namespace GameEngine::Testing
{
    struct InputEventMock : GameEngine::IInputEvent
    {
        MOCK_METHOD(void, OnKeyPress, (int), (override));
    };
}