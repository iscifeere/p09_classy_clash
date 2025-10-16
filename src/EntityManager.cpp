#include "EntityManager.h"
#include <iostream>
#include <algorithm>

#define DEBUG // for console logging when debugging

std::array<Item, EntityMng::ITEM_ARR_SIZE> EntityMng::itemPool{};
std::array<Enemy, EntityMng::ENEMY_ARR_SIZE> EntityMng::enemyPool{};
std::array<GenEntity, EntityMng::PROYECTILE_ARR_SIZE> EntityMng::proyectilePool{};
std::array<Prop, EntityMng::PROP_ARR_SIZE> EntityMng::propPool{};
std::array<EntityVariant, EntityMng::ENTITY_ARR_SIZE> EntityMng::activeEntities{};
size_t EntityMng::i_EntitiesEnd{0};

void EntityMng::spawnProyectile(Vector2 pos, Vector2 direction, Character* playerPtr){
    for(auto& proyectile : proyectilePool){
        if(!proyectile.getAlive()){
            proyectile.spawnReset(pos, direction, playerPtr);

            // add to active entities
            if(i_EntitiesEnd < ENTITY_ARR_SIZE){
                activeEntities[i_EntitiesEnd] = &proyectile;
                i_EntitiesEnd++;
            }

            return;
        }}
    std::cout << "[Proyectile pool full!]" << std::endl;
}

void EntityMng::tickProyectiles(float deltaTime){
    for(auto& proyectile : proyectilePool){
        if(proyectile.getAlive()){
            proyectile.tick(deltaTime);
        }}
}

void EntityMng::showProyectilesDebugData(){
    int proyectilesAlive{};

    for(auto& proyectile : proyectilePool){
        if(proyectile.getAlive()){
            proyectile.showDebugData();
            proyectilesAlive++;
        }
    }

    DrawText(TextFormat("proy: %01i",proyectilesAlive), 55.f, 185.f, 30, WHITE);
}

void EntityMng::spawnItem(Vector2 pos, Character* playerPtr, const itemData* item_data){
    for(auto& item : itemPool){
        if(!item.getAlive()){
            item.spawnReset(pos, playerPtr, item_data);

            // add to active entities
            if(i_EntitiesEnd < ENTITY_ARR_SIZE){
                activeEntities[i_EntitiesEnd] = &item;
                i_EntitiesEnd++;
            }

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

            // add to active entities
            if(i_EntitiesEnd < ENTITY_ARR_SIZE){
                activeEntities[i_EntitiesEnd] = &enemy;
                i_EntitiesEnd++;
            }

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

            // add to active entities
            if(i_EntitiesEnd < ENTITY_ARR_SIZE){
                activeEntities[i_EntitiesEnd] = &prop;
                i_EntitiesEnd++;
            }

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

void EntityMng::tickEntities(float deltaTime, Character* playerPtr){
    playerPtr->tick(deltaTime);

    for(int i{} ; i < i_EntitiesEnd ; i++){
        EntityVariant& currentEntity = activeEntities[i];
        EntityVariant& lastEntity = activeEntities[i_EntitiesEnd-1];
        bool isEntityAlive{false};

        // do tick on currentEntity
        std::visit(
            [&deltaTime, &isEntityAlive](auto&& e){
                e->tick(deltaTime);
                isEntityAlive = e->getAlive();  // update currentEntity alive state after tick
            },
            currentEntity
        );

        // if entity died inside tick logic
        if(!isEntityAlive){

            // if index is last active entity
            if(i == i_EntitiesEnd-1){
                // free the spot and update array end
                currentEntity.emplace<0>(nullptr);  // resetting to null Item*
                i_EntitiesEnd = i;
            }
            else{
                // copy last active entity ptr into current one
                currentEntity = lastEntity;
                // free last active entity and update array end
                lastEntity.emplace<0>(nullptr);
                i_EntitiesEnd--;
            }
        }

    }
}
void EntityMng::tickEntities2(float deltaTime, Character* playerPtr){
    playerPtr->tick(deltaTime);

    for(auto& enemy : enemyPool){
        if(enemy.getAlive()) enemy.tick(deltaTime);
    }
    for(auto& item : itemPool){
        if(item.getAlive()) item.tick(deltaTime);
    }
    for(auto& proyectile : proyectilePool){
        if(proyectile.getAlive()) proyectile.tick(deltaTime);
    }
    for(auto& prop : propPool){
        if(prop.getAlive()) prop.tick(deltaTime);
    }
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