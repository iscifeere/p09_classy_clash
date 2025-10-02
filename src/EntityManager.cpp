#include "EntityManager.h"
#include <iostream>
#include <algorithm>
#include <array>

Item* EntityMng::itemArr[ITEM_ARR_SIZE]{nullptr};
Enemy* EntityMng::enemyArr[ENEMY_ARR_SIZE]{nullptr};
GenEntity* EntityMng::proyectileArr[PROYECTILE_ARR_SIZE]{nullptr};
GenEntity* EntityMng::ammo{nullptr};
Prop* EntityMng::propArr[PROP_ARR_SIZE]{nullptr};
Entity* EntityMng::EntityArr[ENTITY_ARR_SIZE]{nullptr};

void EntityMng::spawnProyectile(GenEntity*& proyectile, Vector2 pos, Vector2 direction, Character* playerPtr){
    if(proyectile != nullptr){
        proyectile->spawnReset(pos, direction);
    } else{
        proyectile = new GenEntity(pos, direction, playerPtr);
    }
}
void EntityMng::spawnProyectileInPool(Vector2 pos, Vector2 direction, Character* playerPtr){
    for(int i{0} ; i < PROYECTILE_ARR_SIZE ; i++){
        if(proyectileArr[i] == nullptr){
            proyectileArr[i] = new GenEntity(pos, direction, playerPtr);
            break;
        } else if(!proyectileArr[i]->getAlive()){
            proyectileArr[i]->spawnReset(pos, direction);
            break;
        }
        if(i >= PROYECTILE_ARR_SIZE-1) std::cout << "[Proyectile array full!]" << std::endl;
    }
}

void EntityMng::spawnAmmo(Vector2 pos, Vector2 direction, Character* playerPtr){
    if(ammo != nullptr){
        ammo->spawnReset(pos, direction);
    } else{
        ammo = new GenEntity(pos, direction, playerPtr);
    }
}

void EntityMng::tickProyectile(GenEntity*& proyectile, float deltaTime){
    if(proyectile != nullptr) proyectile->tick(deltaTime);
}
void EntityMng::tickProyectiles(float deltaTime){
    for(int i{0} ; i < PROYECTILE_ARR_SIZE ; i++){
        if(proyectileArr[i] != nullptr){
            if(proyectileArr[i]->getAlive()) proyectileArr[i]->tick(deltaTime);
        }
    }
}

void EntityMng::renderProyectiles(){
    for( auto &proyectile : proyectileArr ){
        if(proyectile != nullptr){
            if(proyectile->getAlive()) proyectile->render();
        }
    }
}

void EntityMng::showProyectilesDebugData(){
    for(int i{0} ; i < PROYECTILE_ARR_SIZE ; i++){
        if(proyectileArr[i] != nullptr){
            proyectileArr[i]->showDebugData();
        }
    }
}

void EntityMng::tickAmmo(float deltaTime){
    if(ammo != nullptr) ammo->tick(deltaTime);
}

void EntityMng::spawnItem(Vector2 pos, Character* playerPtr){
    for(int i{0} ; i < ITEM_ARR_SIZE ; i++){
        if( itemArr[i] == nullptr ){
            itemArr[i] = new Item(pos, playerPtr);
            std::cout << "[Item spawned!]" << std::endl;
            break;
        }
        if(i >= ITEM_ARR_SIZE-1) std::cout << "[Item array full!]" << std::endl;
    }
}
void EntityMng::spawnItem(Vector2 pos, Character* playerPtr, const itemData* item_data){
    for(int i{0} ; i < ITEM_ARR_SIZE ; i++){
        if( itemArr[i] == nullptr ){
            itemArr[i] = new Item(pos, playerPtr, item_data);
            std::cout << "[Item spawned!]" << std::endl;
            break;
        }
        if(i >= ITEM_ARR_SIZE-1) std::cout << "[Item array full!]" << std::endl;
    }
}

void EntityMng::killItem(){
    for(int i{0} ; i < ITEM_ARR_SIZE ; i++){
        if( itemArr[i] != nullptr ){
            delete itemArr[i];
            itemArr[i] = nullptr;
            std::cout << "[Item deleted!]" << std::endl;
            break;
        }
        if(i >= ITEM_ARR_SIZE-1) std::cout << "[Item array empty!]" << std::endl;
    }
}

void EntityMng::tickItems(float deltaTime){
    // Do item array tick every frame
    for(int i{0} ; i < ITEM_ARR_SIZE ; i++){
        if( itemArr[i] != nullptr ){
            if( !(itemArr[i]->tick(deltaTime)) ){
                delete itemArr[i];
                itemArr[i] = nullptr;
                std::cout << "[Item deleted!]" << std::endl;
            }
        }
    }
}

void EntityMng::renderItems(){
    for( auto &item : itemArr ){
        if( item != nullptr ){
            if( item->getAlive() ) item->render();
        }
    }
}

void EntityMng::showItemsDebugData(){
    for(int i{0} ; i < ITEM_ARR_SIZE ; i++){
        if(itemArr[i] != nullptr){
            itemArr[i]->showDebugData();
        }
    }
}

