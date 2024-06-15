/*********************************************************
 *
 *
 *    ███╗   ███╗ █████╗ ██╗███╗   ██╗
 *    ████╗ ████║██╔══██╗██║████╗  ██║
 *    ██╔████╔██║███████║██║██╔██╗ ██║
 *    ██║╚██╔╝██║██╔══██║██║██║╚██╗██║
 *    ██║ ╚═╝ ██║██║  ██║██║██║ ╚████║
 *    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝
 *
 *    Main File of the game where the main loop happens.
 *    Game Authors:
 *
 *    - Moamen
 *    - Marwan
 *    - Akram
 *    - Amir
 *
 *********************************************************/

#include "raylib.h"

#include "CRougeLite.h" // NOTE: declare global extern vars

#include "engine/draw.h"
#include "engine/input.h"

//========================================================
// Global Shared Variables
// NOTE: this must be defined as externs in the .h file
//========================================================
Music music = {0};
Player player = {0};
Entity bullet = {0};
State state = {0};

//========================================================
// MAIN ENTRY POINT
//========================================================
int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C rougelite game");

  // load global assets
  InitAudioDevice();
  music = LoadMusicStream("./src/"
                          "./resources/ambient.ogg");
  // NOTE: All paths must start from the src dir

  SetMusicVolume(music, 1.0f);
  PlayMusicStream(music);

  SetTargetFPS(60);

  player.x = SCREEN_WIDTH / 2;
  player.y = SCREEN_HEIGHT / 2;
  player.texture = LoadTexture("./src/"
                               "./resources/knight.png");
  bullet.texture = LoadTexture("./src/"
                               "./resources/bullet.png");
  bullet.health = 0;
  state.isFiring = false;

  // Main Game Loop
  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    handleInput();

    if (state.isFiring && bullet.health == 0) {
      bullet.x = player.x;
      bullet.y =
          player.y + player.texture.height / 2 - bullet.texture.height / 2;
      bullet.dx = 5;
      bullet.dy = 0;
      bullet.health = 1;
    }

    bullet.x += bullet.dx;
    bullet.y += bullet.dy;

    if (bullet.x > SCREEN_WIDTH || bullet.x < 0 || bullet.y > SCREEN_HEIGHT ||
        bullet.y < 0) {
      bullet.health = 0;
    }

    drawScene();
  }

  // Unload assets and cleaning
  UnloadMusicStream(music);

  CloseAudioDevice();

  CloseWindow();

  return 0;
}

/*********************************************************
 *
 *
 *      ███████╗ ██████╗ ███████╗
 *      ██╔════╝██╔═══██╗██╔════╝
 *      █████╗  ██║   ██║█████╗
 *      ██╔══╝  ██║   ██║██╔══╝
 *      ███████╗╚██████╔╝██║
 *      ╚══════╝ ╚═════╝ ╚═╝
 *
 *
 *********************************************************/
