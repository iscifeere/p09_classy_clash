#include "EntityManager.h"
#include <iostream>
#include <algorithm>

Character EntityMng::player{};
std::array<Enemy, EntityMng::ENEMY_ARR_SIZE> EntityMng::enemyPool{};
std::array<Item, EntityMng::ITEM_ARR_SIZE> EntityMng::itemPool{};
std::array<GenEntity, EntityMng::PROYECTILE_ARR_SIZE> EntityMng::proyectilePool{};
std::array<Prop, EntityMng::PROP_ARR_SIZE> EntityMng::propPool{};
std::array<SwordSlash, EntityMng::ATTACK_ENTITY_ARR_SIZE> EntityMng::attackEntityPool{};
std::array<EntityVariant, EntityMng::ENTITY_ARR_SIZE> EntityMng::activeEntities{};
size_t EntityMng::i_EntitiesEnd{0};
size_t EntityMng::i_EnemiesStart{0};
size_t EntityMng::i_EnemiesEnd{0};
size_t EntityMng::i_ProyectilesStart{0};
size_t EntityMng::i_ProyectilesEnd{0};
size_t EntityMng::i_AttacksStart{0};
size_t EntityMng::i_AttacksEnd{0};
std::array<KnockbackForce, EntityMng::KNOCKBACK_ARR_SIZE> EntityMng::m_knockbackPool{};

void EntityMng::createKnockbackForce(Vector2 direction, float magnitude, BaseCharacter* targetCharacter){
    for(auto& knockback : m_knockbackPool){
        if(!knockback.isActive()){
            knockback.reset(direction, magnitude, targetCharacter);
            return;
        }}
}

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

void EntityMng::spawnAttack(Vector2 pos, float damage){
    for(auto& attackEntity : attackEntityPool){
        if(!attackEntity.getAlive()){
            attackEntity.spawnReset(pos, damage);
            std::cout << "[Attack entity spawned in pool!]" << std::endl;
            return;
        }
    }
    std::cout << "[Attack entity pool full!]" << std::endl;
}

void EntityMng::checkCollisions(){
    checkEnemyCollisions();
    checkPropCollisions();
    checkEntityMapBoundsCollisions();
    checkProyectileCollisions();
    checkAttackCollisions();
}