void EntityMng::spawnEnemy(Vector2 pos, Character* playerPtr){
    for(int i{0} ; i < ENEMY_ARR_SIZE ; i++){
        if( enemyArr[i] == nullptr ){
            enemyArr[i] = new Enemy(pos);
            enemyArr[i]->setTarget(playerPtr);
            std::cout << "[Enemy spawned!]" << std::endl;
            break;
        }
        if(i >= ENEMY_ARR_SIZE-1) std::cout << "[Enemy array full!]" << std::endl;
    }
}
void EntityMng::spawnEnemy(Vector2 pos, Character* playerPtr, const enemyData* enemy_data){
    for(int i{0} ; i < ENEMY_ARR_SIZE ; i++){
        if( enemyArr[i] == nullptr ){
            enemyArr[i] = new Enemy(pos, enemy_data);
            enemyArr[i]->setTarget(playerPtr);
            std::cout << "[Enemy spawned!]" << std::endl;
            break;
        }
        if(i >= ENEMY_ARR_SIZE-1) std::cout << "[Enemy array full!]" << std::endl;
    }
}

void EntityMng::killEnemy(){
    for(int i{0} ; i < ENEMY_ARR_SIZE ; i++){
        if( enemyArr[i] != nullptr ){
            enemyArr[i]->deathSequence();
            delete enemyArr[i];
            enemyArr[i] = nullptr;
            std::cout << "[Enemy deleted!]" << std::endl;
            break;
        }
        if( i >= ENEMY_ARR_SIZE-1 ) std::cout << "[Enemy array empty!]" << std::endl;
    }
}

void EntityMng::tickEnemies(float deltaTime){
    for(int i{0} ; i < ENEMY_ARR_SIZE ; i++){
        if( enemyArr[i] != nullptr ){
            if( !(enemyArr[i]->tick(deltaTime)) ){          // do tick, and if it's not alive, delete enemy
                delete enemyArr[i];
                enemyArr[i] = nullptr;
                std::cout << "[Enemy deleted!]" << std::endl;
            }
        }
    }
}

void EntityMng::renderEnemies(){
    for( auto &enemy : enemyArr ){
        if( enemy != nullptr ){
            if( enemy->getAlive() ) enemy->render();
        }
    }
}

void EntityMng::showEnemiesDebugData(){
    for(int i{0} ; i < ENEMY_ARR_SIZE ; i++){
        if(enemyArr[i] != nullptr){
            enemyArr[i]->showDebugData();
        }
    }
}

void EntityMng::spawnProp(Vector2 pos, const propData* prop_data, Character* playerPtr){
    for( auto &propPtr : propArr ){
        if(propPtr == nullptr){
            propPtr = new Prop(pos, prop_data, playerPtr);
            break;
        }}
}

void EntityMng::showPropsDebugData(){
    for( auto prop : propArr){
        if(prop != nullptr) prop->showDebugData();
    }
}

void EntityMng::checkPropCollisions(Character* playerPtr){
    for( auto propPtr : propArr ){
        if(propPtr != nullptr){
            if( CheckCollisionRecs(propPtr->getCollisionRec(), playerPtr->getCollisionRec()) ){
                playerPtr->undoMovementX();
                playerPtr->undoMovementY();
            }}}
}

void EntityMng::renderEntities(Character* playerPtr){
    std::array<Entity*, ENTITY_ARR_SIZE> renderQueue = {nullptr};

    size_t queueEnd{0};

    // add entities to render
    if(playerPtr != nullptr){
        renderQueue[queueEnd] = playerPtr;
        queueEnd++;
    }
    for( auto entity : propArr ){
        if(entity != nullptr){
            renderQueue[queueEnd] = entity;
            queueEnd++;
        }}
    for( auto entity : itemArr ){
        if(entity != nullptr){
            renderQueue[queueEnd] = entity;
            queueEnd++;
        }}
    for( auto entity : enemyArr ){
        if(entity != nullptr){
            renderQueue[queueEnd] = entity;
            queueEnd++;
        }}
    for( auto entity : proyectileArr ){
        if(entity != nullptr){
            if(entity->getAlive()){
                renderQueue[queueEnd] = entity;
                queueEnd++;
            }}}

    std::sort(renderQueue.begin(), renderQueue.begin() + queueEnd,
        [](Entity* a, Entity* b) {
            return a->getScreenPos().y + a->getHeight() < b->getScreenPos().y + b->getHeight();
        });

    for(size_t i{} ; i != queueEnd ; i++){
        renderQueue[i]->render();
    }

}

void EntityMng::clearEntityPools(){
    for( auto &entityPtr : itemArr ){
        if(entityPtr != nullptr){
            delete entityPtr;
            entityPtr = nullptr;
        }}
    for( auto &entityPtr : enemyArr ){
        if(entityPtr != nullptr){
            delete entityPtr;
            entityPtr = nullptr;
        }}
    for( auto &entityPtr : proyectileArr ){
        if(entityPtr != nullptr){
            delete entityPtr;
            entityPtr = nullptr;
        }}

    std::cout << "[Entity pools cleared]" << std::endl;
}