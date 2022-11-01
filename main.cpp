#include "raylib.h"

int main()
{
    // Window dimension
    const int windowWidth = 800;
    const int windowHeight = 600;
    InitWindow(windowWidth, windowHeight, "Raylib Dapper Dasher");

    // acceleration due to gravity (pixels/(frame^2))
    const int gravity{1};
    const int jumpVelocity{-22};

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

    int velocityY{0};

    bool isInAir{false};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // ground check
        isInAir = scarfyPosition.y < scarfyBoundaryInitialTop;
        if (isInAir)
        {
            velocityY += gravity;
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
        scarfyPosition.y += velocityY;

        DrawTextureRec(scarfyTexture, scarfyTextureBoundary, scarfyPosition, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfyTexture);
    CloseWindow();
}