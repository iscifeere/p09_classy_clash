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

    // draw health bar
    Rectangle healthBar{50.f, 45.f, 4.f * health, 40.f};
    // Color healthBarColor{GREEN};
    // if( drawColor.r != 255 ||
    //     drawColor.g != 255 ||
    //     drawColor.b != 255 ||
    //     drawColor.a != 255) healthBarColor = drawColor;
    if(health <= 100.f)
    {
        Color healthBarColor{GREEN};
        if( drawColor.r != 255 ||
            drawColor.g != 255 ||
            drawColor.b != 255 ||
            drawColor.a != 255) healthBarColor = drawColor;

        DrawRectangle(healthBar.x, healthBar.y, 400.f, healthBar.height, BLACK);
        DrawRectangle(healthBar.x, healthBar.y, healthBar.width, healthBar.height, healthBarColor);
    }
    else if(health <= 300.f)
    {
        Color healthBarColor{BLUE};
        if( drawColor.r != 255 ||
            drawColor.g != 255 ||
            drawColor.b != 255 ||
            drawColor.a != 255) healthBarColor = drawColor;
        
        DrawRectangle(healthBar.x, healthBar.y, 400.f, healthBar.height, GREEN);
        DrawRectangle(healthBar.x, healthBar.y, ( healthBar.width - (4.f * 100.f) ) * 0.5f, healthBar.height, healthBarColor);
    } else{
        Color healthBarColor{BLUE};
        if( drawColor.r != 255 ||
            drawColor.g != 255 ||
            drawColor.b != 255 ||
            drawColor.a != 255) healthBarColor = drawColor;

        DrawRectangle(healthBar.x, healthBar.y, 400.f, healthBar.height, healthBarColor);
    }
    
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

    // // draw health bar
    // Rectangle healthBar{55.f, 45.f, 4.f * health, 40.f};
    // DrawRectangle(healthBar.x, healthBar.y, 400.f, healthBar.height, BLACK);
    // DrawRectangle(healthBar.x, healthBar.y, healthBar.width, healthBar.height, GREEN);


    return true;
}


void Character::deathSequence(){
    setAlive(false);
}

void Character::addHealth( float healthAdd ){
    BaseCharacter::addHealth( healthAdd );
    setDrawColor(PINK);
}