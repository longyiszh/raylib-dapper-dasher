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

    scarfyAnimData.position.x = (windowWidth - scarfyAnimData.textureBoundary.width) / 2;
    scarfyAnimData.position.y = windowHeight - scarfyAnimData.textureBoundary.height;

    // nebula hazard
    Texture2D nebulaTexture = LoadTexture("textures/12_nebula_spritesheet.png");

    AnimationData nebulaAnimData{
        .textureBoundary{
            .x = 0,
            .y = 0,
            .width{nebulaTexture.width / 8.0},
            .height{nebulaTexture.height / 8.0}},
        .position{},
        .animationframe{0},
        .updateTime{1.0 / 12.0},
        .runningTime{0}};

    nebulaAnimData.position.x = windowWidth;
    nebulaAnimData.position.y = windowHeight - nebulaAnimData.textureBoundary.height;

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
        nebulaAnimData.position.x += nebulaVelocityX * deltaTime;

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
        nebulaAnimData.runningTime += deltaTime;

        if (nebulaAnimData.runningTime >= nebulaAnimData.updateTime)
        {
            nebulaAnimData.textureBoundary.x = nebulaAnimData.animationframe * nebulaAnimData.textureBoundary.width;

            // reset animation stopwatch
            nebulaAnimData.runningTime = 0.0;
            // control frame
            nebulaAnimData.animationframe++;
            if (nebulaAnimData.animationframe > 7)
            {
                nebulaAnimData.animationframe = 0;
            }
        }

        // draw nebula
        DrawTextureRec(nebulaTexture, nebulaAnimData.textureBoundary, nebulaAnimData.position, WHITE);

        // draw Scarfy
        DrawTextureRec(scarfyTexture, scarfyAnimData.textureBoundary, scarfyAnimData.position, WHITE);

        EndDrawing();
    }

    // exit cleaning up
    UnloadTexture(scarfyTexture);
    UnloadTexture(nebulaTexture);
    CloseWindow();
}