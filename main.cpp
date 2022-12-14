#include "raylib.h"

struct AnimationData
{
    Rectangle textureBoundary;
    Vector2 position;
    int animationframe;
    float updateTime;
    float runningTime;
};

class BackgroundTextureData
{
public:
    Texture2D texture;
    float scale;
    Vector2 leftInstancePosition;
    Vector2 rightInstancePosition;
    float velocityX;

public:
    float getScaledWidth()
    {
        return texture.width * scale;
    }
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

void moveBackgroundBuildings(BackgroundTextureData &backgroundBuilding, float deltaTime)
{
    backgroundBuilding.leftInstancePosition.x += backgroundBuilding.velocityX * deltaTime;

    // reset left instance posotion when completely out left
    if (backgroundBuilding.leftInstancePosition.x <= -backgroundBuilding.getScaledWidth())
    {
        backgroundBuilding.leftInstancePosition.x = 0;
    }

    backgroundBuilding.rightInstancePosition.x = backgroundBuilding.leftInstancePosition.x + backgroundBuilding.getScaledWidth();
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

    BackgroundTextureData farBuildings{
        .texture{LoadTexture("textures/far-buildings.png")},
        .scale{3.5},
        .leftInstancePosition{0.0, 0.0},
        .rightInstancePosition{0.0, 0.0},
        .velocityX{-100}};

    BackgroundTextureData midBuildings{
        .texture{LoadTexture("textures/back-buildings.png")},
        .scale{3.5},
        .leftInstancePosition{0.0, 30.0},
        .rightInstancePosition{0.0, 30.0},
        .velocityX{-200}};

    BackgroundTextureData nearBuildings{
        .texture{LoadTexture("textures/foreground.png")},
        .scale{2.5},
        .leftInstancePosition{0.0, 200.0},
        .rightInstancePosition{0.0, 200.0},
        .velocityX{-250}};

    int backgroundBuildingsCount{3};
    BackgroundTextureData backgroundBuildings[backgroundBuildingsCount]{
        farBuildings,
        midBuildings,
        nearBuildings};

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

        nebulaAnims[i].position.x = windowDimensions[0] + i * 777;
    }

    // finish line - same point of the last nebula
    float finishLineX{nebulaAnims[nebulaeCount - 1].position.x};

    // start on the ground
    float scarfyBoundaryInitialTop{scarfyAnimData.position.y};

    bool isInAir{false};

    bool collision{false};

    bool godMod{false};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // time since last frame
        float deltaTime{GetFrameTime()};

        BeginDrawing();
        ClearBackground(WHITE);

        // move far buildings
        for (int i = 0; i < backgroundBuildingsCount; i++)
        {
            BackgroundTextureData &currentBuilding{backgroundBuildings[i]};
            moveBackgroundBuildings(currentBuilding, deltaTime);
            // draw far buildings
            DrawTextureEx(currentBuilding.texture, currentBuilding.leftInstancePosition, 0.0, currentBuilding.scale, WHITE);
            DrawTextureEx(currentBuilding.texture, currentBuilding.rightInstancePosition, 0.0, currentBuilding.scale, WHITE);
        }

        if (collision && !godMod)
        {
            DrawText("Game Over!", windowDimensions[0] / 2 - 100, windowDimensions[1] / 2 - 50, 50, RED);
            EndDrawing();
            continue;
        }

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

        finishLineX += nebulaVelocityX * deltaTime;

        // check collision
        Rectangle scarfyCollisionRec{
            .x{scarfyAnimData.position.x},
            .y{scarfyAnimData.position.y},
            .width{scarfyAnimData.textureBoundary.width},
            .height{scarfyAnimData.textureBoundary.height}};

        for (AnimationData anim : nebulaAnims)
        {
            float nebulaPadding{50};
            // create collision box
            Rectangle nebularCollisionRec{
                .x{anim.position.x + nebulaPadding},
                .y{anim.position.y + nebulaPadding},
                .width{anim.textureBoundary.width - 2 * nebulaPadding},
                .height{anim.textureBoundary.height - 2 * nebulaPadding}};
            if (CheckCollisionRecs(scarfyCollisionRec, nebularCollisionRec))
            {
                collision = true;
                break;
            }
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

        if (scarfyAnimData.position.x >= finishLineX)
        {
            DrawText("You Win!", windowDimensions[0] / 2 - 100, windowDimensions[1] / 2 - 50, 50, GREEN);
        }

        EndDrawing();
    }

    // exit cleaning up
    UnloadTexture(scarfyTexture);
    UnloadTexture(nebulaTexture);
    for (int i = 0; i < backgroundBuildingsCount; i++)
    {
        UnloadTexture(backgroundBuildings[i].texture);
    }
    CloseWindow();
}