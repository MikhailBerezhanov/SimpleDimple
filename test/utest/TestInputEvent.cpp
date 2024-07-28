#include "Mock.h"

#include <InputEventPublisher.h>

#include <gtest/gtest.h>

using namespace ::testing;
using namespace GameEngine;
using namespace GameEngine::Testing;

TEST(InputEventPublisher, ShouldSafelyUnsubscribeNonExistingSubscribers)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber = std::make_shared<InputEventSubscriberMock>();
    
    sut->UnsubscribeFromInputEvents(subscriber);
}

TEST(InputEventPublisher, ShouldPublishOnKeyDownEventToOneSubscriber)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber = std::make_shared<InputEventSubscriberMock>();
    const auto testKeyCode = KeyCodes::ARROW_RIGHT;
    EXPECT_CALL(*subscriber, OnKeyDown(testKeyCode)).Times(1);

    sut->SubscribeToInputEvents(subscriber);

    sut->OnKeyDown(testKeyCode);
}

TEST(InputEventPublisher, ShouldPublishOnKeyDownEventsToAllSubscribers)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscribersNumber = 1'000;
    const auto testKeyCode = KeyCodes::ARROW_UP;

    std::vector<std::shared_ptr<InputEventSubscriberMock>> subscribersHolder;
    subscribersHolder.reserve(subscribersNumber);

    for (auto i = 0; i < subscribersNumber; ++i)
    {
        const auto subscriber = std::make_shared<InputEventSubscriberMock>();
        EXPECT_CALL(*subscriber, OnKeyDown(testKeyCode)).Times(1);

        sut->SubscribeToInputEvents(subscriber);
        subscribersHolder.push_back(subscriber);
    }

    sut->OnKeyDown(testKeyCode);
}

TEST(InputEventPublisher, ShouldPublishOnKeyDownEventsToAllSubscribersMultipleTimes)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscribersNumber = 100;
    const auto testKeyCode = KeyCodes::ARROW_UP;
    const auto numberOfEvents = 10;

    std::vector<std::shared_ptr<InputEventSubscriberMock>> subscribersHolder;
    subscribersHolder.reserve(subscribersNumber);

    for (auto i = 0; i < subscribersNumber; ++i)
    {
        const auto subscriber = std::make_shared<InputEventSubscriberMock>();
        EXPECT_CALL(*subscriber, OnKeyDown(testKeyCode)).Times(numberOfEvents);

        sut->SubscribeToInputEvents(subscriber);
        subscribersHolder.push_back(subscriber);
    }

    for (auto i = 0; i < numberOfEvents; ++i)
    {
        sut->OnKeyDown(testKeyCode);
    }
}

TEST(InputEventPublisher, ShouldUnsubscribeSubscriber)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber = std::make_shared<InputEventSubscriberMock>();
    
    sut->SubscribeToInputEvents(subscriber);
    EXPECT_CALL(*subscriber, OnKeyDown).Times(1);
    sut->OnKeyDown(KeyCodes::A);

    sut->UnsubscribeFromInputEvents(subscriber);
    EXPECT_CALL(*subscriber, OnKeyDown).Times(0);
    sut->OnKeyDown(KeyCodes::A);
}

TEST(InputEventPublisher, ShouldIngoreDestoyedSubscriber)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    {
        const auto subscriber = std::make_shared<InputEventSubscriberMock>();
        EXPECT_CALL(*subscriber, OnKeyDown).Times(0);
        sut->SubscribeToInputEvents(subscriber);
        // subscriber should be destroyed out of scope
    }
    
    sut->OnKeyDown(KeyCodes::D);
}

