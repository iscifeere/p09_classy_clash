#include "Character.h"
#include "raymath.h"

Character::Character(Vector2 pos) :
    windowWidth(Tex::winSize[0]),
    windowHeight(Tex::winSize[1])
{
    frameWidth = texture->width / maxFrames;
    frameHeight = texture->height;
    scale = 8.0f;

    speed = 10.f;    // default speed

    worldPos = pos;
}

// Vector2 Character::getWorldPos(){
//     return Vector2Add( BaseCharacter::getWorldPos(),
//     Vector2{static_cast<float>(windowWidth)*0.5f,static_cast<float>(windowHeight)*0.5f}
//     );
// }

// Vector2 Character::getWorldPos(){
//     return Vector2Add( BaseCharacter::getWorldPos(),
//     Vector2{width*scale*0.5f,height*scale*0.5f}
//     );
// }

Vector2 Character::getScreenPos(){
    return Vector2{
        Tex::halfWinSize.x - (scale * (0.5f * frameWidth)) ,
        Tex::halfWinSize.y - (scale * (0.5f * frameHeight))
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

    // attack & defend
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_SPACE)) { isAttacking = true; swordVariables.rotation = 35.f; }
    if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON) || IsKeyDown(KEY_V)) { swordVariables.rotation = -30.f; drawColor = YELLOW; invul = true; }

    if(rightLeft > 0.f){
        swordVariables.origin = {0.f, weapon->height * scale};
        swordVariables.offset = {70.f, 110.f};
        weaponCollisionRec = {
            getScreenPos().x + swordVariables.offset.x,
            getScreenPos().y + swordVariables.offset.y - weapon->height * scale,
            weapon->width * scale,
            weapon->height * scale
        };
    } else{
        swordVariables.origin = {weapon->width * scale, weapon->height * scale};
        swordVariables.offset = {50.f, 110.f};
        weaponCollisionRec = {
            getScreenPos().x + swordVariables.offset.x - weapon->width * scale,
            getScreenPos().y + swordVariables.offset.y - weapon->height * scale,
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

void Character::showDebugData(){
    // DrawRectangleLines(getWorldPos().x, getWorldPos().y, width*scale, height*scale, YELLOW); [SHOOTING AIM IDEA!!!]
    DrawRectangleLines(getScreenPos().x, getScreenPos().y, frameWidth*scale, frameHeight*scale, YELLOW);
    DrawText(TextFormat("X: %01.01f",getWorldPos().x), 55.f, 125.f, 30, WHITE);
    DrawText(TextFormat("Y: %01.01f",getWorldPos().y), 55.f, 155.f, 30, WHITE);
}

void Character::showStats(){
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

    DrawText(TextFormat("Health: %01.01f",health), 55.f, 45.f, 40, WHITE);
    DrawText(TextFormat("Money: %i",moneyCount), 55.f, 80.f, 40, WHITE);
}

void Character::render(){
    BaseCharacter::render();

    // draw the sword
    Rectangle source{0.f, 0.f, static_cast<float>(weapon->width) * rightLeft, static_cast<float>(weapon->height)};
    Rectangle dest{getScreenPos().x + swordVariables.offset.x, getScreenPos().y + swordVariables.offset.y, weapon->width * scale, weapon->height * scale};
    DrawTexturePro(*weapon, source, dest, swordVariables.origin, swordVariables.rotation, drawColor);

    healthBarColor = drawColor;
    drawColor = WHITE;  // reset draw color
}