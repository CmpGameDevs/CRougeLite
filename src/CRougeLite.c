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

Game_System *getGameSystemInstance() {
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
static void update();
static void clearResources();

//========================================================
// MAIN ENTRY POINT
//========================================================
int main(void) {
  Game_System *game = getGameSystemInstance();
  Settings *settings = &(game->settings);

  InitWindow(settings->screen_width, settings->screen_height,
             "C rougelite game");
  // printf("TEST\n");
  loadResources(settings);

  SetTargetFPS(60);

  // Main Game Loop
  bool *quit = &(game->finished);
  while (!WindowShouldClose() && !(*quit)) {
    handleInput();
    update();

    drawScene();
  }

  clearResources();

  return 0;
}

static void loadResources(Settings *settings) {
  // load global assets
  InitAudioDevice();
  music = LoadMusicStream("./src/"
                          "./resources/ambient.ogg");
  // NOTE: All paths must start from the src dir

  SetMusicVolume(music, settings->volume / 100.0);
  PlayMusicStream(music);

  Player *player = initPlayer("Marcus", KNIGHT, LONG_SWORD);
  player->position =
      (Vector2){settings->screen_width / 2.0, settings->screen_height / 2.0};
  Game_System *game = getGameSystemInstance();
  game->players->texture = LoadTexture("./src/"
                                       "./resources/Meow-Knight_Idle.png");
}

static void update() { UpdateMusicStream(music); }

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
