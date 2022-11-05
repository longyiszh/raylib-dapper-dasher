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

    // start on the ground
    float scarfyBoundaryInitialTop{scarfyPosition.y};

    float velocityY{0};

    // animation frame
    int animationFrame{0};
    // amount of time before we update the animation frame
    const float animationUpdateTime{1.0 / 12.0};
    float animationRunningTime{0};

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
            velocityY += gravity * deltaTime;
        }
        else
        {
            // on the ground
            velocityY = 0;
        }

        // Game logics
        if (IsKeyPressed(KEY_SPACE))
        {
            if (!isInAir)
            {
                velocityY += jumpVelocity;
            }
        }

        // update position
        scarfyPosition.y += velocityY * deltaTime;

        // update animation frame
        animationRunningTime += deltaTime;
        if (animationRunningTime >= animationUpdateTime)
        {
            scarfyTextureBoundary.x = animationFrame * scarfyTextureBoundary.width;

            // reset time meter
            animationRunningTime = 0.0;
            // control frame
            animationFrame++;
            if (animationFrame > 5)
            {
                animationFrame = 0;
            }
        }

        DrawTextureRec(scarfyTexture, scarfyTextureBoundary, scarfyPosition, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfyTexture);
    CloseWindow();
}