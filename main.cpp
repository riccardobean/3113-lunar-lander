/**
* Author: Riccardo Bean
* Assignment: Lunar Lander!
* Date due: 10/30/2026
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
int gBuildingNum = -1;

enum GameState { HOME, PLAYING, GAME_WON, GAME_LOST };
GameState gGameState = HOME;


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
    {"assets/game/building1.png", {80.0f, 320.0f}, false},
};

constexpr int   NUMBER_OF_BUILDINGS     = sizeof(BUILDING_TEXTURES) / sizeof(BUILDING_TEXTURES[0]);
constexpr float ACCELERATION_OF_GRAVITY = 5.0f,
                FIXED_TIMESTEP          = 1.0f / 60.0f;
constexpr Vector2 HELICOPTER_START = {ORIGIN.x, ORIGIN.y - 300.0f};

// Global Variables
AppStatus gAppStatus       = RUNNING;
float     gPreviousTicks   = 0.0f,
          gTimeAccumulator = 0.0f;

Entity *gHelicopter = nullptr;
Entity *gBuildings = nullptr;
Entity *gPlane = nullptr;
float gColorTimer = 0.0f;
bool gFlashRed = false;
Texture2D gBG;
Texture2D gWelcomeScreen;
Texture2D gWinScreen;
Texture2D gLoseScreen;

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

void resetGame() {
    gHelicopter -> setPosition(HELICOPTER_START);
    gHelicopter->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    gHelicopter->resetVelocity();
    float planey = GetRandomValue(ORIGIN.y - 250, ORIGIN.y - 100);
    gPlane -> setPosition({-50, planey});
    gPlane->setSpeed(150);
    gPlane->setMovement({1.0f, 0.0f});
    gFuel = MAX_FUEL;
    do {
        gBuildingNum = GetRandomValue(0, NUMBER_OF_BUILDINGS - 1);
    } while (!gBuildings[gBuildingNum].isLandable());
    gGameState = PLAYING;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");

    /*
        ----------- HELICOPTER -----------
    */
    std::vector<int> animationAtlas = { 0,  1,  2,  3,  4,  5,  6, 7 };

    gHelicopter = new Entity(
        HELICOPTER_START, // position
        { 72.0f, 24.0f },    // scale
        "assets/game/helicopter.png",    // texture file address
        ATLAS,                           // single image or atlas?
        { 1, 8 },                        // atlas dimensions
        animationAtlas                   // actual atlas
    );


    /*
        ----------- BUILDINGS -----------
    */

    gBuildings = new Entity[NUMBER_OF_BUILDINGS];

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
        gBuildings[i].setLandable(BUILDING_TEXTURES[i].landable);
        currentX += dimensions.x;
    }

    /*
        ----------- PLANE/ENEMY -----------
    */

    float planey = GetRandomValue(ORIGIN.y - 250, ORIGIN.y - 100);
    gPlane = new Entity(
        { -200, planey}, 
        { 50.0f, 33.0f },    
        "assets/game/plane1.png"   
    );


    gFuelTexture = LoadTexture("assets/game/fuel.png");
    gBG = LoadTexture("assets/game/BG.png");
    gWelcomeScreen = LoadTexture("assets/game/welcome.png");
    gWinScreen = LoadTexture("assets/game/win.png");
    gLoseScreen = LoadTexture("assets/game/lose.png");


    SetTargetFPS(FPS);
}

