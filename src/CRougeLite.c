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

//========================================================
// Global Shared Variables
// NOTE: this must be defined as externs in the .h file
//========================================================
Music music = {0};

Game_System* getGameSystemInstance() {
  static Game_System *game = NULL;
  if (game == NULL) {
    initGameSystem(game);
  }
  return game;
}

//========================================================
// LOCAL VARIABLE DIFINATIONS (local to this file)
//========================================================
static const int screenWidth = 800;
static const int screenHeight = 450;

//========================================================
// Local Functions Headers
//========================================================
static void loadResources();
static void eventHandler();
static void update();
static void draw();
static void clearResources();

//========================================================
// MAIN ENTRY POINT
//========================================================
int main(void) {
  InitWindow(screenWidth, screenHeight, "C rougelite game");

  // load global assets
  InitAudioDevice();
  music = LoadMusicStream("./src/"
                          "./resources/ambient.ogg");
  // NOTE: All paths must start from the src dir

  SetMusicVolume(music, 1.0f);
  PlayMusicStream(music);

  SetTargetFPS(60);

  // Main Game Loop
  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    draw();
  }

  // Unload assets and cleaning
  UnloadMusicStream(music);

  CloseAudioDevice();

  CloseWindow();

  return 0;
}

static void draw() {
  BeginDrawing();
  ClearBackground(GRAY);
  DrawGrid(10, 10);

  EndDrawing();
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
