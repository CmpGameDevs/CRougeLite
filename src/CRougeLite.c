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

#include "system/atlas.h"
#include "system/draw.h"
#include "system/input.h"

//========================================================
// Global Shared Variables
// NOTE: this must be defined as externs in the .h file
//========================================================
Music music = {0};
GameState* gameState = NULL;

//========================================================
// LOCAL VARIABLE DEFINITIONS (local to this file)
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
  gameState = initGameState();
  Settings *settings = &(gameState->settings);

  InitWindow(settings->screenWidth, settings->screenHeight,
             "C rougelite game");
  // printf("TEST\n");
  loadResources(settings);

  initAtlas();
  SetTargetFPS(60);

  // Main Game Loop
  bool *quit = &(gameState->isFinished);
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

  SetMusicVolume(music, settings->musicVolume / 100.0);
  PlayMusicStream(music);

  Player *player = initPlayer("Marcus", KNIGHT, P_GUN, 0);
  player->object.transform.position =
      (Vector2){settings->screenWidth / 2.0, settings->screenHeight / 2.0};
  GameState *game = getGameSystemInstance();
  game->players->object.spriteRenderer.texture = LoadTexture("./src/"
                                       "./resources/Meow-Knight_Idle.png");
  //game->bulletTexture = LoadTexture("./src/""./resources/bullet.png");
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
