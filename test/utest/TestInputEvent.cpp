#include "Mock.h"

#include <gtest/gtest.h>

using namespace GameEngine;
using namespace GameEngine::Testing;

TEST(InputEventPublisher, ShouldSafelyUnsubscribeNonExistingSubscribers)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber = std::make_shared<InputEventMock>();
    
    sut->UnsubscribeFromInputEvents(subscriber);
}

TEST(InputEventPublisher, ShouldPublishOnKeyPressEventToOneSubscriber)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber = std::make_shared<InputEventMock>();
    const auto testKeyCode = 10;
    EXPECT_CALL(*subscriber, OnKeyPress(testKeyCode)).Times(1);

    sut->SubscribeToInputEvents(subscriber);

    sut->OnKeyPress(testKeyCode);
}

TEST(InputEventPublisher, ShouldPublishOnKeyPressEventsToAllSubscribers)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscribersNumber = 1'000;
    const auto testKeyCode = 43;

    std::vector<std::shared_ptr<InputEventMock>> subscribersHolder;
    subscribersHolder.reserve(subscribersNumber);

    for (auto i = 0; i < subscribersNumber; ++i)
    {
        const auto subscriber = std::make_shared<InputEventMock>();
        EXPECT_CALL(*subscriber, OnKeyPress(testKeyCode)).Times(1);

        sut->SubscribeToInputEvents(subscriber);
        subscribersHolder.push_back(subscriber);
    }

    sut->OnKeyPress(testKeyCode);
}

TEST(InputEventPublisher, ShouldPublishOnKeyPressEventsToAllSubscribersMultipleTimes)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscribersNumber = 100;
    const auto testKeyCode = 0;
    const auto numberOfEvents = 10;

    std::vector<std::shared_ptr<InputEventMock>> subscribersHolder;
    subscribersHolder.reserve(subscribersNumber);

    for (auto i = 0; i < subscribersNumber; ++i)
    {
        const auto subscriber = std::make_shared<InputEventMock>();
        EXPECT_CALL(*subscriber, OnKeyPress(testKeyCode)).Times(numberOfEvents);

        sut->SubscribeToInputEvents(subscriber);
        subscribersHolder.push_back(subscriber);
    }

    for (auto i = 0; i < numberOfEvents; ++i)
    {
        sut->OnKeyPress(testKeyCode);
    }
}

TEST(InputEventPublisher, ShouldUnsubscribeSubscriber)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber = std::make_shared<InputEventMock>();
    
    sut->SubscribeToInputEvents(subscriber);
    EXPECT_CALL(*subscriber, OnKeyPress).Times(1);
    sut->OnKeyPress(1);

    sut->UnsubscribeFromInputEvents(subscriber);
    EXPECT_CALL(*subscriber, OnKeyPress).Times(0);
    sut->OnKeyPress(1);
}

TEST(InputEventPublisher, ShouldIngoreDestoyedSubscriber)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    {
        const auto subscriber = std::make_shared<InputEventMock>();
        EXPECT_CALL(*subscriber, OnKeyPress).Times(0);
        sut->SubscribeToInputEvents(subscriber);
        // subscriber should be destroyed out of scope
    }
    
    sut->OnKeyPress(1);
}

TEST(InputEventPublisher, ShouldNotUnsubscribeSubscriberIfAnotherUnsubscribed)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber1 = std::make_shared<InputEventMock>();
    sut->SubscribeToInputEvents(subscriber1);

    const auto subscriber2 = std::make_shared<InputEventMock>();    
    sut->SubscribeToInputEvents(subscriber2);

    const auto subscriber3 = std::make_shared<InputEventMock>();

    // Removing subscribed subscriber
    sut->UnsubscribeFromInputEvents(subscriber2);

    // Removing not subscribed subscriber
    sut->UnsubscribeFromInputEvents(subscriber3);

    // Removing empty subscriber
    sut->UnsubscribeFromInputEvents({});
    sut->UnsubscribeFromInputEvents(nullptr);
    
    EXPECT_CALL(*subscriber1, OnKeyPress).Times(2);
    EXPECT_CALL(*subscriber2, OnKeyPress).Times(0);
    EXPECT_CALL(*subscriber3, OnKeyPress).Times(0);
    sut->OnKeyPress(1);
    sut->OnKeyPress(2);
}

TEST(InputEventPublisher, ShouldDeliverEventsIfSubscriberThrowsStdException)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto numberOfEvents = 3;

    const auto subscriber1 = std::make_shared<InputEventMock>();
    EXPECT_CALL(*subscriber1, OnKeyPress).Times(numberOfEvents);
    sut->SubscribeToInputEvents(subscriber1);

    const auto throwingSubscriber = std::make_shared<InputEventMock>();
    EXPECT_CALL(*throwingSubscriber, OnKeyPress).WillRepeatedly(
        [](auto){ throw std::runtime_error("throwingSubscriber exception"); });

    sut->SubscribeToInputEvents(throwingSubscriber);

    const auto subscriber2 = std::make_shared<InputEventMock>();
    EXPECT_CALL(*subscriber2, OnKeyPress).Times(numberOfEvents);
    sut->SubscribeToInputEvents(subscriber2);

    for (auto i = 0; i < numberOfEvents; ++i)
    {
        sut->OnKeyPress(i);
    }
}
