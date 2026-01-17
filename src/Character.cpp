#include "Character.h"
#include "raymath.h"
#include <iostream>
#include "EntityManager.h"

void Character::init(){
    std::cout << "[Player init function (" << this << ") ]" << std::endl;

    windowWidth = Tex::winSize[0];
    windowHeight = Tex::winSize[1];
    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;

    scale = 8.f;
    speed = 10.f;

    std::cout << "window width & height: " << windowWidth << ", " << windowHeight << std::endl;
    std::cout << "frame width & height: " << frameWidth << ", " << frameHeight << std::endl;
    std::cout << "scale: " << scale << std::endl;
    std::cout << "speed: " << speed << std::endl;
}

Character::Character()
{
    std::cout << "\n[Player default constructor (" << this << ") ]" << std::endl;
    init();
}

Character::Character(Vector2 pos)
{
    init();
    worldPos = pos;
}

Vector2 Character::getScreenPos(){
    return Tex::halfWinSize;
}

Vector2 Character::getWindowOriginWorPos(){
    return Vector2Subtract( worldPos, Tex::halfWinSize );
}

Vector2 Character::getPrevWorldPos(){
    return Vector2Subtract( worldPos, movement );
}

Rectangle Character::getCollisionRec(){
    Vector2 renderPos{getRenderPos()};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    return Rectangle{
        // displacement
        renderPos.x + ( scaledWidth * collisionBox.x ),
        renderPos.y + ( scaledHeight * collisionBox.y ),

        // scaling
        scaledWidth * collisionBox.width,
        scaledHeight * collisionBox.height
    };
}

Rectangle Character::getCollisionRecWorPos(){
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;
    Vector2 worldPosTopLeft{
        worldPos.x - scaledWidth*0.5f,
        worldPos.y - scaledHeight*0.5f
    };

    return Rectangle{
        // displacement
        worldPosTopLeft.x + ( scaledWidth * collisionBox.x ),
        worldPosTopLeft.y + ( scaledHeight * collisionBox.y ),

        // scaling
        scaledWidth * collisionBox.width,
        scaledHeight * collisionBox.height
    };
}

Rectangle Character::getPrevCollisionRecWorPos(){
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;
    Vector2 prevWorldPos{getPrevWorldPos()};
    Vector2 worldPosTopLeft{
        prevWorldPos.x - scaledWidth*0.5f,
        prevWorldPos.y - scaledHeight*0.5f
    };

    return Rectangle{
        // displacement
        worldPosTopLeft.x + ( scaledWidth * collisionBox.x ),
        worldPosTopLeft.y + ( scaledHeight * collisionBox.y ),

        // scaling
        scaledWidth * collisionBox.width,
        scaledHeight * collisionBox.height
    };
}

Rectangle Character::getHurtRec(){
    Vector2 renderPos{getRenderPos()};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    return Rectangle{
        // displacement
        renderPos.x + ( scaledWidth * hurtBox.x ),
        renderPos.y + ( scaledHeight * hurtBox.y ),

        // scaling
        scaledWidth * hurtBox.width,
        scaledHeight * hurtBox.height
    };
}

bool Character::tick(float deltaTime){

    if (!getAlive()) return false;    // if not alive, do nothing

    // ====== MOVEMENT ============
    velocity = {};          // reset velocity
    // direction vector for movement
    if(IsKeyDown(KEY_A)) velocity.x -= 1.0;
    if(IsKeyDown(KEY_D)) velocity.x += 1.0;
    if(IsKeyDown(KEY_W)) velocity.y -= 1.0;
    if(IsKeyDown(KEY_S)) velocity.y += 1.0;

    // run with SHIFT key
    if(IsKeyDown(KEY_LEFT_SHIFT) && Vector2Length(velocity) != 0.f){
        updateTime = 1.f/24.f;  // when SHIFT -> faster animation
        speed = 14.f;
    } else {
        updateTime = 1.f/12.f;
        speed = 10.f;
    }


    // ====== TICK AND VARIABLE RESETS ============
    BaseCharacter::tick(deltaTime);

    invul = false;  // reset invulnerability

    swordVariables.origin = {};
    swordVariables.offset = {};
    isAttacking = false;
    swordVariables.rotation = {};

    // ==================================================================
    // ATTACK AND SHIELD
    // ==================================================================

    if( attackTimer >= 0.40f ) attackTimer = 0.f;
    else if(attackTimer > 0.f) attackTimer += deltaTime;

    // shield
    if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON) || IsKeyDown(KEY_V))
    {
        swordVariables.rotation = -30.f;
        drawColor = YELLOW;
        invul = true;
    }
    else
    {   
        // sword animation and shoot projectile
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_SPACE))
        {
            swordVariables.rotation = 35.f;
            
            if(attackTimer == 0.f)
            {
                shootProyectile();
                attackTimer += deltaTime;

                // melee attack
                if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE) )
                {
                    isAttacking = true;
                }
            }
        }
    }

    if(rightLeft > 0.f){
        swordVariables.origin = {0.f, weapon->height * scale};
        swordVariables.offset = {70.f, 110.f};
        Vector2 renderPos{getRenderPos()};

        weaponCollisionRec = {
            renderPos.x + swordVariables.offset.x,
            renderPos.y + swordVariables.offset.y - weapon->height * scale,
            weapon->width * scale,
            weapon->height * scale
        };
    } else{
        swordVariables.origin = {weapon->width * scale, weapon->height * scale};
        swordVariables.offset = {50.f, 110.f};
        Vector2 renderPos{getRenderPos()};

        weaponCollisionRec = {
            renderPos.x + swordVariables.offset.x - weapon->width * scale,
            renderPos.y + swordVariables.offset.y - weapon->height * scale,
            weapon->width * scale,
            weapon->height * scale
        };
        swordVariables.rotation *= -1;
    }

    return true;
}


