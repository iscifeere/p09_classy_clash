#ifndef ENTITYMNG_H
#define ENTITYMNG_H

#include "raylib.h"
#include "Item.h"
#include "Character.h"
#include "Enemy.h"
#include "GenericEntity.h"
#include "Entity.h"
#include "Prop.h"
#include <array>

class EntityMng
{
public:
    EntityMng() = delete;       // no constructor = un-instantiable --> "static class"

    static void spawnItem(Vector2 pos, Character* playerPtr, const itemData* item_data);
    static void killItem();
    static void tickItems(float deltaTime);
    static void showItemsDebugData();
    
    static void spawnEnemy(Vector2 pos, Character* playerPtr, const enemyData* enemy_data);
    static void killEnemy();
    static void tickEnemies(float deltaTime);
    static void showEnemiesDebugData();

    static void spawnProyectile(Vector2 pos, Vector2 direction, Character* playerPtr);
    static void tickProyectiles(float deltaTime);
    static void showProyectilesDebugData();

    static void spawnProp(Vector2 pos, const propData* prop_data, Character* playerPtr);
    static void showPropsDebugData();
    static void checkPropCollisions(Character* playerPtr);

    static void tickEntities(float deltaTime, Character* playerPtr);
    static void renderEntities(Character* playerPtr);
    static void clearEntityPools();

private:
    static const int ITEM_ARR_SIZE{10};
    static const int ENEMY_ARR_SIZE{10};
    static const int PROYECTILE_ARR_SIZE{ENEMY_ARR_SIZE};
    static const int PROP_ARR_SIZE{5};
    static const int ENTITY_ARR_SIZE{ITEM_ARR_SIZE+ENEMY_ARR_SIZE+PROYECTILE_ARR_SIZE+PROP_ARR_SIZE};

    // static entity object pools
    static std::array<Item, ITEM_ARR_SIZE> itemPool;
    static std::array<Enemy, ENEMY_ARR_SIZE> enemyPool;
    static std::array<GenEntity, PROYECTILE_ARR_SIZE> proyectilePool;
    static std::array<Prop, PROP_ARR_SIZE> propPool;

    static std::array<Entity*, ENTITY_ARR_SIZE> activeEntities;
    static size_t i_EntitiesEnd;
};

#endif