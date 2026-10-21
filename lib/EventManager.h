#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <string>
#include <array>
#include <vector>
#include <unordered_map>

// using EventID = const char*;
enum class EventID
{
    HeartCollected,
    CoinCollected,
    GemCollected,
    EnemyKilled,
    Count
};

struct Subscriber
{
    const std::string* OwnerName = nullptr;
    void(*Logic)(EventID) = nullptr;
};

class EventManager
{
public:
    EventManager() = default;

    void Init();
    void Shutdown();

    static Subscriber CreateSubscriber(const std::string& ownerName, void(*logic)(EventID));
    void SubscribeToEvent(EventID event, const Subscriber& sub);
    void DesubscribeFromEvent(EventID event, const Subscriber& sub);
    void DispatchEvent(EventID event) const;

    std::string GetEventName(EventID event) const;

private:
    void SetEventName(EventID event, const std::string& eventName);

    // std::unordered_map<EventID, Subscriber> m_EventSubscribers{};
    // std::unordered_map<EventID, std::array<Subscriber, 3> > m_EventSubscribers{};

    std::array<std::string, static_cast<size_t>(EventID::Count)> m_EventNames{};
    std::array<Subscriber, static_cast<size_t>(EventID::Count)> m_EventSubscribers{};
    // std::array<std::vector<Subscriber>, static_cast<size_t>(EventID::Count)> m_EventSubscribers{};
    
};

#endif