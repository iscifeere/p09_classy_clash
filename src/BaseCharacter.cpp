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
    Vector2 screenPos{getScreenPos()};
    float scaledWidth = frameWidth*scale;
    float scaledHeight = frameHeight*scale;

    return Rectangle{
        // displacement
        screenPos.x + scaledWidth * 0.2f,
        screenPos.y + scaledHeight * 0.75f,

        // scaling
        scaledWidth * 0.6f,
        scaledHeight * 0.25f
    };
}

Rectangle BaseCharacter::getHurtRec(){
    Vector2 screenPos{getScreenPos()};

    return Rectangle{
        screenPos.x,
        screenPos.y,
        frameWidth*scale,
        frameHeight*scale
    };
}
// Rectangle BaseCharacter::getHurtRec(){
//     Vector2 screenPos{getScreenPos()};
//     float scaledWidth = frameWidth*scale;
//     float scaledHeight = frameHeight*scale;

//     return Rectangle{
//         // displacement
//         screenPos.x + scaledWidth * 0.125f,
//         screenPos.y + scaledHeight * 0.166f,

//         // scaling
//         scaledWidth * 0.75f,
//         scaledHeight * 0.833f
//     };
// }

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

        if(
            getWorldPos().x < Tex::halfWinSize.x ||
            getWorldPos().x + frameWidth*scale > static_cast<float>(Tex::texture_map.width) * Tex::MAP_SCALE - Tex::halfWinSize.x )
        {
            undoMovementX();
        }
        if(
            getWorldPos().y < Tex::halfWinSize.y ||
            getWorldPos().y + frameHeight*scale > static_cast<float>(Tex::texture_map.height) * Tex::MAP_SCALE - Tex::halfWinSize.y )
        {
            undoMovementY();
        }

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

    // draw character
    // Rectangle source{ static_cast<float>(frame) * frameWidth, static_cast<float>(frameRow) * frameHeight, rightLeft * frameWidth, frameHeight };
    Rectangle source{ static_cast<float>(frame) * frameWidth, 0.f, rightLeft * frameWidth, frameHeight };
    Rectangle dest{ getScreenPos().x , getScreenPos().y , scale * frameWidth , scale * frameHeight };
    DrawTexturePro(*texture, source, dest, (Vector2){0.f,0.f}, 0.f, drawColor);
}