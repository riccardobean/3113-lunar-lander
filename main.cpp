/**
* Author: Riccardo Bean
* Assignment: Lunar Lander!
* Date due: 10/26/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1600,
              SCREEN_HEIGHT = 850,
              FPS           = 120;

constexpr char    BG_COLOUR[] = "#fae0db";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr float MAX_FUEL = 100.0f;
constexpr float FUEL_CONSUMPTION_RATE = 12.0f;

float gFuel = MAX_FUEL;
Texture2D gFuelTexture;

struct BuildingConfig {
    const char* texture;
    Vector2 size;
    bool landable;
};
const BuildingConfig BUILDING_TEXTURES[] = {
    {"assets/game/building1.png", {100.0f, 260.0f}, true},
    {"assets/game/building2.png", {90.0f, 382.5f}, false},
    {"assets/game/building3.png", {96.0f, 228.0f}, false},
    {"assets/game/building4.png", {75.0f, 300.0f}, false},
    {"assets/game/building5.png", {89.0f, 181.5f}, true},
    {"assets/game/building6.png", {103.5f, 340.5f}, false},
    {"assets/game/building7.png", {91.5f, 249.0f}, false},
    {"assets/game/building8.png", {110.0f, 350.0f}, false},
    {"assets/game/building9.png", {85.0f, 252.0f}, false},
    {"assets/game/building10.png", {90.0f, 351.0f}, true},
    {"assets/game/building11.png", {84.5f, 182.0f}, true},
    {"assets/game/building12.png", {84.5f, 237.0f}, true},
    {"assets/game/building13.png", {105.0f, 211.0f}, true},
    {"assets/game/building14.png", {104.0f, 287.0f}, true},
    {"assets/game/building15.png", {103.0f, 308.0f}, true},
    {"assets/game/building4.png", {70.0f, 340.0f}, false},
    {"assets/game/building2.png", {90.0f, 382.5f}, false},
    {"assets/game/building1.png", {80.0f, 320.0f}, true},
};

constexpr int   NUMBER_OF_BUILDINGS     = sizeof(BUILDING_TEXTURES) / sizeof(BUILDING_TEXTURES[0]);;
constexpr float ACCELERATION_OF_GRAVITY = 1.0f,
                FIXED_TIMESTEP          = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus       = RUNNING;
float     gPreviousTicks   = 0.0f,
          gTimeAccumulator = 0.0f;

Entity *gHelicopter = nullptr;
Entity *gBuildings = nullptr;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
int getFuelFrame();

int getFuelFrame() {
    float fuelRatio = gFuel / MAX_FUEL;

    if (fuelRatio > 0.75f) return 0;
    if (fuelRatio > 0.50f) return 1;
    if (fuelRatio > 0.25f) return 2;
    if (fuelRatio > 0.0f)  return 3;

    return 4;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");

    /*
        ----------- HELICOPTER -----------
    */
    std::vector<int> animationAtlas = { 0,  1,  2,  3,  4,  5,  6, 7 };

    gHelicopter = new Entity(
        { ORIGIN.x, ORIGIN.y - 200.0f }, // position
        { 72.0f, 24.0f },    // scale
        "assets/game/helicopter.png",    // texture file address
        ATLAS,                           // single image or atlas?
        { 1, 8 },                        // atlas dimensions
        animationAtlas                   // actual atlas
    );

    // gProtag->setJumpingPower(400.0f);
    // gProtag->setColliderDimensions({
    //     gProtag->getScale().x / 3.0f,
    //     gProtag->getScale().y / 3.0f
    // });
    gHelicopter->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    /*
        ----------- BUILDINGS -----------
    */

    gBuildings = new Entity[NUMBER_OF_BUILDINGS];

    // // Compute the left-most x coordinate so that the entire row is centred
    // float leftMostX = ORIGIN.x - (NUMBER_OF_TILES * TILE_DIMENSION) / 2.0f;
    float currentX = 0.0f;
    for (int i = 0; i < NUMBER_OF_BUILDINGS; i++) {
        Vector2 dimensions = BUILDING_TEXTURES[i].size;
        gBuildings[i].setTexture(BUILDING_TEXTURES[i].texture);
        gBuildings[i].setScale(dimensions);
        gBuildings[i].setColliderDimensions(dimensions);
        gBuildings[i].setPosition({
            currentX + dimensions.x / 2.0f,
            SCREEN_HEIGHT - dimensions.y / 2.0f + 5.0f
        });
        currentX += dimensions.x;
    }

    // Allocate the tile for the horizontal collision
    // gTiles[NUMBER_OF_TILES - 1].setTexture("assets/game/tile_0000.png");
    // gTiles[NUMBER_OF_TILES - 1].setScale({ TILE_DIMENSION, TILE_DIMENSION });
    // gTiles[NUMBER_OF_TILES - 1].setColliderDimensions({ TILE_DIMENSION, TILE_DIMENSION });
    // gTiles[NUMBER_OF_TILES - 1].setPosition({
    //     ORIGIN.y + TILE_DIMENSION,
    //     ORIGIN.y - TILE_DIMENSION
    // });

    gFuelTexture = LoadTexture("assets/game/fuel.png");

    SetTargetFPS(FPS);
}

