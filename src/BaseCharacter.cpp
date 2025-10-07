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

bool BaseCharacter::tick(float deltaTime){
    // save previous world position
    worldPosLastFrame = worldPos;       // unused

    // update animation frame
    runningTime += deltaTime;

    if(Vector2Length(velocity) != 0.0){
        // set worldPos = worldPos + normalized direction
        texture = run;
        maxFrames = maxFramesRun;
        velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
        movement = Vector2Scale(Vector2Normalize(velocity), speed);
        worldPos = Vector2Add(worldPos, movement);

        checkMapBoundsCollision();

    } else {texture = idle; maxFrames = maxFramesIdle;}

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

void BaseCharacter::render(){
    // update animation frame
    if(runningTime >= updateTime){
        frame++;
        runningTime = 0.f;
        if(frame >= maxFrames){frame = 0; frameRow++;}
        if(frameRow >= maxFrameRows) frameRow = 0;
    }

    Vector2 renderPos{getRenderPos()};

    // draw character
    // Rectangle source{ static_cast<float>(frame) * frameWidth, static_cast<float>(frameRow) * frameHeight, rightLeft * frameWidth, frameHeight };
    Rectangle source{ static_cast<float>(frame) * frameWidth, 0.f, rightLeft * frameWidth, frameHeight };
    Rectangle dest{ renderPos.x , renderPos.y , scale * frameWidth , scale * frameHeight };
    DrawTexturePro(*texture, source, dest, (Vector2){0.f,0.f}, 0.f, drawColor);
}

void BaseCharacter::checkMapBoundsCollision(){  // necesita optimización/reestructuración
    float entityWidth{getWidth()};
    float entityHeight{getHeight()};

    if(
        worldPos.x + entityWidth * 0.2f < Tex::halfWinSize.x ||
        worldPos.x + entityWidth * 0.2f + entityWidth * 0.6f > static_cast<float>(Tex::texture_map.width) * Tex::MAP_SCALE - Tex::halfWinSize.x )
    {
        undoMovementX();
    }
    if(
        worldPos.y + entityHeight * 0.75f < Tex::halfWinSize.y ||
        worldPos.y + entityHeight * 0.75f + entityHeight * 0.25f > static_cast<float>(Tex::texture_map.height) * Tex::MAP_SCALE - Tex::halfWinSize.y )
    {
        undoMovementY();
    }
}