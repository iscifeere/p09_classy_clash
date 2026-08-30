#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "raymath.h"
#include "BaseCharacter.h"
#include "Character.h"
#include "Item.h"
#include "GenericEntity.h"

class EnemyBehaviour;
struct enemyData;
enum class EnemyType;
enum class EnemyState
{
    IDLE,
    PLAYER_SPOTTED,
    ACTION
};

//===========================================================

class Enemy : public BaseCharacter
{
public:
    friend EnemyBehaviour;      // manages different behaviours and state's logic
    
    Enemy();

    void init();    // initialize necessary variables, called in constructor
    void spawnReset(Vector2 pos, const enemyData* enemy_data);
    bool tick(float deltaTime) override;
    void takeDamage(float damage) override;
    void deathSequence() override;
    void render() override;
    void showDebugData() override;
    void drawHealthBar();

    Vector2 getScreenPos() override;
    Rectangle getCollisionRec();
    Rectangle getHurtRec();
    float getDamage();
    EnemyType getEnemyType();
    EnemyState getEnemyState(){ return state; }

    void setTarget(Character* ptr){ target = ptr; }
    void setEnemyState(EnemyState newState);
    void setWanderingPoint(Vector2 newWanderingPoint){ wanderingPoint = newWanderingPoint; }
    
private:
    float chaseTime{};
    float attackTimer{};
    float fleeTimer{};
    float freeUseTimer1{};
    float radius{50.f};
    float chaseRadius{400.f};
    bool flee{false};
    bool chase{false};
    Vector2 wanderingPoint{};
    EnemyState state{EnemyState::IDLE};
    
    Character* target{nullptr};
    Enemy* nearestEnemy{nullptr};
    const enemyData* data{nullptr};
    const itemData* itemDrop{&HEART_ITEMDATA};
    
    void (*currentStateLogic)(Enemy* thisEnemy, Character* player, const float& deltaTime) = nullptr;
    void (*idleLogic)(Enemy* thisEnemy, Character* player, const float& deltaTime) = nullptr;
    void (*transitionLogic)(Enemy* thisEnemy, Character* player, const float& deltaTime) = nullptr;
    void (*actionLogic)(Enemy* thisEnemy, Character* player, const float& deltaTime) = nullptr;
};

//===========================================================

extern const enemyData SLIME_ENEMYDATA;
extern const enemyData SLIME_BLUE_ENEMYDATA;
extern const enemyData MADKNIGHT_ENEMYDATA;
extern const enemyData RED_ENEMYDATA;
extern const enemyData GOBLIN_ENEMYDATA;

extern const enemyData* ENEMYDATA_ARR[];


#endif