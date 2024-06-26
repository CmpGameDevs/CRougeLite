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

#include "game/enemy.h"
#include "game/player.h"
#include "system/atlas.h"
#include "system/camera.h"
#include "system/draw.h"
#include "system/input.h"
#include "system/map.h"
#include "system/collision.h"

//========================================================
// Global Shared Variables
// NOTE: this must be defined as externs in the .h file
//========================================================
Music music = {0};
GameState *gameState = NULL;

//========================================================
// LOCAL VARIABLE DEFINITIONS (local to this file)
//========================================================

//========================================================
// Local Functions Headers
//========================================================
static void loadResources();
static void update();
static void clearResources();

//========================================================
// MAIN ENTRY POINT
//========================================================
int main(void) {
  gameState = initGameState();
  initSettings();
  initCamera();
  initDictionary();
  Settings *settings = &(gameState->settings);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
  InitWindow(settings->screenWidth, settings->screenHeight, "C rougelite game");

  initAtlas();
  initMap();

  loadResources();

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

static void loadResources() {
  // load global assets
  Settings settings = gameState->settings;
  InitAudioDevice();
  music = LoadMusicStream("./src/"
                          "./resources/ambient.ogg");
  // NOTE: All paths must start from the src dir

  SetMusicVolume(music, settings.musicVolume / 100.0);
  PlayMusicStream(music);

  setupPlayers();
  setupEnemies();
}

static void update() {
  if (IsKeyPressed(KEY_TAB)) {
    gameState->settings.showDebugMenu = !gameState->settings.showDebugMenu;
  }
  updateCamera();
  updatePlayers();
  updateEnemies();
  updateCombatActions();
  broadPhaseCollision();
  if (gameState->settings.playMusic)
    UpdateMusicStream(music);
}

static void clearResources() {
  clearGameState();
  // clear map
  clearMap();

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
