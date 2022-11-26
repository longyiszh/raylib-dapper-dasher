#include "raylib.h"

struct AnimationData
{
    Rectangle textureBoundary;
    Vector2 position;
    int animationframe;
    float updateTime;
    float runningTime;
};

bool checkInAir(AnimationData objectAnimData, int groundLevelRecTopY)
{
    return objectAnimData.position.y < groundLevelRecTopY;
}

AnimationData updateAnimationData(AnimationData nextAnimData, float currentDeltaTime, int animFrameCount, bool pauseInAir, bool inAir)
{
    if (!pauseInAir || !inAir)
    {
        nextAnimData.runningTime += currentDeltaTime;
    } // else, pause the anim play for this object

    if (nextAnimData.runningTime >= nextAnimData.updateTime)
    {
        nextAnimData.textureBoundary.x = nextAnimData.animationframe * nextAnimData.textureBoundary.width;

        // reset animation stopwatch
        nextAnimData.runningTime = 0.0;
        // control frame
        nextAnimData.animationframe++;
        if (nextAnimData.animationframe > animFrameCount - 1)
        {
            nextAnimData.animationframe = 0;
        }
    }
    return nextAnimData;
}

int main()
{
    // Window dimension
    // [width, height]
    int windowDimensions[]{800, 600};
    InitWindow(windowDimensions[0], windowDimensions[1], "Raylib Dapper Dasher");

    // acceleration due to gravity (pixels/(s^2))
    const int gravity{1000};
    // jump velocity (pixels/second)
    const int jumpVelocity{-600};

    float scarfyVelocityY{0};

    float nebulaVelocityX{-600};

    // scarfy
    Texture2D scarfyTexture = LoadTexture("textures/scarfy.png");
    AnimationData scarfyAnimData{
        .textureBoundary{
            .x = 0,
            .y = 0,
            .width{scarfyTexture.width / 6},
            .height{scarfyTexture.height}},
        .position{},
        .animationframe{0},
        .updateTime{1.0 / 12.0},
        .runningTime{0}};

    scarfyAnimData.position.x = (windowDimensions[0] - scarfyAnimData.textureBoundary.width) / 2;
    scarfyAnimData.position.y = windowDimensions[1] - scarfyAnimData.textureBoundary.height;

    // nebula hazard
    Texture2D nebulaTexture = LoadTexture("textures/12_nebula_spritesheet.png");

    const int nebulaeCount{6};
    const float nebulaTextureBoundaryWidth{nebulaTexture.width / 8.0};
    const float nebulaTextureBoundaryHeight{nebulaTexture.height / 8.0};

    AnimationData nebulaAnims[nebulaeCount]{};

    for (int i = 0; i < nebulaeCount; i++)
    {
        nebulaAnims[i] = {
            .textureBoundary{
                .x = 0,
                .y = 0,
                .width{nebulaTextureBoundaryWidth},
                .height{nebulaTextureBoundaryHeight}},
            .position{
                .y = windowDimensions[1] - nebulaTextureBoundaryHeight},
            .animationframe{0},
            .updateTime{1.0 / 12.0},
            .runningTime{0}};

        nebulaAnims[i].position.x = windowDimensions[0] + i * 300;
    }

    // start on the ground
    float scarfyBoundaryInitialTop{scarfyAnimData.position.y};

    bool isInAir{false};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // time since last frame
        float deltaTime{GetFrameTime()};

        BeginDrawing();
        ClearBackground(WHITE);

        // ground check
        isInAir = checkInAir(scarfyAnimData, scarfyBoundaryInitialTop);
        if (isInAir)
        {
            scarfyVelocityY += gravity * deltaTime;
        }
        else
        {
            // on the ground
            scarfyVelocityY = 0;
        }

        // Game logics
        if (IsKeyPressed(KEY_SPACE))
        {
            if (!isInAir)
            {
                scarfyVelocityY += jumpVelocity;
            }
        }

        // update position
        scarfyAnimData.position.y += scarfyVelocityY * deltaTime;

        for (int i = 0; i < nebulaeCount; i++)
        {
            nebulaAnims[i].position.x += nebulaVelocityX * deltaTime;
        }

        // => update animation frame for Scarfy
        scarfyAnimData = updateAnimationData(
            scarfyAnimData,
            deltaTime,
            6,
            true,
            isInAir);

        // => update animation frame for nebulae
        for (int i = 0; i < nebulaeCount; i++)
        {
            nebulaAnims[i] = updateAnimationData(
                nebulaAnims[i],
                deltaTime,
                8,
                false,
                false);
        }

        // draw nebulae
        for (int i = 0; i < nebulaeCount; i++)
        {
            DrawTextureRec(nebulaTexture, nebulaAnims[i].textureBoundary, nebulaAnims[i].position, WHITE);
        }

        // draw Scarfy
        DrawTextureRec(scarfyTexture, scarfyAnimData.textureBoundary, scarfyAnimData.position, WHITE);

        EndDrawing();
    }

    // exit cleaning up
    UnloadTexture(scarfyTexture);
    UnloadTexture(nebulaTexture);
    CloseWindow();
}