void Character::deathSequence(){
    setAlive(false);
}

void Character::addHealth( float healthAdd ){
    BaseCharacter::addHealth( healthAdd );
    setDrawColor(PINK);
}

void Character::checkWinCondition()
{
    winCondition = killedEnemies >= 7;
}

void Character::showDebugData(){
    Vector2 screenPos{getScreenPos()};
    Vector2 renderPos{getRenderPos( screenPos )};
    Rectangle collisionRec{getCollisionRec()};
    Rectangle hurtRec{getHurtRec()};

    // DrawRectangleLines(getWorldPos().x, getWorldPos().y, width*scale, height*scale, YELLOW); [SHOOTING AIM IDEA!!!]
    DrawRectangleLines(renderPos.x, renderPos.y, frameWidth*scale, frameHeight*scale, BLUE);    // sprite box
    DrawRectangleLines(hurtRec.x, hurtRec.y, hurtRec.width, hurtRec.height, RED);               // hurt box
    DrawRectangleLines(collisionRec.x, collisionRec.y, collisionRec.width, collisionRec.height, YELLOW);    // collision box
    DrawCircleV(screenPos, 5.f, BLUE);  // worldPos mark
    DrawText(TextFormat("X: %01.01f",worldPos.x), 55.f, 125.f, 30, WHITE);
    DrawText(TextFormat("Y: %01.01f",worldPos.y), 55.f, 155.f, 30, WHITE);
}

void Character::showStats(){

    Vector2 playerScreenPos{getScreenPos()};

    // draw health bar
    Rectangle healthBar{50.f, 45.f, 4.f * health, 40.f};
    if(health <= 100.f)
    {
        if( healthBarColor.r == 255 &&       // if color is WHITE
            healthBarColor.g == 255 &&
            healthBarColor.b == 255 &&
            healthBarColor.a == 255) healthBarColor = GREEN;

        DrawRectangle(healthBar.x, healthBar.y, 400.f, healthBar.height, BLACK);
        DrawRectangle(healthBar.x, healthBar.y, healthBar.width, healthBar.height, healthBarColor);
    }
    else if(health <= 300.f)
    {
        if( healthBarColor.r == 255 &&
            healthBarColor.g == 255 &&
            healthBarColor.b == 255 &&
            healthBarColor.a == 255) healthBarColor = BLUE;
        
        DrawRectangle(healthBar.x, healthBar.y, 400.f, healthBar.height, GREEN);
        DrawRectangle(healthBar.x, healthBar.y, ( healthBar.width - (4.f * 100.f) ) * 0.5f, healthBar.height, healthBarColor);
    } else{
        if( healthBarColor.r == 255 &&
            healthBarColor.g == 255 &&
            healthBarColor.b == 255 &&
            healthBarColor.a == 255) healthBarColor = BLUE;

        DrawRectangle(healthBar.x, healthBar.y, 400.f, healthBar.height, healthBarColor);
    }

    // draw attack cooldown bar
    Rectangle attackCooldownBar{playerScreenPos.x, playerScreenPos.y, 100.f * attackTimer, 10.f};
    DrawRectangle(attackCooldownBar.x - getWidth()*0.5f, attackCooldownBar.y + getHeight()*0.5f, attackCooldownBar.width, attackCooldownBar.height, LIGHTGRAY);

    DrawText(TextFormat("Health: %01.01f",health), 55.f, 45.f, 40, WHITE);
    DrawText(TextFormat("Money: %i",moneyCount), 55.f, 80.f, 40, WHITE);
}

void Character::render(){
    BaseCharacter::render();

    Vector2 renderPos{getRenderPos()};

    // draw the sword
    Rectangle source{0.f, 0.f, static_cast<float>(weapon->width) * rightLeft, static_cast<float>(weapon->height)};
    Rectangle dest{renderPos.x + swordVariables.offset.x, renderPos.y + swordVariables.offset.y, weapon->width * scale, weapon->height * scale};
    DrawTexturePro(*weapon, source, dest, swordVariables.origin, swordVariables.rotation, drawColor);

    healthBarColor = drawColor;
    drawColor = WHITE;  // reset draw color
}

void Character::resurrect(){
    setHealth(100.f);
    setDrawColor(WHITE);    // reset color
    setAlive(true);
}

void Character::resetState(){
    setWorldPos(Tex::halfWinSize);
    moneyCount = 0;
    killedEnemies = 0;
    winCondition = false;
    resurrect();
}

void Character::shootProyectile(){
    Vector2 proyectileDirection{Vector2Subtract( GetMousePosition(), getScreenPos() )};
    EntityMng::spawnProyectile(worldPos, proyectileDirection, false);
}