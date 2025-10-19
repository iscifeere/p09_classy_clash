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

    void init();    // initialize necessary variables, called in constructor
    void spawnReset(Vector2 pos, const enemyData* enemy_data);

    bool tick(float deltaTime) override;
    void setTarget(Character* ptr){ target = ptr; }
    Vector2 getScreenPos() override;
    Rectangle getCollisionRec();
    Rectangle getHurtRec();
    virtual void takeDamage(float damage) override;
    virtual void deathSequence() override;
    float& getRadiusEtc(int choice);
    virtual void showDebugData() override;
    void drawHealthBar();
    void render() override;

    GenEntity* proyectile{nullptr};
    float attackTimer{};
    float fleeTimer{};
    bool flee{false};
    bool chase{false};
    bool neutral{false};
    
protected:
    Character* target{nullptr};
    float damagePerSec{10.f};
    float radius{50.f};
    float chaseRadius{400.f};
    float chaseTime{};
    const enemyData* data{nullptr};
    const itemData* itemDrop{&HEART_ITEMDATA};

    void(*action)(Enemy* this_enemy, Character* player, const float& deltaTime) = [](Enemy* enemy, Character* player, const float& deltaTime){
        // default dummy function...
    };
};

//============================

extern const enemyData SLIME_ENEMYDATA;
extern const enemyData MADKNIGHT_ENEMYDATA;
extern const enemyData RED_ENEMYDATA;

extern const enemyData* ENEMYDATA_ARR[];


#endif