TEST(InputEventPublisher, ShouldNotUnsubscribeSubscriberIfAnotherUnsubscribed)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber1 = std::make_shared<InputEventSubscriberMock>();
    sut->SubscribeToInputEvents(subscriber1);

    const auto subscriber2 = std::make_shared<InputEventSubscriberMock>();    
    sut->SubscribeToInputEvents(subscriber2);

    const auto subscriber3 = std::make_shared<InputEventSubscriberMock>();

    // Removing subscribed subscriber
    sut->UnsubscribeFromInputEvents(subscriber2);

    // Removing not subscribed subscriber
    sut->UnsubscribeFromInputEvents(subscriber3);

    // Removing empty subscriber
    sut->UnsubscribeFromInputEvents({});
    sut->UnsubscribeFromInputEvents(nullptr);
    
    EXPECT_CALL(*subscriber1, OnKeyUp).Times(2);
    EXPECT_CALL(*subscriber2, OnKeyUp).Times(0);
    EXPECT_CALL(*subscriber3, OnKeyUp).Times(0);
    sut->OnKeyUp(KeyCodes::W);
    sut->OnKeyUp(KeyCodes::S);
}

TEST(InputEventPublisher, ShouldDeliverEventsIfSubscriberThrowsStdException)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto numberOfEvents = 3;

    const auto subscriber1 = std::make_shared<InputEventSubscriberMock>();
    EXPECT_CALL(*subscriber1, OnKeyUp).Times(numberOfEvents);
    sut->SubscribeToInputEvents(subscriber1);

    const auto throwingSubscriber = std::make_shared<InputEventSubscriberMock>();
    EXPECT_CALL(*throwingSubscriber, OnKeyUp).WillRepeatedly(
        [](auto){ throw std::runtime_error("throwingSubscriber exception"); });

    sut->SubscribeToInputEvents(throwingSubscriber);

    const auto subscriber2 = std::make_shared<InputEventSubscriberMock>();
    EXPECT_CALL(*subscriber2, OnKeyUp).Times(numberOfEvents);
    sut->SubscribeToInputEvents(subscriber2);

    for (auto i = 0; i < numberOfEvents; ++i)
    {
        sut->OnKeyUp(KeyCodes::A);
    }
}

TEST(InputEventPublisher, ShouldUnsubscribeHalfOfSubscribers)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscribersNumber = 1'000;
    const auto testKeyCode = KeyCodes::ARROW_DOWN;
    const auto testEventsNumber = 3;

    std::vector<std::shared_ptr<InputEventSubscriberMock>> subscribersHolder;
    subscribersHolder.reserve(subscribersNumber);

    for (auto i = 0; i < subscribersNumber; ++i)
    {
        const auto subscriber = std::make_shared<InputEventSubscriberMock>();

        if (i % 2)
        {
            EXPECT_CALL(*subscriber, OnKeyUp(testKeyCode)).Times(testEventsNumber);
            EXPECT_CALL(*subscriber, OnKeyDown(testKeyCode)).Times(testEventsNumber);
        }
        else
        {
            EXPECT_CALL(*subscriber, OnKeyUp(testKeyCode)).Times(0);
            EXPECT_CALL(*subscriber, OnKeyDown(testKeyCode)).Times(0);
        }

        sut->SubscribeToInputEvents(subscriber);
        subscribersHolder.push_back(subscriber);
    }

    for (auto i = 0; i < subscribersNumber; ++i)
    {
        if (i % 2 == 0)
        {
            sut->UnsubscribeFromInputEvents(subscribersHolder[i]);
        }
    }

    for (auto i = 0; i < testEventsNumber; ++i)
    {
        sut->OnKeyUp(testKeyCode);
        sut->OnKeyDown(testKeyCode);
    }
}

TEST(InputEventPublisher, ShouldPublishEventsInEventsOrder)
{
    const auto sut = std::make_unique<InputEventPublisher>();

    const auto subscriber1 = std::make_shared<InputEventSubscriberMock>();
    {
        InSequence seq;
        EXPECT_CALL(*subscriber1, OnKeyDown).Times(1);
        EXPECT_CALL(*subscriber1, OnKeyUp).Times(1);
    }
    
    const auto subscriber2 = std::make_shared<InputEventSubscriberMock>();
    {
        InSequence seq;
        EXPECT_CALL(*subscriber2, OnKeyDown).Times(1);
        EXPECT_CALL(*subscriber2, OnKeyUp).Times(1);
    }

    sut->SubscribeToInputEvents(subscriber1);
    sut->SubscribeToInputEvents(subscriber2);

    sut->OnKeyDown(KeyCodes::ARROW_DOWN);
    sut->OnKeyUp(KeyCodes::ARROW_DOWN);
}