void EntityMng::checkPropCollisions(){
    Rectangle propCollisionRec{};
    Rectangle playerCollisionRecWorPos{player.getCollisionRecWorPos()};
    Rectangle playerPrevCollisionRec{player.getPrevCollisionRecWorPos()};
    Rectangle enemyCollisionRecWorPos{};
    Rectangle enemyPrevCollisionRec{};
    Rectangle projectileCollisionRecWorPos{};
    // Rectangle projectilePrevCollisionRec{};

    for(auto& prop : propPool){ // TODO optimize -> loop through active props
        if(prop.getAlive()){
            propCollisionRec = prop.getCollisionRecWorPos();
            
            // Collision with player
            if( CheckCollisionRecs(propCollisionRec, playerCollisionRecWorPos) ){

                // Check in which direction they are approaching
                if(playerPrevCollisionRec.x + playerPrevCollisionRec.width < propCollisionRec.x) // approaching prop from right
                {
                    // Calculate collision area and push player out
                    player.addWorldPosX( propCollisionRec.x - (playerCollisionRecWorPos.x + playerCollisionRecWorPos.width) - 1.f );
                }
                else if(playerPrevCollisionRec.x > propCollisionRec.x + propCollisionRec.width) // approaching prop from left
                {
                    player.addWorldPosX( (propCollisionRec.x + propCollisionRec.width) - playerCollisionRecWorPos.x + 1.f );
                }
                else if(playerPrevCollisionRec.y + playerPrevCollisionRec.height < propCollisionRec.y) // approaching prop from above
                {
                    player.addWorldPosY( propCollisionRec.y - (playerCollisionRecWorPos.y + playerCollisionRecWorPos.height) - 1.f );
                }
                else if(playerPrevCollisionRec.y > propCollisionRec.y + propCollisionRec.height) // approaching prop from below
                {
                    player.addWorldPosY( (propCollisionRec.y + propCollisionRec.height) - playerCollisionRecWorPos.y + 1.f );
                }

                playerCollisionRecWorPos = player.getCollisionRecWorPos();
            }

            // Collision with enemy
            forEachActiveEnemy(
                [
                    &propCollisionRec,
                    &enemyCollisionRecWorPos,
                    &enemyPrevCollisionRec
                ](Enemy& enemy)
                {
                    enemyCollisionRecWorPos = enemy.getCollisionRecWorPos();
                    enemyPrevCollisionRec = enemy.getPrevCollisionRecWorPos();
                    
                    if( CheckCollisionRecs(propCollisionRec, enemyCollisionRecWorPos) ){

                        // Check in which direction they are approaching
                        if(enemyPrevCollisionRec.x + enemyPrevCollisionRec.width < propCollisionRec.x)
                        {
                            enemy.addWorldPosX( propCollisionRec.x - (enemyCollisionRecWorPos.x + enemyCollisionRecWorPos.width) - 1.f );
                        }
                        else if(enemyPrevCollisionRec.x > propCollisionRec.x + propCollisionRec.width)
                        {
                            enemy.addWorldPosX( (propCollisionRec.x + propCollisionRec.width) - enemyCollisionRecWorPos.x + 1.f );
                        }
                        else if(enemyPrevCollisionRec.y + enemyPrevCollisionRec.height < propCollisionRec.y)
                        {
                            enemy.addWorldPosY( propCollisionRec.y - (enemyCollisionRecWorPos.y + enemyCollisionRecWorPos.height) - 1.f );
                        }
                        else if(enemyPrevCollisionRec.y > propCollisionRec.y + propCollisionRec.height)
                        {
                            enemy.addWorldPosY( (propCollisionRec.y + propCollisionRec.height) - enemyCollisionRecWorPos.y + 1.f );
                        }
                    }
                }
            );

            // Collision with projectile
            forEachActiveProjectile(
                [
                    &propCollisionRec,
                    &projectileCollisionRecWorPos
                ](GenEntity& projectile)
                {
                    projectileCollisionRecWorPos = projectile.getCollisionRecWorPos();
                    // kill projectile on collision
                    if( CheckCollisionRecs(propCollisionRec, projectileCollisionRecWorPos) ){
                        projectile.setAlive(false); // still on active entities
                        // TODO when disabling entity also remove it from activeEntities
                        // disableEntity(projectile) --> kills it and removes it from activeEntities
                    }
                }
            );
            
            // Projectiles get treated as solid by collision (fun)
            // forEachActiveProjectile(
            //     [
            //         &propCollisionRec,
            //         &projectileCollisionRecWorPos,
            //         &projectilePrevCollisionRec
            //     ](GenEntity& projectile)
            //     {
            //         projectileCollisionRecWorPos = projectile.getCollisionRecWorPos();
            //         projectilePrevCollisionRec = projectile.getPrevCollisionRecWorPos();
                    
            //         if( CheckCollisionRecs(propCollisionRec, projectileCollisionRecWorPos) ){

            //             // Check in which direction they are approaching
            //             if(projectilePrevCollisionRec.x + projectilePrevCollisionRec.width < propCollisionRec.x)
            //             {
            //                 projectile.addWorldPosX( propCollisionRec.x - (projectileCollisionRecWorPos.x + projectileCollisionRecWorPos.width) - 1.f );
            //             }
            //             else if(projectilePrevCollisionRec.x > propCollisionRec.x + propCollisionRec.width)
            //             {
            //                 projectile.addWorldPosX( (propCollisionRec.x + propCollisionRec.width) - projectileCollisionRecWorPos.x + 1.f );
            //             }
            //             else if(projectilePrevCollisionRec.y + projectilePrevCollisionRec.height < propCollisionRec.y)
            //             {
            //                 projectile.addWorldPosY( propCollisionRec.y - (projectileCollisionRecWorPos.y + projectileCollisionRecWorPos.height) - 1.f );
            //             }
            //             else if(projectilePrevCollisionRec.y > propCollisionRec.y + propCollisionRec.height)
            //             {
            //                 projectile.addWorldPosY( (propCollisionRec.y + propCollisionRec.height) - projectileCollisionRecWorPos.y + 1.f );
            //             }
            //         }
            //     }
            // );
        }
    }
}

