#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"
#include "Character.h"
#include "Tex.h"

struct itemData{
    Texture2D* texture;
    int maxFrames;
    float updateTime = 1.f/12.f;
    void(*effect)(Character*);      // effect on player collision
};

//============================

class Item
{
public:
    Item();    // default constructor
    Item( Vector2 pos, Character* player_ptr );
    Item( Vector2 pos, Character* player_ptr, const itemData* item_data );
    
    bool tick(float deltatime);
    Vector2 getWorldPos() { return worldPos; }
    Vector2 getScreenPos();
    bool getAlive() { return alive; }
    void setAlive( bool isAlive ) { alive = isAlive; }
    Rectangle getCollisionRec();
protected:
    Vector2 worldPos{};
    Character* player{nullptr};
    Texture2D* texture{&Tex::texture_heart};
    int frame{};
    int maxFrames{1};
    float frameWidth{};       // width of one animation frame
    float frameHeight{};      // height of one animation frame
    float runningTime{};
    float updateTime{1.f/12.f};     // speed of animation
    float scale{4.f};
    float moveTimer{0.f};

    const itemData* data{nullptr};

    // effect on player collision
    void(*item_effect)(Character*) = [](Character* player){
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
    [](Character* player){
        player->addHealth(20.f);
    }
};
inline const itemData COIN_ITEMDATA{
    &Tex::texture_coin,
    5,
    (1.f/12.f),
    [](Character* player){
        player->addMoney(10);
    }
};
inline const itemData GEM_ITEMDATA{
    &Tex::texture_gem_green,
    4,
    (1.f/6.f),
    [](Character* player){
        player->addMoney(50);
    }
};

inline const itemData* ITEMDATA_ARR[]{
    &HEART_ITEMDATA,
    &COIN_ITEMDATA,
    &GEM_ITEMDATA
};

#endif