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
    static void showItemsDebugData();
    
    static void spawnEnemy(Vector2 pos, Character* playerPtr);
    static void spawnEnemy(Vector2 pos, Character* playerPtr, const enemyData* enemy_data);
    static void killEnemy();
    static void tickEnemies(float deltaTime);
    static void showEnemiesDebugData();

    static void spawnAmmo(Vector2 pos, Vector2 direction, Character* playerPtr);
    static void tickAmmo(float deltaTime);
    static void spawnProyectile(GenEntity*& proyectile, Vector2 pos, Vector2 direction, Character* playerPtr);
    static void spawnProyectileInPool(Vector2 pos, Vector2 direction, Character* playerPtr);
    static void tickProyectile(GenEntity*& proyectile, float deltaTime);
    static void tickProyectiles(float deltaTime);
    static void showProyectilesDebugData();

private:
    static const int ITEM_ARR_SIZE{10};     // max number of items
    static Item* itemArr[ITEM_ARR_SIZE];

    static const int ENEMY_ARR_SIZE{10};    // max number of enemies
    static Enemy* enemyArr[ENEMY_ARR_SIZE];

    static const int PROYECTILE_ARR_SIZE{ENEMY_ARR_SIZE};   // max number of proyectiles
    static GenEntity* proyectileArr[PROYECTILE_ARR_SIZE];
    static GenEntity* ammo;
};

#endif