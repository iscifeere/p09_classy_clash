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
    virtual bool tick(float deltaTime) override;
    void setTarget(Character* ptr){ target = ptr; }
    virtual Vector2 getScreenPos() override;
    virtual void takeDamage(float damage) override;
    virtual void deathSequence() override;
    float& getRadiusEtc(int choice);
    float attackTimer{};
    virtual void showDebugData() override;
    void drawHealthBar();
    GenEntity* proyectile{nullptr};
    // const enemyData* data{nullptr};
    float fleeTimer{};
    bool flee{false};
    bool chase{false};
    bool neutral{false};
protected:
    Character* target{};
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