void EntityMng::checkProyectileCollisions(){
    if(i_ProyectilesStart < i_ProyectilesEnd){     // if there are alive projectiles
        GenEntity* proyectile{nullptr};
        Enemy* enemy{nullptr};

        if(i_EnemiesStart < i_EnemiesEnd){      // if there are alive enemies
            
            for(size_t i{i_ProyectilesStart} ; i < i_ProyectilesEnd ; ++i){     // loop through projectiles
                proyectile = std::get<GenEntity*>(activeEntities[i]);
    
                if(proyectile->getIsEnemy()) proyectile->checkPlayerCollision();    // if it's an enemy projectile, affect player
                else{
                    for(size_t k{i_EnemiesStart} ; k < i_EnemiesEnd ; ++k){     // if not, loop through enemies
                        enemy = std::get<Enemy*>(activeEntities[k]);
    
                        if(CheckCollisionRecs( proyectile->getCollisionRec(), enemy->getHurtRec() )){   // affect one enemy
                            enemy->takeDamage(20);      // (if enemy dies, it's still in active entities for that frame...)
                            proyectile->setAlive(false);    // (still in active entities)
                            break;
                        }
                    }
                }
            }
        }
        else{       // if there aren't alive enemies
            for(size_t i{i_ProyectilesStart} ; i < i_ProyectilesEnd ; ++i){     // loop through projectiles
                proyectile = std::get<GenEntity*>(activeEntities[i]);
                if(proyectile->getIsEnemy()) proyectile->checkPlayerCollision();    // if it's an enemy projectile, affect player
            }
        }
    }
}

void EntityMng::checkAttackCollisions(){
    if(i_AttacksStart < i_AttacksEnd && i_EnemiesStart < i_EnemiesEnd){     // if there are alive attacks and enemies
        SwordSlash* attack{nullptr};
        Enemy* enemy{nullptr};

        for(size_t i{i_AttacksStart} ; i < i_AttacksEnd ; ++i){     // loop through attacks
            attack = std::get<SwordSlash*>(activeEntities[i]);

            for(size_t k{i_EnemiesStart} ; k < i_EnemiesEnd ; ++k){     // loop through enemies
                enemy = std::get<Enemy*>(activeEntities[k]);

                if(CheckCollisionRecs( attack->getHitBox(), enemy->getHurtRec() )){   // affect enemies
                    enemy->takeDamage(attack->getDamage());
                    createKnockbackForce( Vector2Normalize(Vector2Subtract(enemy->getWorldPos(), player.getWorldPos())), 25.f, enemy );
                    attack->setAlive(false);
                }
            }
        }
    }
}

void EntityMng::checkEntityMapBoundsCollisions(){
    player.checkMapBoundsCollision();

    Enemy* enemy{nullptr};
    for(size_t i{i_EnemiesStart} ; i < i_EnemiesEnd ; ++i){
        enemy = std::get<Enemy*>(activeEntities[i]);
        enemy->checkMapBoundsCollision();
    }
}

