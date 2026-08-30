#include "EnemyBehaviour.h"
#include "EntityManager.h"

// ========= ENEMY BEHAVIOUR FUNCTIONS ==================================

// IDLE LOGIC
// =========================================================

void EnemyBehaviour::idleWandering(Enemy* thisEnemy, Character* player, const float& deltaTime)
{
    Vector2& velocity = thisEnemy->velocity;
    float& chaseTime = thisEnemy->chaseTime;
    const Vector2& wanderingPoint = thisEnemy->wanderingPoint;
    const Vector2& worldPos = thisEnemy->worldPos;

    if(Vector2Length(velocity) == 0.f) chaseTime += deltaTime;  // start counting when still

    // once every amount of time decide randomly whether to move or stay still
    if(chaseTime >= 2.f)
    {
        if(GetRandomValue(0,2))
        {
            // create random point to wander off to
            Vector2 randomDirection
            {
                static_cast<float>(GetRandomValue(-10,10)),
                static_cast<float>(GetRandomValue(-10,10))
            };
            float randomDistance = static_cast<float>(GetRandomValue(100,600));
            // float randomDistance = 600.f;

            thisEnemy->setWanderingPoint( Vector2Add(worldPos, Vector2Scale(Vector2Normalize(randomDirection), randomDistance)) );

            velocity = Vector2Subtract(wanderingPoint, worldPos);   // get direction to wander point
        }

        chaseTime = {};
    }
    
    // if too close or far to wander point, stop moving
    float wanderPointDistance = Vector2Length(Vector2Subtract(wanderingPoint, worldPos));
    if(wanderPointDistance < 20.f || wanderPointDistance > 610.f) velocity = {};
}

void EnemyBehaviour::idleWanderingAlert(Enemy* thisEnemy, Character* player, const float& deltaTime)
{
    Vector2& velocity = thisEnemy->velocity;
    float& chaseTime = thisEnemy->chaseTime;
    const Vector2& wanderingPoint = thisEnemy->wanderingPoint;
    const Vector2& worldPos = thisEnemy->worldPos;

    float distanceToPlayer{ Vector2Length( Vector2Subtract(player->getWorldPos(), worldPos) ) };
    if(distanceToPlayer < 400.f)
    {
        chaseTime = {};
        velocity = {};
        thisEnemy->setEnemyState(EnemyState::PLAYER_SPOTTED);
        thisEnemy->transitionLogic(thisEnemy, player, deltaTime);
        return;
    }

    if(Vector2Length(velocity) == 0.f) chaseTime += deltaTime;  // start counting when still

    // once every amount of time decide randomly whether to move or stay still
    if(chaseTime >= 2.f)
    {
        if(GetRandomValue(0,2))
        {
            // create random point to wander off to
            Vector2 randomDirection
            {
                static_cast<float>(GetRandomValue(-10,10)),
                static_cast<float>(GetRandomValue(-10,10))
            };
            float randomDistance = static_cast<float>(GetRandomValue(100,600));
            // float randomDistance = 600.f;

            thisEnemy->setWanderingPoint( Vector2Add(worldPos, Vector2Scale(Vector2Normalize(randomDirection), randomDistance)) );

            velocity = Vector2Subtract(wanderingPoint, worldPos);   // get direction to wander point
        }

        chaseTime = {};
    }
    
    // if too close or far to wander point, stop moving
    float wanderPointDistance = Vector2Length(Vector2Subtract(wanderingPoint, worldPos));
    if(wanderPointDistance < 20.f || wanderPointDistance > 610.f) velocity = {};
}


// TRANSITION LOGIC
// =========================================================

void EnemyBehaviour::playerSpottedSequence(Enemy* thisEnemy, Character* player, const float& deltaTime)
{
    float& chaseTime = thisEnemy->chaseTime;

    chaseTime += deltaTime;
    if(chaseTime >= 0.5f)
    {
        chaseTime = {};
        thisEnemy->setEnemyState(EnemyState::ACTION);
        thisEnemy->actionLogic(thisEnemy, player, deltaTime);
        return;
    }
}


// CONFRONTATION LOGIC
// =========================================================

void EnemyBehaviour::chaseTarget(Enemy* thisEnemy, Character* target, const float& deltaTime)
{
    Vector2& velocity = thisEnemy->velocity;
    float& in_radius = thisEnemy->radius;
    float& out_radius = thisEnemy->chaseRadius;
    float& chaseTime = thisEnemy->chaseTime;
    float& attCooldown = thisEnemy->attackTimer;

    // get to target
    velocity = Vector2Subtract(target->getWorldPos(), thisEnemy->getWorldPos());
    float distance = Vector2Length(velocity);

    if(attCooldown == 0.f)      // if attack cooldown is off
    {
        // damage player on contact
        if(CheckCollisionRecs(target->getHurtRec(),thisEnemy->getHurtRec()))
        {
            target->takeDamage(thisEnemy->getDamage());
            EntityMng::createKnockbackForce(Vector2Normalize(velocity), 25.f, target); // TO DO testing
            attCooldown += deltaTime;
        }
    }
    else if(attCooldown >= 1.f)
    {
        attCooldown = 0.f;
    }
    else
    {
        attCooldown += deltaTime;
    }

    // if too close / too far -> don't chase
    if(distance < in_radius){
        velocity = {};
        chaseTime = 0.f;
        return;
    }
    else if(distance > out_radius)
    {
        velocity = {};
        chaseTime = 0.f;
        thisEnemy->setEnemyState(EnemyState::IDLE);
        return;
    }

    chaseTime += deltaTime;
    if(chaseTime < 0.2f || chaseTime >= 5.f) velocity = {}; // wait a bit to move, and at 5 sec stop
}

