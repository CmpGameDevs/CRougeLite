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
    game = initGameSystem();
  }
  return game;
}

//========================================================
// LOCAL VARIABLE DIFINATIONS (local to this file)
//========================================================

//========================================================
// Local Functions Headers
//========================================================
static void loadResources(Settings *settings);
static void eventHandler(Game_System *game);
static void keyboardEventHandler(Game_System *game);
static void mouseEventHandler(Game_System *game);
static void update();
static void draw();
static void clearResources();

//========================================================
// MAIN ENTRY POINT
//========================================================
int main(void) {
  Game_System *game = getGameSystemInstance();
  Settings *settings = &(game->settings);

  InitWindow(settings->screen_width, settings->screen_height, "C rougelite game");
    //printf("TEST\n");

  loadResources(settings);

  SetTargetFPS(60);

  // Main Game Loop
  bool *quit = &(game->finished);
  while (!WindowShouldClose() && !(*quit)) {
    update();

    draw();
  }

  clearResources();

  return 0;
}

static void loadResources(Settings* settings) {
  // load global assets
  InitAudioDevice();
  music = LoadMusicStream("./src/"
                          "./resources/ambient.ogg");
  // NOTE: All paths must start from the src dir

  SetMusicVolume(music, settings->volume / 100.0);
  PlayMusicStream(music);
}

static void eventHandler(Game_System *game) {
  keyboardEventHandler(game);

  mouseEventHandler(game);
}

static void keyboardEventHandler(Game_System *game) {
  switch (GetKeyPressed()) {
    case KEY_ESCAPE:
      game->finished = true;
    break;
    default:
    break;
  }
}

static void mouseEventHandler(Game_System *game) {
  Vector2 mousePos = GetMousePosition();
}

static void update() {
  UpdateMusicStream(music);
}

static void draw() {
  BeginDrawing();
  ClearBackground(GRAY);
  DrawGrid(10, 10);

  EndDrawing();
}

static void clearResources() {
  clearGameSystem();

  // Unload assets and cleaning
  UnloadMusicStream(music);

  CloseAudioDevice();

  CloseWindow();
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
