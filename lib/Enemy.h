#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "raymath.h"
#include "BaseCharacter.h"
#include "Character.h"
#include "Item.h"
#include "GenericEntity.h"

struct enemyData;
enum class EnemyState
{
    IDLE,
    PLAYER_SPOTTED,
    ACTION
};

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
    int getEnemyType();
    float getDamage();
    virtual void showDebugData() override;
    void drawHealthBar();
    void render() override;

    void playerSpottedSequence(float deltaTime);
    EnemyState getEnemyState(){ return state; }
    void setEnemyState(EnemyState newState){ state = newState; }
    const Vector2& getRefWanderingPoint(){ return wanderingPoint; }
    void setWanderingPoint(Vector2 newWanderingPoint){ wanderingPoint = newWanderingPoint; }

    float attackTimer{};
    float fleeTimer{};
    float freeUseTimer1{};
    bool flee{false};
    bool chase{false};
    bool neutral{false};    // unused

    Enemy* nearestEnemy{nullptr};
    
protected:
    Character* target{nullptr};
    float damagePerSec{10.f};   // unused
    float radius{50.f};
    float chaseRadius{400.f};
    float chaseTime{};
    EnemyState state{EnemyState::IDLE};
    Vector2 wanderingPoint{};
    const enemyData* data{nullptr};
    const itemData* itemDrop{&HEART_ITEMDATA};
    
    void(*idleBehave)(Enemy* this_enemy, Character* player, const float& deltaTime) = [](Enemy* enemy, Character* player, const float& deltaTime){};
    void(*action)(Enemy* this_enemy, Character* player, const float& deltaTime) = [](Enemy* enemy, Character* player, const float& deltaTime){
        // default dummy function...
    };
};

//============================

extern const enemyData SLIME_ENEMYDATA;
extern const enemyData SLIME_BLUE_ENEMYDATA;
extern const enemyData MADKNIGHT_ENEMYDATA;
extern const enemyData RED_ENEMYDATA;
extern const enemyData GOBLIN_ENEMYDATA;

extern const enemyData* ENEMYDATA_ARR[];


#endif