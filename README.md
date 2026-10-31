# Project 3: Lunar Lander!

## CS-UY 3113

This project is a 2D landing game built in C++ using Raylib by **Riccardo Bean** as part of CS-UY 3113 coursework. The game is themed around an emergency helicopter landing in a city, where the player must navigate around obstacles and safely land on a designated skyscraper.

## Game Features

* Single-player gameplay controlling a helicopter during an emergency landing
* Gravity-based movement and acceleration
* Limited fuel supply that decreases when horizontal movement controls are used
* A fuel indicator that updates as fuel is consumed
* A randomly selected landable skyscraper that serves as the landing target
* Flashing collision indicator to identify the designated landing location
* A moving airplane that acts as an additional obstacle
* Collision detection between the helicopter, airplane, and skyscrapers
* Screen boundary detection
* Fixed-timestep updates for consistent physics and movement
* Animated helicopter sprite
* Welcome screen, successful landing screen, and crash screen

## Controls

* `A` / `←` — Accelerate the helicopter to the left
* `D` / `→` — Accelerate the helicopter to the right
* `Space` — Start the game from the welcome screen
* `R` — Restart the game after a successful landing or crash
* `M` — Return to the main menu from the game-over screen
* `Q` — Quit the game

## Landing Mechanics

* The helicopter is affected by gravity and must be controlled using horizontal acceleration.
* Horizontal movement consumes fuel, so the player must manage the available fuel carefully.
* The player must land on the designated skyscraper to win.
* Landing on any other skyscraper or colliding with the airplane results in a crash.
* Touching the left or right screen boundaries also results in a crash.
* The target skyscraper is randomly selected at the beginning of each game.

## Project Structure

* `main.cpp` — Game initialization, input handling, game states, gameplay logic, collision detection, and rendering
* `Entity.h` / `Entity.cpp` — Entity class implementation, movement, acceleration, animation, rendering, and collision detection
* `assets/game/` — Textures used for the helicopter, airplane, skyscrapers, background, fuel indicator, and game screens
* `CS3113/` — Graphics and game support code
* `makefile` — Builds the application

## How to Run It

From the project directory, build and run the program with:

```bash
make run
```

If the build succeeds, the application window should open and display the welcome screen.

## Credits

* **Background and Airplane:** [Free Plane Sprite](https://opengameart.org/content/free-plane-sprite) from OpenGameArt.org
* **Helicopter:** [Free Plane Sprite](https://opengameart.org/content/free-plane-sprite) from OpenGameArt.org
* **Skyscrapers:** Created by **OldNinjaCat (2022)**. Instagram and itch.io: `@oldninjacat`
* **Crash and Successful Landing Screens:** Generated using ChatGPT.

All other assets and game implementation were created by myself.
