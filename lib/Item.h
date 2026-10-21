#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"
#include "EventManager.h"
#include "Character.h"
#include "Tex.h"
#include "AudioManager.h"
#include "Entity.h"

struct itemData{
    Texture2D* texture;
    int maxFrames;
    float updateTime = 1.f/12.f;
    void(*effect)(Character*, const EventManager&);      // effect on player collision
};

//============================

class Item : public Entity
{
public:
    Item();    // default constructor
    Item(Vector2 pos);
    Item(Vector2 pos, const itemData* item_data);

    void init();    // initialize necessary variables, called in constructor
    void spawnReset(Vector2 pos, const itemData* item_data);
    
    bool tick(float deltaTime);
    Vector2 getWorldPos() { return worldPos; }
    Vector2 getScreenPos() override;
    bool getAlive() { return alive; }
    void setAlive( bool isAlive ) { alive = isAlive; }
    Rectangle getCollisionRec();
    void showDebugData();
    void render();

protected:
    Vector2 worldPos{};
    Character* player{nullptr};
    Texture2D* texture{&Tex::texture_heart};
    int frame{};
    int maxFrames{1};
    // float frameWidth{};       // width of one animation frame
    // float frameHeight{};      // height of one animation frame
    float runningTime{};
    float updateTime{1.f/12.f};     // speed of animation
    // float scale{4.f};
    float moveTimer{0.f};

    const itemData* data{nullptr};

    // effect on player collision
    void(*item_effect)(Character*, const EventManager&) = [](Character* player, const EventManager& events){
        player->addHealth(20.f);
    };

private:
    bool alive{true};
};

//============================

inline const itemData HEART_ITEMDATA{
    &Tex::texture_heart,
    1,
    (1.f/12.f),
    [](Character* player, const EventManager& events){
        player->addHealth(20.f);
        events.DispatchEvent(EventID::HeartCollected);
    }
};
inline const itemData HEART_2_ITEMDATA{
    &Tex::texture_heart_2,
    6,
    (1.f/12.f),
    [](Character* player, const EventManager& events){
        player->addHealth(20.f);
        events.DispatchEvent(EventID::HeartCollected);
    }
};
inline const itemData HEART_3_ITEMDATA{
    &Tex::texture_heart_2,
    6,
    (1.f/24.f),
    [](Character* player, const EventManager& events){
        player->addHealth(40.f);
        events.DispatchEvent(EventID::HeartCollected);
    }
};
inline const itemData COIN_ITEMDATA{
    &Tex::texture_coin,
    5,
    (1.f/12.f),
    [](Character* player, const EventManager& events){
        player->addMoney(10);
        events.DispatchEvent(EventID::CoinCollected);
    }
};
inline const itemData GEM_ITEMDATA{
    &Tex::texture_gem_green,
    4,
    (1.f/6.f),
    [](Character* player, const EventManager& events){
        player->addMoney(50);
        events.DispatchEvent(EventID::GemCollected);
    }
};

inline const itemData* ITEMDATA_ARR[]{
    &HEART_ITEMDATA,
    &HEART_2_ITEMDATA,
    &HEART_3_ITEMDATA,
    &COIN_ITEMDATA,
    &GEM_ITEMDATA
};

#endif