void EntityMng::checkEnemyCollisions(){
    Rectangle playerCollisionRecWorPos{player.getCollisionRecWorPos()};
    Rectangle playerPrevCollisionRec{player.getPrevCollisionRecWorPos()};
    Rectangle enemyCollisionRecWorPos{};
    Rectangle enemyPrevCollisionRec{};
    float halfCollisionLength{};

    // forEachActiveEnemy(checkCollision);
    forEachActiveEnemy(
        [
            &playerCollisionRecWorPos,
            &playerPrevCollisionRec,
            &enemyCollisionRecWorPos,
            &enemyPrevCollisionRec,
            &halfCollisionLength
        ](Enemy& enemy)
        {
            enemyCollisionRecWorPos = enemy.getCollisionRecWorPos();
            enemyPrevCollisionRec = enemy.getPrevCollisionRecWorPos();
            
            if( CheckCollisionRecs(enemyCollisionRecWorPos, playerCollisionRecWorPos) )
            {

                // Check in which direction they are approaching
                if(playerPrevCollisionRec.x + playerPrevCollisionRec.width < enemyPrevCollisionRec.x)
                {
                    // Calculate collision area and push both entities in opposite directions
                    // Entity speed decides who pushes more -> When pushing against each other, the faster entity will push the slower one
                    halfCollisionLength = ((playerCollisionRecWorPos.x + playerCollisionRecWorPos.width) - enemyCollisionRecWorPos.x + 1.f) * 0.5f;
                    enemy.addWorldPosX( halfCollisionLength );
                    player.addWorldPosX( -halfCollisionLength );
                }
                else if(playerPrevCollisionRec.x > enemyPrevCollisionRec.x + enemyPrevCollisionRec.width)
                {
                    halfCollisionLength = (playerCollisionRecWorPos.x - (enemyCollisionRecWorPos.x + enemyCollisionRecWorPos.width) - 1.f) * 0.5f;
                    enemy.addWorldPosX( halfCollisionLength );
                    player.addWorldPosX( -halfCollisionLength );
                }
                else if(playerPrevCollisionRec.y + playerPrevCollisionRec.height < enemyPrevCollisionRec.y)
                {
                    halfCollisionLength = ((playerCollisionRecWorPos.y + playerCollisionRecWorPos.height) - enemyCollisionRecWorPos.y + 1.f) * 0.5f;
                    enemy.addWorldPosY( halfCollisionLength );
                    player.addWorldPosY( -halfCollisionLength );
                }
                else if(playerPrevCollisionRec.y > enemyPrevCollisionRec.y + enemyPrevCollisionRec.height)
                {
                    halfCollisionLength = (playerCollisionRecWorPos.y - (enemyCollisionRecWorPos.y + enemyCollisionRecWorPos.height) - 1.f) * 0.5f;
                    enemy.addWorldPosY( halfCollisionLength );
                    player.addWorldPosY( -halfCollisionLength );
                }

                playerCollisionRecWorPos = player.getCollisionRecWorPos();
            }
        }
    );
}

