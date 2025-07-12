#include "Character.h"
#include "raymath.h"

Character::Character(int winWidth, int winHeight) :
    windowWidth(winWidth),
    windowHeight(winHeight)
{
    width = texture->width / maxFrames;
    height = texture->height;

    speed = 10.f;    // default speed
}

Vector2 Character::getScreenPos(){
    return Vector2{
        static_cast<float>(windowWidth)/2.0f - (scale * (0.5f * width)) ,
        static_cast<float>(windowHeight)/2.0f - (scale * (0.5f * height))
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
    
    drawColor = WHITE;  // reset draw color
    invul = false;  // reset invulnerability

    Vector2 origin{};
    Vector2 offset{};
    isAttacking = false;
    float rotation{};

    // attack & defend
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_SPACE)) { isAttacking = true; rotation = 35.f; }
    if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON) || IsKeyDown(KEY_V)) { rotation = -30.f; drawColor = YELLOW; invul = true; }

    if(rightLeft > 0.f){
        origin = {0.f, weapon->height * scale};
        offset = {70.f, 110.f};
        weaponCollisionRec = {
            getScreenPos().x + offset.x,
            getScreenPos().y + offset.y - weapon->height * scale,
            weapon->width * scale,
            weapon->height * scale
        };
    } else{
        origin = {weapon->width * scale, weapon->height * scale};
        offset = {50.f, 110.f};
        weaponCollisionRec = {
            getScreenPos().x + offset.x - weapon->width * scale,
            getScreenPos().y + offset.y - weapon->height * scale,
            weapon->width * scale,
            weapon->height * scale
        };
        rotation *= -1;
    }

    // draw the sword
    Rectangle source{0.f, 0.f, static_cast<float>(weapon->width) * rightLeft, static_cast<float>(weapon->height)};
    Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon->width * scale, weapon->height * scale};
    DrawTexturePro(*weapon, source, dest, origin, rotation, drawColor);

    /*DrawRectangleLines(
        weaponCollisionRec.x,
        weaponCollisionRec.y,
        weaponCollisionRec.width,
        weaponCollisionRec.height,
        RED
    );*/

    return true;
}


void Character::deathSequence(){
    setAlive(false);
}

void Character::addHealth( float healthAdd ){
    BaseCharacter::addHealth( healthAdd );
    setDrawColor(GREEN);
}