void processInput()
{
    // gProtag->resetMovement();

    // if      (IsKeyDown(KEY_A)) gProtag->moveLeft();
    // else if (IsKeyDown(KEY_D)) gProtag->moveRight();

    // if (IsKeyPressed(KEY_W) && gProtag->isCollidingBottom()) gProtag->jump();

    // to avoid faster diagonal speed
    // if (GetLength(gProtag->getMovement()) > 1.0f)
    //     gProtag->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
    if (gFuel > 0.0f) {
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            gHelicopter -> accelerateLeft();
            gFuel -= FUEL_CONSUMPTION_RATE * GetFrameTime();
        }
        else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            gHelicopter -> accelerateRight();
            gFuel -= FUEL_CONSUMPTION_RATE * GetFrameTime();
        }
        else
            gHelicopter -> stopAccelerating();
    } else {
        gFuel = 0.0f;
        gHelicopter -> stopAccelerating();
    }

}

void update()
{
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator; // fold in any leftover from last frame

    if (deltaTime < FIXED_TIMESTEP) // not enough time for a full step yet
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) // consume as many full steps as possible
    {
    //     /*
    //     Note here that we're only checking collisions from the PLAYER'S point
    //     of view. This is, in general, the best practice.
    //     */
    //     gProtag->update(FIXED_TIMESTEP, gTiles, NUMBER_OF_TILES);
        gHelicopter ->update(FIXED_TIMESTEP, gBuildings, NUMBER_OF_BUILDINGS);

        for (int i = 0; i < NUMBER_OF_BUILDINGS; i++)
            gBuildings[i].update(FIXED_TIMESTEP, nullptr, 0);

        deltaTime -= FIXED_TIMESTEP;
    }

    gTimeAccumulator = deltaTime; // save the remainder for next frame
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    gHelicopter -> render();

    for (int i = 0; i < NUMBER_OF_BUILDINGS; i++) gBuildings[i].render();

    // Fuel bar
    float barX = 30.0f;
    float barY = 30.0f;
    float barWidth = 250.0f;
    float barHeight = 24.0f;

    float fuelRatio = gFuel / MAX_FUEL;

    int fuel = getFuelFrame();

    Rectangle source = {
        fuel * 64.0f,
        0.0f,
        64.0f,
        16.0f
    };

    Rectangle destination = {
        30.0f,
        30.0f,
        192.0f,
        48.0f
    };

    DrawTexturePro(gFuelTexture, source, destination, {0.0f, 0.0f}, 0.0f, WHITE);

    EndDrawing();
}

void shutdown()
{
    // Unload the textures while the window (GL context) still exists
    delete gHelicopter;
    delete[] gBuildings;
    UnloadTexture(gFuelTexture);
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}