void EntityMng::tickEntities(float deltaTime){
    i_EntitiesEnd = 0;
    i_EnemiesStart = 0;
    i_EnemiesEnd = 0;
    i_ProyectilesStart = 0;
    i_ProyectilesEnd = 0;
    i_AttacksStart = 0;
    i_AttacksEnd = 0;

    player.tick(deltaTime);
    // add player to active entities
    activeEntities[i_EntitiesEnd] = &player;
    i_EntitiesEnd++;

    i_AttacksStart = i_EntitiesEnd;
    for(auto& attackEntity : attackEntityPool){
        if(attackEntity.getAlive()){
            attackEntity.tick(deltaTime);

            if(!attackEntity.getAlive()) continue;
            activeEntities[i_EntitiesEnd] = &attackEntity;
            i_EntitiesEnd++;
        }
    }
    i_AttacksEnd = i_EntitiesEnd;

    i_EnemiesStart = i_EntitiesEnd;
    for(auto& enemy : enemyPool){
        if(enemy.getAlive()){
            enemy.tick(deltaTime);

            if(!enemy.getAlive()) continue;    // filter out entities who died inside tick logic
            activeEntities[i_EntitiesEnd] = &enemy; // add alive entity to active entities
            i_EntitiesEnd++;
        }}
    i_EnemiesEnd = i_EntitiesEnd;

    for(auto& item : itemPool){
        if(item.getAlive()){
            item.tick(deltaTime);

            if(!item.getAlive()) continue;
            activeEntities[i_EntitiesEnd] = &item;
            i_EntitiesEnd++;
        }}

    i_ProyectilesStart = i_EntitiesEnd;
    for(auto& proyectile : proyectilePool){
        if(proyectile.getAlive()){
            proyectile.tick(deltaTime);

            if(!proyectile.getAlive()) continue;
            activeEntities[i_EntitiesEnd] = &proyectile;
            i_EntitiesEnd++;
        }}
    i_ProyectilesEnd = i_EntitiesEnd;

    for(auto& prop : propPool){
        if(prop.getAlive()){
            prop.tick(deltaTime);

            if(!prop.getAlive()) continue;
            activeEntities[i_EntitiesEnd] = &prop;
            i_EntitiesEnd++;
        }}

    for(auto& knockback : m_knockbackPool){
        if(knockback.isActive()){
            knockback.tick(deltaTime);
        }
    }
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

    for(int i{} ; i < 20 ; i++){
        Vector2 newEnemyPos{
            static_cast<float>(GetRandomValue(1500,5350)),
            static_cast<float>(GetRandomValue(1500,5350))
        };

        randomEnemy = GetRandomValue(0,14);
        
        // spawn ratios of each enemy type
        if(randomEnemy < 6) data = &MADKNIGHT_ENEMYDATA;
        else if(randomEnemy < 9) data = &SLIME_ENEMYDATA;
        else if(randomEnemy < 10) data = &SLIME_BLUE_ENEMYDATA;
        else if(randomEnemy < 11) data = &RED_ENEMYDATA;
        else data = &GOBLIN_ENEMYDATA;

        spawnEnemy(newEnemyPos, data);
    }
}

Enemy* EntityMng::getNearestEnemy(Enemy* this_enemy){   // returns a pointer to the nearest enemy
    Enemy* nearestEnemy{nullptr};
    float distanceToNearestEnemy{};
    float distanceToCurrentEnemy{};

    for(Enemy& enemy : enemyPool){
        if(enemy.getAlive() && &enemy != this_enemy){
            if(nearestEnemy == nullptr) nearestEnemy = &enemy;
            else {
                distanceToNearestEnemy = Vector2Length( Vector2Subtract(nearestEnemy->getWorldPos(), this_enemy->getWorldPos()) );
                distanceToCurrentEnemy = Vector2Length( Vector2Subtract(enemy.getWorldPos(), this_enemy->getWorldPos()) );
                
                if(distanceToCurrentEnemy < distanceToNearestEnemy){
                    nearestEnemy = &enemy;
                    distanceToNearestEnemy = distanceToCurrentEnemy;
                }

            }
        }
    }

    return nearestEnemy;   // returns nullptr if no enemy is found
}

Enemy* EntityMng::getNearestEnemyByType(Enemy* this_enemy){     // returns a pointer to the nearest enemy of the same type
    Enemy* nearestEnemy{nullptr};
    float distanceToNearestEnemy{};
    float distanceToCurrentEnemy{};
    EnemyType thisEnemyType = this_enemy->getEnemyType();

    for(Enemy& enemy : enemyPool){
        if(enemy.getAlive() && enemy.getEnemyType() == thisEnemyType && &enemy != this_enemy){
            if(nearestEnemy == nullptr) nearestEnemy = &enemy;
            else {
                distanceToNearestEnemy = Vector2Length( Vector2Subtract(nearestEnemy->getWorldPos(), this_enemy->getWorldPos()) );
                distanceToCurrentEnemy = Vector2Length( Vector2Subtract(enemy.getWorldPos(), this_enemy->getWorldPos()) );
                
                if(distanceToCurrentEnemy < distanceToNearestEnemy){
                    nearestEnemy = &enemy;
                    distanceToNearestEnemy = distanceToCurrentEnemy;
                }

            }
        }
    }

    return nearestEnemy;   // returns nullptr if no enemy is found
}

