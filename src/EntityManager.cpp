#include "EntityManager.h"
#include <iostream>

Item* EntityMng::itemArr[ITEM_ARR_SIZE]{nullptr};
Enemy* EntityMng::enemyArr[ENEMY_ARR_SIZE]{nullptr};
// GenEntity* EntityMng::genEntArr[GEN_ENT_ARR_SIZE]{nullptr};
GenEntity* EntityMng::ammo{nullptr};

void EntityMng::spawnProyectile(GenEntity*& proyectile, Vector2 pos, Vector2 direction, Character* playerPtr){
    if(proyectile != nullptr){
        proyectile->reset(pos, direction);
    } else{
        proyectile = new GenEntity(pos, direction, playerPtr);
    }
}
void EntityMng::spawnAmmo(Vector2 pos, Vector2 direction, Character* playerPtr){
    if(ammo != nullptr){
        ammo->reset(pos, direction);
    } else{
        ammo = new GenEntity(pos, direction, playerPtr);
    }
}

void EntityMng::tickProyectile(GenEntity*& proyectile, float deltaTime){
    if(proyectile != nullptr) proyectile->tick(deltaTime);
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