#ifndef ENEMYDATA_H
#define ENEMYDATA_H

#include "Enemy.h"
#include "EnemyBehaviourManager.h"

// ========= ENEMY DATA STRUCTS ============================

enum class EnemyType
{
    ENEMY_SLIME,
    ENEMY_SLIME_BLUE,
    ENEMY_MAD_KNIGHT,
    ENEMY_RED,
    ENEMY_GOBLIN
};

struct enemyData
{
    Texture2D* texture_idle{&Tex::texture_goblin_idle};
    Texture2D* texture_run{&Tex::texture_goblin_run};
    int maxFramesIdle{6};
    int maxFramesRun{6};
    int frameRows{1};
    bool ignoreFrameRows{false};

    float speed{};
    float health{};
    float damage{};
    float chase_radius{};
    EnemyType enemyType{EnemyType::ENEMY_SLIME};
    bool isNeutral{false};  // unused

    // Location and size of collisionBox & hurtBox.
    // x and y are propotional displacement from sprite screen position,
    // width and height are scaling proportional to texture.
    Rectangle collisionBox{
        .x = 0.f,
        .y = 0.f,
        .width = 1.f,
        .height = 1.f,
    };
    Rectangle hurtBox{
        .x = 0.f,
        .y = 0.f,
        .width = 1.f,
        .height = 1.f,
    };

    const itemData* item_drop{&HEART_ITEMDATA};

    // behaviour functions
    void (*idleLogic)(Enemy& thisEnemy, Character* player, const float& deltaTime) = EnemyBehaviourManager::idleWandering;
    void (*transitionLogic)(Enemy& thisEnemy, Character* player, const float& deltaTime) = EnemyBehaviourManager::playerSpottedSequence;
    void (*actionLogic)(Enemy& thisEnemy, Character* player, const float& deltaTime) = EnemyBehaviourManager::fleeTarget;
};

const enemyData DEFAULT_ENEMYDATA
{

};
const enemyData SLIME_ENEMYDATA
{
    .texture_idle = &Tex::texture_slime_idle,
    .texture_run = &Tex::texture_slime_run,
    .maxFramesIdle = 6,
    .maxFramesRun = 6,
    .frameRows = 1,
    .speed = 4.f,
    .health = 40.f,
    .damage = 5.f,
    .chase_radius = 300.f,
    .enemyType = EnemyType::ENEMY_SLIME,
    .item_drop = &HEART_2_ITEMDATA,
    .idleLogic = EnemyBehaviourManager::idleWandering,
    .transitionLogic = EnemyBehaviourManager::playerSpottedSequence,
    .actionLogic = EnemyBehaviourManager::fleeTarget
};
const enemyData SLIME_BLUE_ENEMYDATA
{
    .texture_idle = &Tex::texture_slime_blue_idle,
    .texture_run = &Tex::texture_slime_blue_run,
    .maxFramesIdle = 6,
    .maxFramesRun = 6,
    .frameRows = 1,
    .speed = 4.f,
    .health = 80.f,
    .damage = 10.f,
    .chase_radius = 400.f,
    .enemyType = EnemyType::ENEMY_SLIME_BLUE,
    .item_drop = &HEART_3_ITEMDATA,
    .idleLogic = EnemyBehaviourManager::idleWandering,
    .transitionLogic = EnemyBehaviourManager::playerSpottedSequence,
    .actionLogic = EnemyBehaviourManager::chaseTarget
};
const enemyData MADKNIGHT_ENEMYDATA
{
    .texture_idle = &Tex::texture_madknight_idle,
    .texture_run = &Tex::texture_madknight_run,
    .maxFramesIdle = 6,
    .maxFramesRun = 6,
    .frameRows = 1,
    .speed = 9.f,
    .health = 120.f,
    .damage = 10.f,
    .chase_radius = 400.f,
    .enemyType = EnemyType::ENEMY_MAD_KNIGHT,
    .isNeutral = true,
    .collisionBox = {
        .x = 0.2f,
        .y = 0.75f,
        .width = 0.6f,
        .height = 0.25f
    },
    .item_drop = &COIN_ITEMDATA,
    .idleLogic = EnemyBehaviourManager::idleWandering,
    .transitionLogic = EnemyBehaviourManager::playerSpottedSequence,
    .actionLogic = EnemyBehaviourManager::shootTarget
};
const enemyData RED_ENEMYDATA
{
    .texture_idle = &Tex::texture_red_idle,
    .texture_run = &Tex::texture_red_run,
    .maxFramesIdle = 4,
    .maxFramesRun = 6,
    .frameRows = 1,
    .speed = 9.f,
    .health = 120.f,
    .damage = 10.f,
    .chase_radius = 400.f,
    .enemyType = EnemyType::ENEMY_RED,
    .collisionBox = {
        .x = 0.25f,
        .y = 0.25f,
        .width = 0.5f,
        .height = 0.5f
    },
    .item_drop = &GEM_ITEMDATA,
    .idleLogic = EnemyBehaviourManager::idleWanderingAlert,
    .transitionLogic = EnemyBehaviourManager::playerSpottedSequence,
    .actionLogic = EnemyBehaviourManager::chaseTarget
};
const enemyData GOBLIN_ENEMYDATA
{
    .texture_idle = &Tex::texture_goblin_idle,
    .texture_run = &Tex::texture_goblin_run,
    .maxFramesIdle = 6,
    .maxFramesRun = 6,
    .frameRows = 1,
    .speed = 9.f,
    .health = 80.f,
    .damage = 10.f,
    .chase_radius = 400.f,
    .enemyType = EnemyType::ENEMY_GOBLIN,
    .isNeutral = true,
    .collisionBox = {
        .x = 0.2f,
        .y = 0.75f,
        .width = 0.6f,
        .height = 0.25f
    },
    .item_drop = &COIN_ITEMDATA,
    .idleLogic = EnemyBehaviourManager::idleWanderingAlert,
    .transitionLogic = EnemyBehaviourManager::playerSpottedSequence,
    .actionLogic = EnemyBehaviourManager::chaseTarget
};
const enemyData* ENEMYDATA_ARR[]
{
    &SLIME_ENEMYDATA,
    &SLIME_BLUE_ENEMYDATA,
    &MADKNIGHT_ENEMYDATA,
    &RED_ENEMYDATA,
    &GOBLIN_ENEMYDATA
};

#endif