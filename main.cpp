#include "raylib.h"

struct AnimationData
{
    Rectangle textureBoundary;
    Vector2 position;
    int animationframe;
    float updateTime;
    float runningTime;
};

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
        isInAir = scarfyAnimData.position.y < scarfyBoundaryInitialTop;
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

        // update animation frame
        if (!isInAir)
        {
            // => pause Scarfy while in air
            scarfyAnimData.runningTime += deltaTime;
        }

        if (scarfyAnimData.runningTime >= scarfyAnimData.updateTime)
        {
            scarfyAnimData.textureBoundary.x = scarfyAnimData.animationframe * scarfyAnimData.textureBoundary.width;

            // reset animation stopwatch
            scarfyAnimData.runningTime = 0.0;
            // control frame
            scarfyAnimData.animationframe++;
            if (scarfyAnimData.animationframe > 5)
            {
                scarfyAnimData.animationframe = 0;
            }
        }

        // => nebula

        for (int i = 0; i < nebulaeCount; i++)
        {
            nebulaAnims[i].runningTime += deltaTime;

            if (nebulaAnims[i].runningTime >= nebulaAnims[i].updateTime)
            {
                nebulaAnims[i].textureBoundary.x = nebulaAnims[i].animationframe * nebulaAnims[i].textureBoundary.width;

                // reset animation stopwatch
                nebulaAnims[i].runningTime = 0.0;
                // control frame
                nebulaAnims[i].animationframe++;
                if (nebulaAnims[i].animationframe > 7)
                {
                    nebulaAnims[i].animationframe = 0;
                }
            }
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