void processInput()
{

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
    if (gGameState == HOME && IsKeyPressed(KEY_SPACE)) resetGame();
    if (gGameState == GAME_LOST || gGameState == GAME_WON) {
        if (IsKeyPressed(KEY_R)) resetGame();
        if (IsKeyPressed(KEY_M)) gGameState = HOME;
    }
    if (gGameState == PLAYING) {
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

}

void update()
{
    
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;
    if (gGameState == PLAYING) {

        deltaTime += gTimeAccumulator; // fold in any leftover from last frame

        if (deltaTime < FIXED_TIMESTEP) // not enough time for a full step yet
        {
            gTimeAccumulator = deltaTime;
            return;
        }

        while (deltaTime >= FIXED_TIMESTEP) // consume as many full steps as possible
        {
            gHelicopter ->update(FIXED_TIMESTEP, gBuildings, NUMBER_OF_BUILDINGS);

            Vector2 position = gHelicopter->getPosition();
            Vector2 dimensions = gHelicopter->getColliderDimensions();

            // Left edge of the screen
            if (position.x - dimensions.x / 2.0f < 0.0f)
                gGameState = GAME_LOST;

            // Right edge of the screen
            if (position.x + dimensions.x / 2.0f > SCREEN_WIDTH)
                gGameState = GAME_LOST;
            gPlane -> update(FIXED_TIMESTEP, gBuildings, NUMBER_OF_BUILDINGS);

            if (gHelicopter->isCollidingX(gPlane) && gHelicopter->isCollidingY(gPlane)) gGameState = GAME_LOST;

            gColorTimer += FIXED_TIMESTEP;

            if (gColorTimer >= 0.25f) {
                gColorTimer = 0.0f;
                gFlashRed = !gFlashRed;
            }


            for (int i = 0; i < NUMBER_OF_BUILDINGS; i++) {
                if (gHelicopter -> isCollidingX(&gBuildings[i]) && gHelicopter -> isCollidingY(&gBuildings[i])) {
                    if (i == gBuildingNum && gHelicopter -> isCollidingBottom(&gBuildings[i])) gGameState = GAME_WON;
                    else gGameState = GAME_LOST;
                }
            }
            deltaTime -= FIXED_TIMESTEP;
        }
        gTimeAccumulator = deltaTime; // save the remainder for next frame
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    Rectangle background = {
        0.0f,
        0.0f,
        (float)gBG.width,
        (float)gBG.height
    };

    Rectangle backgroundDestination = {
        0.0f,
        0.0f,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

    DrawTexturePro(gBG, background, backgroundDestination, {0.0f, 0.0f}, 0.0f, WHITE);

    Rectangle screenSource = {
        0.0f,
        0.0f,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

    Rectangle screenDestination = {
        0.0f,
        0.0f,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

    if (gGameState == GAME_LOST) {
        DrawTexturePro(gLoseScreen, screenSource, screenDestination, {0.0f, 0.0f}, 0.0f, WHITE);
    }
    else if (gGameState == GAME_WON) {
        DrawTexturePro(gWinScreen, screenSource, screenDestination, {0.0f, 0.0f}, 0.0f, WHITE);
    }
    else if (gGameState == HOME) {
        DrawTexturePro(gWelcomeScreen, screenSource, screenDestination, {0.0f, 0.0f}, 0.0f, WHITE);
    }
    else {
        gHelicopter -> render();

        for (int i = 0; i < NUMBER_OF_BUILDINGS; i++) {
            gBuildings[i].render();
            if (i == gBuildingNum)
                gBuildings[i].displayCollider(gFlashRed ? RED : WHITE);
        }

        gPlane -> render();

        // Fuel bar

        float fuelRatio = gFuel / MAX_FUEL;

        int fuel = getFuelFrame();

        Rectangle fuelSource = {
            fuel * 64.0f,
            0.0f,
            64.0f,
            16.0f
        };

        Rectangle fuelDestination = {
            30.0f,
            30.0f,
            192.0f,
            48.0f
        };

        DrawTexturePro(gFuelTexture, fuelSource, fuelDestination, {0.0f, 0.0f}, 0.0f, WHITE);
    }

    EndDrawing();
}

void shutdown()
{
    // Unload the textures while the window (GL context) still exists
    delete gHelicopter;
    delete[] gBuildings;
    delete gPlane;
    UnloadTexture(gFuelTexture);
    UnloadTexture(gBG);
    UnloadTexture(gWinScreen);
    UnloadTexture(gLoseScreen);
    UnloadTexture(gWelcomeScreen);
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