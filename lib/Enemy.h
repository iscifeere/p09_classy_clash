#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "raymath.h"
#include "BaseCharacter.h"
#include "Character.h"
#include "Item.h"
#include "GenericEntity.h"

struct enemyData;

//============================

class Enemy : public BaseCharacter
{
public:
    Enemy();            // default constructor
    Enemy(Vector2 pos);
    Enemy(Vector2 pos, const enemyData* enemy_data);
    Enemy(Vector2 pos, Texture2D* idle_texture, Texture2D* run_texture);
    // virtual void tick(float deltaTime) override;
    virtual bool tick(float deltaTime) override;
    void setTarget(Character* ptr){ target = ptr; }
    virtual Vector2 getScreenPos() override;
    virtual void takeDamage(float damage) override;
    virtual void deathSequence() override;
    float& getRadiusEtc(int choice);
    float attackTimer{};
    GenEntity* proyectile{nullptr};
    float fleeTimer{};
    bool flee{false};
    bool chase{false};
    bool neutral{true};
protected:
    Character* target{};
    float damagePerSec{10.f};
    float radius{50.f};
    float chaseRadius{400.f};
    float chaseTime{};
    const enemyData* data{nullptr};
    const itemData* itemDrop{&HEART_ITEMDATA};

    void(*action)(Enemy* this_enemy, Character* player, const float& deltaTime) = [](Enemy* enemy, Character* player, const float& deltaTime){
        // get to target
        Vector2& velocity = enemy->getVelocity();
        float& in_radius = enemy->getRadiusEtc(0);
        float& out_radius = enemy->getRadiusEtc(1);
        float& chaseTime = enemy->getRadiusEtc(2);

        velocity = Vector2Subtract(player->getScreenPos(), enemy->getScreenPos());
        // if too close / too far -> don't chase
        if(Vector2Length(velocity) < in_radius || Vector2Length(velocity) > out_radius ) velocity = {};

        if(Vector2Length(velocity) > 0.f) chaseTime += deltaTime;
        else chaseTime = 0.f;

        if(chaseTime >= 5.f) velocity = {}; // at 5 sec chaseTime stop chasing
    };
};

//============================

// struct enemyData{
//     Texture2D* texture_idle;
//     Texture2D* texture_run;
//     int maxFrames;
//     int frameRows;
//     float speed;
//     float health;
//     float damage;
//     float chase_radius;
//     const itemData* item_drop;
//     void(*behave)(Enemy* enemy, Character* player, const float& deltaTime);
// };

inline void chaseTarget(Enemy* enemy, Character* target, const float& deltaTime){
    Vector2& velocity = enemy->getVelocity();
    float& in_radius = enemy->getRadiusEtc(0);
    float& out_radius = enemy->getRadiusEtc(1);
    float& chaseTime = enemy->getRadiusEtc(2);

    // get to target
    velocity = Vector2Subtract(target->getScreenPos(), enemy->getScreenPos());
    // if too close / too far -> don't chase
    if(Vector2Length(velocity) < in_radius || Vector2Length(velocity) > out_radius ) velocity = {};

    if(Vector2Length(velocity) > 0.f) chaseTime += deltaTime;
    else chaseTime = 0.f;

    if(chaseTime >= 5.f) velocity = {}; // at 5 sec chaseTime stop chasing
}

inline void fleeTarget(Enemy* enemy, Character* target, const float& deltaTime){
    Vector2& velocity = enemy->getVelocity();
    float& in_radius = enemy->getRadiusEtc(0);
    float& out_radius = enemy->getRadiusEtc(1);
    float& chaseTime = enemy->getRadiusEtc(2);
    
    // get inverted target direction
    velocity = Vector2Subtract(enemy->getScreenPos(), target->getScreenPos());
    // if too close / too far -> don't flee
    if(Vector2Length(velocity) < in_radius || Vector2Length(velocity) > out_radius ) velocity = {};

    if(Vector2Length(velocity) > 0.f) chaseTime += deltaTime;
    else chaseTime = 0.f;

    if(chaseTime >= 5.f) velocity = {}; // at 5 sec chaseTime stop chasing
}

// inline void shootTarget(Enemy* enemy, Character* target, const float& deltaTime){
//     Vector2& velocity = enemy->getVelocity();

//     // get target direction
//     velocity = Vector2Subtract(target->getScreenPos(), enemy->getScreenPos());
//     // if in radius -> do nothing
//     if(Vector2Length(velocity) <= 400.f && Vector2Length(velocity) >= 330.f) {velocity = {}; enemy->setDrawColor(BLUE);}
//     // if too close -> get away
//     if(Vector2Length(velocity) < 330.f) velocity = Vector2Scale(velocity, -1.f);
// }
void shootTarget(Enemy* enemy, Character* target, const float& deltaTime);

// inline const enemyData SLIME_ENEMYDATA{
//     &Tex::texture_slime_idle,
//     &Tex::texture_slime_run,
//     6,
//     1,
//     4.f,
//     40.f,
//     5.f,
//     300.f,
//     &HEART_ITEMDATA,
//     fleeTarget
// };
extern const enemyData SLIME_ENEMYDATA;

// inline const enemyData MADKNIGHT_ENEMYDATA{
//     &Tex::texture_knight_idle,
//     &Tex::texture_knight_run,
//     6,
//     1,
//     9.f,
//     120.f,
//     10.f,
//     400.f,
//     &COIN_ITEMDATA,
//     shootTarget
// };
extern const enemyData MADKNIGHT_ENEMYDATA;

// inline const enemyData RED_ENEMYDATA{
//     &Tex::texture_red_idle,
//     &Tex::texture_red_run,
//     4,
//     2,
//     9.f,
//     120.f,
//     10.f,
//     400.f,
//     &GEM_ITEMDATA,
//     chaseTarget
// };
extern const enemyData RED_ENEMYDATA;

// inline const enemyData* ENEMYDATA_ARR[]{
//     &SLIME_ENEMYDATA,
//     &MADKNIGHT_ENEMYDATA,
//     &RED_ENEMYDATA
// };
extern const enemyData* ENEMYDATA_ARR[];



#endif