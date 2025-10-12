#include "EntityManager.h"
#include <iostream>
#include <algorithm>

#define DEBUG // for console logging when debugging

std::array<Item, EntityMng::ITEM_ARR_SIZE> EntityMng::itemPool{};
std::array<Enemy, EntityMng::ENEMY_ARR_SIZE> EntityMng::enemyPool{};
std::array<GenEntity, EntityMng::PROYECTILE_ARR_SIZE> EntityMng::proyectilePool{};
std::array<Prop, EntityMng::PROP_ARR_SIZE> EntityMng::propPool{};
std::array<Entity*, EntityMng::ENTITY_ARR_SIZE> EntityMng::activeEntitiesPtrPool{nullptr};

void EntityMng::spawnProyectile(Vector2 pos, Vector2 direction, Character* playerPtr){
    for(auto& proyectile : proyectilePool){
        if(!proyectile.getAlive()){
            proyectile.spawnReset(pos, direction, playerPtr);
            // std::cout << "[Proyectile spawned in pool!]" << std::endl;
            return;
        }}
    // std::cout << "[Proyectile pool full!]" << std::endl;
}

void EntityMng::tickProyectiles(float deltaTime){
    for(auto& proyectile : proyectilePool){
        if(proyectile.getAlive()){
            proyectile.tick(deltaTime);
        }}
}

void EntityMng::showProyectilesDebugData(){
    for(auto& proyectile : proyectilePool){
        if(proyectile.getAlive()){
            proyectile.showDebugData();
        }
    }
}

void EntityMng::spawnItem(Vector2 pos, Character* playerPtr, const itemData* item_data){
    for(auto& item : itemPool){
        if(!item.getAlive()){
            item.spawnReset(pos, playerPtr, item_data);
            std::cout << "[Item spawned in pool!]" << std::endl;
            return;
        }
    }
    std::cout << "[Item pool full!]" << std::endl;
}

void EntityMng::killItem(){
    for(auto& item : itemPool){
        if(item.getAlive()){
            item.setAlive(false);
            std::cout << "[Item deactivated!]" << std::endl;
            return;
        }
    }
    std::cout << "[Item pool empty!]" << std::endl;
}

void EntityMng::tickItems(float deltaTime){
    for(auto& item : itemPool){
        if(item.getAlive()){
            item.tick(deltaTime);
            if(!item.getAlive()) std::cout << "[Item deactivated!]" << std::endl;
        }
    }
}

void EntityMng::showItemsDebugData(){
    for(auto& item : itemPool){
        if(item.getAlive()) item.showDebugData();
    }
}

void EntityMng::spawnEnemy(Vector2 pos, Character* playerPtr, const enemyData* enemy_data){
    for(auto& enemy : enemyPool){
        if(!enemy.getAlive()){
            enemy.spawnReset(pos, enemy_data);
            enemy.setTarget(playerPtr);
            std::cout << "[Enemy spawned in pool!]" << std::endl;
            return;
        }
    }
    std::cout << "[Enemy pool full!]" << std::endl;
}

void EntityMng::killEnemy(){
    for(auto& enemy : enemyPool){
        if(enemy.getAlive()){
            enemy.deathSequence();
            std::cout << "[Enemy deactivated!]" << std::endl;
            return;
        }
    }
    std::cout << "[Enemy pool empty!]" << std::endl;
}

void EntityMng::tickEnemies(float deltaTime){
    for(auto& enemy : enemyPool){
        if(enemy.getAlive()){
            enemy.tick(deltaTime);
            if(!enemy.getAlive()) std::cout << "[Enemy deactivated!]" << std::endl;
        }
    }
}

void EntityMng::showEnemiesDebugData(){
    for(auto& enemy : enemyPool){
        if(enemy.getAlive()) enemy.showDebugData();
    }
}

void EntityMng::spawnProp(Vector2 pos, const propData* prop_data, Character* playerPtr){
    for(auto& prop : propPool){
        if(!prop.getAlive()){
            prop.spawnReset(pos, prop_data, playerPtr);
            std::cout << "[Prop spawned in pool!]" << std::endl;
            return;
        }}
    std::cout << "[Prop pool full!]" << std::endl;
}

void EntityMng::showPropsDebugData(){
    for(auto& prop : propPool){
        if(prop.getAlive()) prop.showDebugData();
    }
}

void EntityMng::checkPropCollisions(Character* playerPtr){
    Rectangle playerPrevCollisionRec{playerPtr->getPrevCollisionRecWorPos()};

    for(auto& prop : propPool){
        if(prop.getAlive()){
            Rectangle propCollisionRec{prop.getCollisionRecWorPos()};
            
            if( CheckCollisionRecs(propCollisionRec, playerPtr->getCollisionRecWorPos()) ){
                if(
                    playerPrevCollisionRec.x + playerPrevCollisionRec.width < propCollisionRec.x ||
                    playerPrevCollisionRec.x > propCollisionRec.x + propCollisionRec.width
                ) playerPtr->undoMovementX();
                else playerPtr->undoMovementY();
            }}}
}

void EntityMng::renderEntities(Character* playerPtr){
    std::array<Entity*, ENTITY_ARR_SIZE> renderQueue = {nullptr};

    size_t queueEnd{0};

    // add entities to render
    renderQueue[queueEnd] = playerPtr;
    queueEnd++;
    
    for(auto& entity : itemPool){
        if(entity.getAlive()){
            renderQueue[queueEnd] = &entity;
            queueEnd++;
        }}
    for(auto& entity : enemyPool){
        if(entity.getAlive()){
            renderQueue[queueEnd] = &entity;
            queueEnd++;
        }}
    for(auto& entity : proyectilePool){
        if(entity.getAlive()){
            renderQueue[queueEnd] = &entity;
            queueEnd++;
        }}
    for(auto& entity : propPool){
        if(entity.getAlive()){
            renderQueue[queueEnd] = &entity;
            queueEnd++;
        }}

    std::sort(renderQueue.begin(), renderQueue.begin() + queueEnd,
        [](Entity* a, Entity* b) {
            return a->getRenderPos().y + a->getHeight() < b->getRenderPos().y + b->getHeight();
        });

    for(size_t i{} ; i != queueEnd ; i++){
        renderQueue[i]->render();
    }
}

void EntityMng::clearEntityPools(){
    for(auto& item : itemPool){
        if(item.getAlive()) item.setAlive(false);
    }
    for(auto& enemy : enemyPool){
        if(enemy.getAlive()) enemy.setAlive(false);
    }
    for(auto& proyectile : proyectilePool){
        if(proyectile.getAlive()) proyectile.setAlive(false);
    }
    for(auto& prop : propPool){
        if(prop.getAlive()) prop.setAlive(false);
    }

    std::cout << "EntityManager: [Entity pools cleared]" << std::endl;
}