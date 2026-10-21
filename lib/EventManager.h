#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <string>
#include <array>
#include <vector>
#include <unordered_map>

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
    void(*EventLogic)(EventID) = nullptr;
};

class EventManager
{
public:
    EventManager() = default;

    void Init();
    void Shutdown();

    static Subscriber CreateSubscriber(const std::string& ownerName, void(*logic)(EventID));
    void SubscribeToEvent(EventID event, const Subscriber& newSub);
    void DesubscribeFromEvent(EventID event, const std::string& subOwnerName);
    void DispatchEvent(EventID event) const;

    std::string GetEventName(EventID event) const;

private:
    void SetEventName(EventID event, const std::string& eventName);

    std::array<std::string, static_cast<size_t>(EventID::Count)> m_EventNames{};
    std::array<std::vector<Subscriber>, static_cast<size_t>(EventID::Count)> m_EventSubscribers{};
    
};

#endif