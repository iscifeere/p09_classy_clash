#include <iostream>
#include "raylib.h"
#include "EventManager.h"

void EventManager::Init()
{
    // set default name for all events
    for(int i{0} ; i < static_cast<int>(EventID::Count) ; ++i)
    {
        m_EventNames[i] = TextFormat("UnnamedEvent_%i", i);
    }

    SetEventName(EventID::HeartCollected, "HeartCollected");
    SetEventName(EventID::CoinCollected, "CoinCollected");
    SetEventName(EventID::GemCollected, "GemCollected");
    SetEventName(EventID::EnemyKilled, "EnemyKilled");

    std::cout << "[EVENTS] Initialized event manager instance" << std::endl;
}

void EventManager::Shutdown()
{
    for(auto& name : m_EventNames) name = {};
    for(auto& subList : m_EventSubscribers) subList = {};

    std::cout << "[EVENTS] Shutdown event manager instance" << std::endl;
}

Subscriber EventManager::CreateSubscriber(const std::string& ownerName, void(*eventLogic)(EventID))
{
    return {&ownerName, eventLogic};
}

void EventManager::SubscribeToEvent(EventID event, const Subscriber& newSub)
{
    size_t eventIndex{static_cast<size_t>(event)};
    for(auto& sub : m_EventSubscribers[eventIndex])
    {
        if(sub.OwnerName == nullptr)
        {
            sub = newSub;
            std::cout << "[EVENTS] " << *newSub.OwnerName << " subscribed to " << m_EventNames[eventIndex] << " event successfully" << std::endl;
            return;
        }
        if(*sub.OwnerName == *newSub.OwnerName)
        {
            std::cout << "[EVENTS] Subscribe error: " << *newSub.OwnerName << " is already subscribed to " << m_EventNames[eventIndex] << " event!" << std::endl;
            return;
        }
    }
    m_EventSubscribers[eventIndex].push_back(newSub);
    std::cout << "[EVENTS] " << *newSub.OwnerName << " subscribed to " << m_EventNames[eventIndex] << " event successfully" << std::endl;
}

void EventManager::DesubscribeFromEvent(EventID event, const std::string& subOwnerName)
{
    size_t eventIndex{static_cast<size_t>(event)};
    for(auto& sub : m_EventSubscribers[eventIndex])
    {
        if(sub.OwnerName == nullptr) break;
        if(*sub.OwnerName == subOwnerName)
        {
            sub = {};
            std::cout << "[EVENTS] " << subOwnerName << " desubscribed from " << m_EventNames[eventIndex] << " event successfully" << std::endl;
            return;
        }
    }
    std::cout << "[EVENTS] Desubscribe error: Could not find subscriber from " << subOwnerName << " to " << m_EventNames[eventIndex] << " event!" << std::endl;
}

void EventManager::DispatchEvent(EventID event) const
{
    size_t eventIndex{static_cast<size_t>(event)};

    std::cout << "[EVENTS] Event dispatched: " << m_EventNames[eventIndex] << ". Notifying subscribers: ";
    for(const auto& sub : m_EventSubscribers[eventIndex])
    {
        if(sub.OwnerName == nullptr) break;
        std::cout << *sub.OwnerName << " ";
    }
    std::cout << "." << std::endl;

    for(const auto& sub : m_EventSubscribers[eventIndex])
    {
        if(sub.OwnerName == nullptr) break;
        sub.EventLogic(event);
    }
}

std::string EventManager::GetEventName(EventID event) const
{
    return m_EventNames[static_cast<size_t>(event)];
}

void EventManager::SetEventName(EventID event, const std::string& eventName)
{
    m_EventNames[static_cast<size_t>(event)] = eventName;
}