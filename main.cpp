#include "raylib.h"

int main()
{
    // Window dimension
    const int windowWidth = 800;
    const int windowHeight = 600;
    InitWindow(windowWidth, windowHeight, "Raylib Dapper Dasher");

    // acceleration due to gravity (pixels/(s^2))
    const int gravity{1000};
    // jump velocity (pixels/second)
    const int jumpVelocity{-600};

    float scarfyVelocityY{0};

    float nebulaVelocityX{-600};

    // scarfy
    Texture2D scarfyTexture = LoadTexture("textures/scarfy.png");
    Rectangle scarfyTextureBoundary;
    scarfyTextureBoundary.width = scarfyTexture.width / 6;
    scarfyTextureBoundary.height = scarfyTexture.height;
    scarfyTextureBoundary.x = 0;
    scarfyTextureBoundary.y = 0;
    // init position: center of window
    Vector2 scarfyPosition;
    scarfyPosition.x = (windowWidth - scarfyTextureBoundary.width) / 2;
    scarfyPosition.y = windowHeight - scarfyTextureBoundary.height;

    // nebula hazard
    Texture2D nebulaTexture = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebulaTextureBoundary = {
        .x = 0,
        .y = 0,
        .width = nebulaTexture.width / 8.0,
        .height = nebulaTexture.height / 8.0};
    Vector2 nebulaPosition = {
        .x = windowWidth,
        .y = windowHeight - nebulaTextureBoundary.height,
    };

    // start on the ground
    float scarfyBoundaryInitialTop{scarfyPosition.y};

    // animation frames
    int scarfyAnimationFrame{0};
    // amount of time before we update the animation frame
    const float scarfyAnimationUpdateTime{1.0 / 12.0};
    float scarfyAnimationRunningTime{0};

    int nebulaAnimationFrame{0};
    const float nebulaAnimationUpdateTime{1.0 / 12.0};
    float nebulaAnimationRunningTime{0};

    bool isInAir{false};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // time since last frame
        float deltaTime{GetFrameTime()};

        BeginDrawing();
        ClearBackground(WHITE);

        // ground check
        isInAir = scarfyPosition.y < scarfyBoundaryInitialTop;
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
        scarfyPosition.y += scarfyVelocityY * deltaTime;
        nebulaPosition.x += nebulaVelocityX * deltaTime;

        // update animation frame
        if (!isInAir)
        {
            // => pause Scarfy while in air
            scarfyAnimationRunningTime += deltaTime;
        }

        if (scarfyAnimationRunningTime >= scarfyAnimationUpdateTime)
        {
            scarfyTextureBoundary.x = scarfyAnimationFrame * scarfyTextureBoundary.width;

            // reset time meter
            scarfyAnimationRunningTime = 0.0;
            // control frame
            scarfyAnimationFrame++;
            if (scarfyAnimationFrame > 5)
            {
                scarfyAnimationFrame = 0;
            }
        }

        // => nebula
        nebulaAnimationRunningTime += deltaTime;

        if (nebulaAnimationRunningTime >= nebulaAnimationUpdateTime)
        {
            nebulaTextureBoundary.x = nebulaAnimationFrame * nebulaTextureBoundary.width;

            // reset time meter
            nebulaAnimationRunningTime = 0.0;
            // control frame
            nebulaAnimationFrame++;
            if (nebulaAnimationFrame > 7)
            {
                nebulaAnimationFrame = 0;
            }
        }

        // draw nebula
        DrawTextureRec(nebulaTexture, nebulaTextureBoundary, nebulaPosition, WHITE);

        // draw Scarfy
        DrawTextureRec(scarfyTexture, scarfyTextureBoundary, scarfyPosition, WHITE);

        EndDrawing();
    }

    // exit cleaning up
    UnloadTexture(scarfyTexture);
    UnloadTexture(nebulaTexture);
    CloseWindow();
}