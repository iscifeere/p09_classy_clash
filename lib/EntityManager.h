#ifndef ENTITYMNG_H
#define ENTITYMNG_H

#include "raylib.h"
#include "Item.h"
#include "Character.h"
#include "Enemy.h"
#include "GenericEntity.h"
#include "Prop.h"
#include <array>
#include <variant>

using EntityVariant = std::variant<Character*, Enemy*, Item*, GenEntity*, Prop*>;

class EntityMng
{
public:
    EntityMng() = delete;       // no constructor = un-instantiable --> "static class"

    static Character player;
    static Character* getPlayerPtr(){ return &player; }
    
    static void initializeEntityManager(){
        player.init();   // necessary because textures and window dimensions were not assigned previous to this
    }

    static void spawnEnemy(Vector2 pos, const enemyData* enemy_data);
    static void killEnemy();
    static void tickEnemies(float deltaTime);
    static void showEnemiesDebugData();

    static void spawnItem(Vector2 pos, const itemData* item_data);
    static void killItem();
    static void tickItems(float deltaTime);
    static void showItemsDebugData();

    static void spawnProyectile(Vector2 pos, Vector2 direction, bool isEnemy);
    static void tickProyectiles(float deltaTime);
    static void checkProyectileCollisions();
    static void showProyectilesDebugData();

    static void spawnProp(Vector2 pos, const propData* prop_data);
    static void showPropsDebugData();
    static void checkPropCollisions();

    static void tickEntities(float deltaTime);
    static void checkEntityCollisions();
    static void showEntitiesDebugData();
    static void renderEntities();
    static void clearEntityPools();
    static void logEntityArrayStatus();

    static void showPlayerScore();
    static void spawnRandomEnemies();

private:
    static const int ITEM_ARR_SIZE{10};
    static const int ENEMY_ARR_SIZE{15};
    static const int PROYECTILE_ARR_SIZE{ENEMY_ARR_SIZE};
    static const int PROP_ARR_SIZE{5};
    static const int ENTITY_ARR_SIZE{ITEM_ARR_SIZE+ENEMY_ARR_SIZE+PROYECTILE_ARR_SIZE+PROP_ARR_SIZE};

    // static entity object pools
    static std::array<Enemy, ENEMY_ARR_SIZE> enemyPool;
    static std::array<Item, ITEM_ARR_SIZE> itemPool;
    static std::array<GenEntity, PROYECTILE_ARR_SIZE> proyectilePool;
    static std::array<Prop, PROP_ARR_SIZE> propPool;

    static std::array<EntityVariant, ENTITY_ARR_SIZE> activeEntities;
    static size_t i_EntitiesEnd;
    static size_t i_EnemiesStart;
    static size_t i_EnemiesEnd;
    static size_t i_ProyectilesStart;
    static size_t i_ProyectilesEnd;
};

#endif