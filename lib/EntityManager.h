#ifndef ENTITYMNG_H
#define ENTITYMNG_H

#include "raylib.h"
#include "Item.h"
#include "Character.h"
#include "Enemy.h"
#include "GenericEntity.h"

class EntityMng
{
public:
    EntityMng() = delete;       // no constructor = un-instantiable --> "static class"

    static void spawnItem(Vector2 pos, Character* playerPtr);
    static void spawnItem(Vector2 pos, Character* playerPtr, const itemData* item_data);
    static void killItem();
    static void tickItems(float deltaTime);
    
    static void spawnEnemy(Vector2 pos, Character* playerPtr);
    static void spawnEnemy(Vector2 pos, Character* playerPtr, const enemyData* enemy_data);
    static void killEnemy();
    static void tickEnemies(float deltaTime);

    // static void spawnGenEnt(Vector2 pos, Vector2 direction, Character* playerPtr);
    static void spawnAmmo(Vector2 pos, Vector2 direction, Character* playerPtr);
    // static void killGenEnt();
    // static void tickGenEnts(float deltaTime);
    static void tickAmmo(float deltaTime);
    // static void killAmmo();
    static void spawnProyectile(GenEntity*& proyectile, Vector2 pos, Vector2 direction, Character* playerPtr);
    static void tickProyectile(GenEntity*& proyectile, float deltaTime);

private:
    static const int ITEM_ARR_SIZE{5};
    static Item* itemArr[ITEM_ARR_SIZE];

    static const int ENEMY_ARR_SIZE{7};
    static Enemy* enemyArr[ENEMY_ARR_SIZE];

    // static const int GEN_ENT_ARR_SIZE{3};
    // static GenEntity* genEntArr[GEN_ENT_ARR_SIZE];
    static GenEntity* ammo;
};

#endif