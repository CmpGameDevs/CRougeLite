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

  gameState->mainMenuBackground = LoadTexture("./src/resources/gfx/menu.png");
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

  printf("Game closed gracefully\n");

  return 0;
}

static void loadResources() {
  // load global assets
  Settings settings = gameState->settings;
  InitAudioDevice();
  
  // Initialize sound system
  initSoundSystem();
  loadSoundEffect("src/resources/sfx/weapons/mage_fireball/big-fire-ball-attack.wav", "big_fireball");
  loadSoundEffect("src/resources/sfx/weapons/mage_fireball/burn.wav", "fireball");
  loadSoundEffect("src/resources/sfx/environment/pickups/item.wav", "pickup");
  loadSoundEffect("src/resources/sfx/enemies/death/slime_death.wav", "slime_death");
  loadSoundEffect("src/resources/sfx/player/impactGeneric_light_003.ogg", "missing_item");
  loadSoundEffect("src/resources/sfx/UI/switch2.ogg", "interact");

  music = LoadMusicStream("./src/"
                          "./resources/ambient.ogg");
  // NOTE: All paths must start from the src dir

  SetMusicVolume(music, settings.musicVolume / 100.0);
  PlayMusicStream(music);

  setupPlayers();
  setupEnemies();
}

static void update() {
  updateCamera();
  if (gameState->settings.playMusic)
    UpdateMusicStream(music);
  if (gameState->settings.sfx_on)
    setSoundVolume(gameState->settings.sfxVolume);
  else
    setSoundVolume(0.0f);

  if (!gameState->isGameStarted || gameState->isGameOver) return;

  updatePlayers();
  updateEnemies();
  updateCombatActions();
  broadPhaseCollision();
}

static void clearResources() {
  // Clear game state and map
  clearGameState();
  clearMap();

  // Clear sound system
  clearSoundSystem();

  // Unload assets and cleaning for raylib
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