void EnemyBehaviour::fleeTarget(Enemy* thisEnemy, Character* target, const float& deltaTime)
{
    Vector2& velocity = thisEnemy->velocity;
    float& out_radius = thisEnemy->chaseRadius; // unused
    float& chaseTime = thisEnemy->chaseTime;
    
    // get inverted target direction
    velocity = Vector2Subtract(thisEnemy->getWorldPos(), target->getWorldPos());
    float distance = Vector2Length(velocity);

    // if too far don't flee
    if(distance > 600.f){
      velocity = {};
      chaseTime = 0.f;
      thisEnemy->setEnemyState(EnemyState::IDLE);
      return;
    }

    chaseTime += deltaTime;
    
    if(chaseTime < 0.2f) velocity = {}; // wait a bit to start moving
    else if (chaseTime >= 5.f){ // after 5 sec return to idle
        velocity = {};
        chaseTime = 0.f;
        thisEnemy->setEnemyState(EnemyState::IDLE);
        return;
    }
}

void EnemyBehaviour::shootTarget(Enemy* thisEnemy, Character* target, const float& deltaTime)
{
    Vector2& velocity = thisEnemy->velocity;
    float& attCooldown = thisEnemy->attackTimer;
    float& fleeTimer = thisEnemy->fleeTimer;
    float& chaseTimer = thisEnemy->chaseTime;
    float& askNearestEnemyTimer = thisEnemy->freeUseTimer1;
    Enemy*& nearestEnemy = thisEnemy->nearestEnemy;

    // get target direction
    Vector2 vecToPlayer = Vector2Subtract(target->getWorldPos(), thisEnemy->getWorldPos());
    float distanceToPlayer = Vector2Length(vecToPlayer);
    velocity = vecToPlayer;
    
    // get direction of nearest enemy of same type
    Vector2 vecAwayFromNearestEnemy{};
    float distanceToNearestEnemy{};

    if(askNearestEnemyTimer == 0.f)
    {
        nearestEnemy = EntityMng::getNearestChasingEnemyByType(thisEnemy);    // TO DO expensive function, shouldn't be done every frame
        askNearestEnemyTimer += deltaTime;
    }
    else if(askNearestEnemyTimer >= 0.7f) askNearestEnemyTimer = 0.f;
    else askNearestEnemyTimer += deltaTime;

    if(nearestEnemy != nullptr)
    {
        vecAwayFromNearestEnemy = Vector2Subtract(thisEnemy->getWorldPos(), nearestEnemy->getWorldPos());
        distanceToNearestEnemy = Vector2Length(vecAwayFromNearestEnemy);
    }

    if(thisEnemy->flee)
    {
        velocity = Vector2Scale(vecToPlayer, -1.f);

        fleeTimer += deltaTime;
        if(fleeTimer >= 0.7f && distanceToPlayer > 380.f) {fleeTimer = 0.f; thisEnemy->flee = false;}
    }
    
    // get away from near enemy
    if(distanceToNearestEnemy <= 200.f)     // if too close to nearest enemy, prioritize them
    {
        velocity = Vector2Add( Vector2Scale(velocity, 0.2f), Vector2Scale(vecAwayFromNearestEnemy, 0.8f) );
    }
    else if(distanceToNearestEnemy <= 400.f)    // if nearest enemy and player are equally distant, get away from both
    {
        velocity = Vector2Add( Vector2Scale(velocity, 0.5f), Vector2Scale(vecAwayFromNearestEnemy, 0.5f) );
    }
        
    if(distanceToPlayer > 430.f)
    {
        thisEnemy->chase = true;
        
        chaseTimer += deltaTime;
        if(chaseTimer < 0.2f) velocity = {};        // wait a bit before chasing
        else if (chaseTimer >= 7.f){                // become neutral after 5 sec chasing
            velocity = {};
            thisEnemy->setEnemyState(EnemyState::IDLE);
            chaseTimer = 0.f;
        }

        return;
    }
    
    chaseTimer = 0.f;
    
    // if in radius -> stop and shoot
    if(!thisEnemy->flee)
    {   
        if(distanceToPlayer <= 360.f)
        {
            thisEnemy->chase = false;
        }
        if(!thisEnemy->chase)
        {
            if(distanceToPlayer >= 215.f)
            {
                if(attCooldown == 0.f){     // if cooldown is off -> shoot proyectile
                    // velocity = vecToPlayer;
                    EntityMng::spawnProyectile(thisEnemy->getWorldPos(), vecToPlayer, true);
                    attCooldown += deltaTime;
                } else if (attCooldown >= 0.8f) attCooldown = 0.f;
                else attCooldown += deltaTime;
                
                thisEnemy->setDrawColor(BLUE);
            }
            else {
                // if too close -> get away
                thisEnemy->flee = true;
            }
            velocity = {};
        }
    }
    
}