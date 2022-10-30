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

    // rectangle dimensions
    const int rectangleWidth{50};
    const int rectangleHeight{80};

    // start on the ground
    int rectangleTop{windowHeight - rectangleHeight};
    int rectanglePosY{rectangleTop};
    int velocityY{0};

    bool isInAir{false};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // ground check
        isInAir = rectanglePosY < rectangleTop;
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
        rectanglePosY += velocityY;

        DrawRectangle(windowWidth / 2, rectanglePosY, rectangleWidth, rectangleHeight, BLUE);

        EndDrawing();
    }
    CloseWindow();
}