Enemy* EntityMng::getNearestEnemyByType(Enemy* this_enemy, EnemyType p_EnemyType){    // returns a pointer to the nearest enemy of a specified type
    Enemy* nearestEnemy{nullptr};
    float distanceToNearestEnemy{};
    float distanceToCurrentEnemy{};

    for(Enemy& enemy : enemyPool){
        if(enemy.getAlive() && enemy.getEnemyType() == p_EnemyType && &enemy != this_enemy){
            if(nearestEnemy == nullptr) nearestEnemy = &enemy;
            else {
                distanceToNearestEnemy = Vector2Length( Vector2Subtract(nearestEnemy->getWorldPos(), this_enemy->getWorldPos()) );
                distanceToCurrentEnemy = Vector2Length( Vector2Subtract(enemy.getWorldPos(), this_enemy->getWorldPos()) );
                
                if(distanceToCurrentEnemy < distanceToNearestEnemy){
                    nearestEnemy = &enemy;
                    distanceToNearestEnemy = distanceToCurrentEnemy;
                }

            }
        }
    }

    return nearestEnemy;   // returns nullptr if no enemy is found
}

Enemy* EntityMng::getNearestChasingEnemyByType(Enemy* this_enemy){     // returns a pointer to the nearest enemy that's chasing the player of the same type
    Enemy* nearestEnemy{nullptr};
    float distanceToNearestEnemy{};
    float distanceToCurrentEnemy{};
    EnemyType thisEnemyType = this_enemy->getEnemyType();

    for(Enemy& enemy : enemyPool){
        if(enemy.getAlive() && enemy.getEnemyType() == thisEnemyType && enemy.getEnemyState() == EnemyState::ACTION && &enemy != this_enemy){
            if(nearestEnemy == nullptr) nearestEnemy = &enemy;
            else {
                distanceToNearestEnemy = Vector2Length( Vector2Subtract(nearestEnemy->getWorldPos(), this_enemy->getWorldPos()) );
                distanceToCurrentEnemy = Vector2Length( Vector2Subtract(enemy.getWorldPos(), this_enemy->getWorldPos()) );
                
                if(distanceToCurrentEnemy < distanceToNearestEnemy){
                    nearestEnemy = &enemy;
                    distanceToNearestEnemy = distanceToCurrentEnemy;
                }

            }
        }
    }

    return nearestEnemy;   // returns nullptr if no enemy is found
}

Enemy& EntityMng::getActiveEnemyAtIndex(size_t enemyIndex){
    // [!] doesn't check wether enemy index is a valid active entity index or not
    // if(enemyIndex < i_EnemiesStart || enemyIndex >= i_EnemiesEnd) return null or smth;
    Enemy* enemy{ std::get<Enemy*>(activeEntities[enemyIndex]) }; // <-- std::get() will throw an exception if index isn't valid
    return *enemy;
}

GenEntity& EntityMng::getActiveProjectileAtIndex(size_t projectileIndex){
    // [!] doesn't check if index is valid
    // if(index < firstEntityIndex || index > lastEntityIndex) return;
    GenEntity* projectile{ std::get<GenEntity*>(activeEntities[projectileIndex]) }; // <-- will throw if index isn't valid
    return *projectile;
}

void EntityMng::forEachActiveEnemy(std::function<void(Enemy&)> func){
    for(size_t i{i_EnemiesStart} ; i < i_EnemiesEnd ; ++i)
    {
        func(getActiveEnemyAtIndex(i));
    }
}

void EntityMng::forEachActiveProjectile(std::function<void(GenEntity&)> func){
    for(size_t i{i_ProyectilesStart} ; i < i_ProyectilesEnd ; ++i)
    {
        func(getActiveProjectileAtIndex(i));
    }
}