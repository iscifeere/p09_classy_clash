#include "BaseCharacter.h"
#include "raymath.h"

BaseCharacter::BaseCharacter(){

}

void BaseCharacter::undoMovementX(){
    worldPos.x -= movement.x;
    drawColor = BLACK;
}
void BaseCharacter::undoMovementY(){
    worldPos.y -= movement.y;
    drawColor = BLACK;
}

Rectangle BaseCharacter::getCollisionRec(){
    return Rectangle{
        getScreenPos().x,
        getScreenPos().y,
        width * scale,
        height * scale
    };
}

bool BaseCharacter::tick(float deltaTime){
    // save previous world position
    worldPosLastFrame = worldPos;       // unused

    // update animation frame
    runningTime += deltaTime;
    if(runningTime >= updateTime){
        frame++;
        runningTime = 0.f;
        if(frame >= maxFrames) frame = 0;
    }

    if(Vector2Length(velocity) != 0.0){
        // set worldPos = worldPos + normalized direction
        texture = run;
        velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
        movement = Vector2Scale(Vector2Normalize(velocity), speed);
        worldPos = Vector2Add(worldPos, movement);
        
        // check map bounds
        if(getWorldPos().x < 0.f ||
        getWorldPos().x + Tex::winSize[0] > Tex::texture_map.width * Tex::MAP_SCALE )
        {
            undoMovementX();
        }
        if(getWorldPos().y < 0.f ||
        getWorldPos().y + Tex::winSize[1] > Tex::texture_map.height * Tex::MAP_SCALE )
        {
            undoMovementY();
        }

    } else texture = idle;
    // velocity = {};      // reset velocity

    // draw the character
    Rectangle source{ (float)frame * width , 0.f , rightLeft * width , height };
    Rectangle dest{ getScreenPos().x , getScreenPos().y , scale * width , scale * height };
    DrawTexturePro(*texture, source, dest, (Vector2){0.f,0.f}, 0.f, drawColor);

    return true;
}

void BaseCharacter::takeDamage(float damage){
    if(!invul){
        health -= damage;
        if(health <= 0.f) deathSequence();
        drawColor = RED;
        invul = true;
    }
}