#include "EntityManager.h"
#include <iostream>
#include <algorithm>

#define DEBUG // for console logging when debugging

Character EntityMng::player{};
std::array<Enemy, EntityMng::ENEMY_ARR_SIZE> EntityMng::enemyPool{};
std::array<Item, EntityMng::ITEM_ARR_SIZE> EntityMng::itemPool{};
std::array<GenEntity, EntityMng::PROYECTILE_ARR_SIZE> EntityMng::proyectilePool{};
std::array<Prop, EntityMng::PROP_ARR_SIZE> EntityMng::propPool{};
std::array<EntityVariant, EntityMng::ENTITY_ARR_SIZE> EntityMng::activeEntities{};
size_t EntityMng::i_EntitiesEnd{0};
size_t EntityMng::i_EnemiesStart{0};
size_t EntityMng::i_EnemiesEnd{0};
size_t EntityMng::i_ProyectilesStart{0};
size_t EntityMng::i_ProyectilesEnd{0};

void EntityMng::spawnProyectile(Vector2 pos, Vector2 direction, bool isEnemy){
    for(auto& proyectile : proyectilePool){
        if(!proyectile.getAlive()){
            proyectile.spawnReset(pos, direction, isEnemy);
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

void EntityMng::spawnItem(Vector2 pos, const itemData* item_data){
    for(auto& item : itemPool){
        if(!item.getAlive()){
            item.spawnReset(pos, item_data);
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

void EntityMng::spawnEnemy(Vector2 pos, const enemyData* enemy_data){
    for(auto& enemy : enemyPool){
        if(!enemy.getAlive()){
            enemy.spawnReset(pos, enemy_data);
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

void EntityMng::spawnProp(Vector2 pos, const propData* prop_data){
    for(auto& prop : propPool){
        if(!prop.getAlive()){
            prop.spawnReset(pos, prop_data);
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

void EntityMng::checkEntityCollisions(){
    checkPropCollisions();
    checkProyectileCollisions();
}

void EntityMng::checkPropCollisions(){
    Rectangle playerPrevCollisionRec{player.getPrevCollisionRecWorPos()};

    for(auto& prop : propPool){
        if(prop.getAlive()){
            Rectangle propCollisionRec{prop.getCollisionRecWorPos()};
            
            if( CheckCollisionRecs(propCollisionRec, player.getCollisionRecWorPos()) ){
                if(
                    playerPrevCollisionRec.x + playerPrevCollisionRec.width < propCollisionRec.x ||
                    playerPrevCollisionRec.x > propCollisionRec.x + propCollisionRec.width
                ) player.undoMovementX();
                else player.undoMovementY();
            }}}
}

void EntityMng::checkProyectileCollisions(){
    if(i_ProyectilesStart < i_ProyectilesEnd){     // if there is alive proyectiles
        GenEntity* proyectile{nullptr};

        for(size_t i{i_ProyectilesStart} ; i < i_ProyectilesEnd ; ++i){     // loop through proyectiles
            proyectile = std::get<GenEntity*>(activeEntities[i]);

            if(proyectile->getIsEnemy()) proyectile->checkPlayerCollision();    // if it's an enemy proyectile, affect player
            else if(i_EnemiesStart < i_EnemiesEnd){    // if not, then, if there is alive enemies
                Enemy* enemy{nullptr};

                for(size_t k{i_EnemiesStart} ; k < i_EnemiesEnd ; ++k){     // loop through enemies
                    enemy = std::get<Enemy*>(activeEntities[k]);

                    if(CheckCollisionRecs( proyectile->getCollisionRec(), enemy->getHurtRec() )){   // affect enemy
                        enemy->takeDamage(20);
                        proyectile->setAlive(false);
                        break;
                    }}
            }

        }
    }
}

void EntityMng::tickEntities(float deltaTime){
    i_EntitiesEnd = 0;
    i_EnemiesStart = 0;
    i_EnemiesEnd = 0;
    i_ProyectilesStart = 0;
    i_ProyectilesEnd = 0;

    player.tick(deltaTime);
    // add player to active entities
    activeEntities[i_EntitiesEnd] = &player;
    i_EntitiesEnd++;

    i_EnemiesStart = i_EntitiesEnd;
    for(auto& enemy : enemyPool){
        if(enemy.getAlive()){
            enemy.tick(deltaTime);

            // if(!enemy.getAlive()) break;    // filter out entities who died inside tick logic
            activeEntities[i_EntitiesEnd] = &enemy; // add alive entity to active entities
            i_EntitiesEnd++;
        }}
    i_EnemiesEnd = i_EntitiesEnd;

    for(auto& item : itemPool){
        if(item.getAlive()){
            item.tick(deltaTime);

            // if(!item.getAlive()) break;
            activeEntities[i_EntitiesEnd] = &item;
            i_EntitiesEnd++;
        }}

    i_ProyectilesStart = i_EntitiesEnd;
    for(auto& proyectile : proyectilePool){
        if(proyectile.getAlive()){
            proyectile.tick(deltaTime);

            // if(!proyectile.getAlive()) break;       // filtering out after tick glitches them, for some reason
            activeEntities[i_EntitiesEnd] = &proyectile;
            i_EntitiesEnd++;
        }}
    i_ProyectilesEnd = i_EntitiesEnd;

    for(auto& prop : propPool){
        if(prop.getAlive()){
            prop.tick(deltaTime);

            // if(!prop.getAlive()) break;
            activeEntities[i_EntitiesEnd] = &prop;
            i_EntitiesEnd++;
        }}
}

void EntityMng::showEntitiesDebugData(){
    int entitiesAlive{};

    for(int i{} ; i < i_EntitiesEnd ; ++i){
        std::visit(
            [&entitiesAlive](auto& entity) {
                entity->showDebugData();
                entitiesAlive++;
            },
            activeEntities[i]
        );
    }

    DrawText(TextFormat("ent: %01i",entitiesAlive), 55.f, 185.f, 30, WHITE);
}

void EntityMng::renderEntities(){

    // sort active entities by their anchor point positions
    std::sort(activeEntities.begin(), activeEntities.begin() + i_EntitiesEnd,
        [](EntityVariant a, EntityVariant b) {
            float anchorPosA{};
            float anchorPosB{};

            std::visit(
                [&anchorPosA](auto& entityA) {
                    anchorPosA = entityA->getRenderPos().y + entityA->getHeight();
                },
                a
            );
            std::visit(
                [&anchorPosB](auto& entityB) {
                    anchorPosB = entityB->getRenderPos().y + entityB->getHeight();
                },
                b
            );

            return anchorPosA < anchorPosB;
        });

    for(size_t i{} ; i < i_EntitiesEnd ; i++){
        std::visit(
            [](auto& entity) {
                entity->render();
            },
            activeEntities[i]
        );
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
    
    // props pool must not get cleared

    for(auto& variant : activeEntities){    // resetting array of pointers
        variant.emplace<0>(nullptr);
    }

    i_EntitiesEnd = 0;
    i_EnemiesStart = 0;
    i_EnemiesEnd = 0;
    i_ProyectilesStart = 0;
    i_ProyectilesEnd = 0;

    std::cout << "EntityManager: [Entity pools cleared]" << std::endl;
}

void EntityMng::logEntityArrayStatus(){
    bool alive{false};
    size_t aliveEntityPtr{};
    size_t notAliveEntityPtr{};
    std::cout << "\nEntityManager: Logging activeEntities array status" << std::endl;

    for(int i{} ; i < ENTITY_ARR_SIZE ; ++i){
        std::visit(
            [&alive](auto& entity){
                if(entity!=nullptr) alive = entity->getAlive();
                else alive = false;
            },
            activeEntities[i]
        );
        alive ? aliveEntityPtr++ : notAliveEntityPtr++;
        std::cout << "activeEntities[" << i << "]: " << alive << " , " << activeEntities[i].index() << std::endl;
        if(i == i_EntitiesEnd-1) std::cout << "END INDEX: [" << i_EntitiesEnd << "]" << std::endl;
    }

    std::cout << "Pointers to alive entitys: " << aliveEntityPtr << std::endl;
    std::cout << "Pointers to notAlive entity: " << notAliveEntityPtr << std::endl;
    std::cout << "Total pointers to entity: " << (aliveEntityPtr+notAliveEntityPtr) << std::endl;
}

void EntityMng::showPlayerScore(){
    DrawText(TextFormat("points: %01i",player.getKilledEnemies()), static_cast<float>(Tex::winSize[0]) - 150.f, 45.f, 30, WHITE);
}

void EntityMng::spawnRandomEnemies(){
    const enemyData* data{};
    int randomEnemy{};

    for(int i{} ; i < ENEMY_ARR_SIZE ; i++){
        Vector2 newEnemyPos{
            static_cast<float>(GetRandomValue(800,5000)),
            static_cast<float>(GetRandomValue(800,5000))
        };

        randomEnemy = GetRandomValue(0,10);
        if(randomEnemy < 5) data = &MADKNIGHT_ENEMYDATA;
        else if(randomEnemy < 10) data = &SLIME_ENEMYDATA;
        else data = &RED_ENEMYDATA;

        spawnEnemy(newEnemyPos, data);
    }
}