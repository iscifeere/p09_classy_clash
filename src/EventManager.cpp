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
    for(auto& sub : m_EventSubscribers) sub = {};
    // for(auto& subList : m_EventSubscribers) subList = {};

    std::cout << "[EVENTS] Shutdown event manager instance" << std::endl;
}

Subscriber EventManager::CreateSubscriber(const std::string& ownerName, void(*logic)(EventID))
{
    return {&ownerName, logic};
}

void EventManager::SubscribeToEvent(EventID event, const Subscriber& sub)
{
    size_t eventIndex{static_cast<size_t>(event)};
    m_EventSubscribers[eventIndex] = sub;
    // m_EventSubscribers[eventIndex].push_back(sub);
    std::cout << "[EVENTS] " << *sub.OwnerName << " subscribed to " << m_EventNames[eventIndex] << " event successfully" << std::endl;
}

void EventManager::DesubscribeFromEvent(EventID event, const Subscriber& sub)   // TODO properly search for subscriber
{
    size_t eventIndex{static_cast<size_t>(event)};
    m_EventSubscribers[eventIndex] = {};    // reset to default values
    std::cout << "[EVENTS] " << *sub.OwnerName << " desubscribed from " << m_EventNames[eventIndex] << " event successfully" << std::endl;
}

void EventManager::DispatchEvent(EventID event) const
{
    size_t eventIndex{static_cast<size_t>(event)};
    
    std::cout << "[EVENTS] Event dispatched: " << m_EventNames[eventIndex] << ". Notifying subscribers: " << *m_EventSubscribers[eventIndex].OwnerName << std::endl;
    // std::cout << "[EVENTS] Event dispatched: " << m_EventNames[eventIndex] << ". Notifying subscribers: ";
    // for(const auto& sub : m_EventSubscribers[eventIndex]) std::cout << *sub.OwnerName << " ";
    // std::cout << "." << std::endl;

    m_EventSubscribers[eventIndex].Logic(event);
    // for(const auto& sub : m_EventSubscribers[eventIndex]) sub.Logic(event);
}

std::string EventManager::GetEventName(EventID event) const
{
    return m_EventNames[static_cast<size_t>(event)];
}

void EventManager::SetEventName(EventID event, const std::string& eventName)
{
    m_EventNames[static_cast<size_t>(event)] = eventName;
}