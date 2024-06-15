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
// LOCAL VARIABLE DIFINATIONS (local to this file)
//========================================================
static const int DEFAULT_SCREEN_WIDTH = 800;
static const int DEFAULT_SCREEN_HEIGHT = 450;
static const int DEFAULT_MAX_PLAYERS = 4;
static const int DEFAULT_MAX_ENEMIES = 100;

//========================================================
// Init Functions
//========================================================

Game_System* initGameSystem() {
  Game_System* gameSystemInstance = (Game_System*)malloc(sizeof(Game_System));
  if (gameSystemInstance != NULL) {
    // Initialize Players Related Variables
    gameSystemInstance->MAX_NUM_OF_PLAYERS = DEFAULT_MAX_PLAYERS;
    gameSystemInstance->num_of_players = 0;

    // Initialize Enemies Related Variables
    gameSystemInstance->MAX_NUM_OF_ENEMIES = DEFAULT_MAX_ENEMIES;
    gameSystemInstance->num_of_enemies = 0;

    // Initialize Other General Variables
    gameSystemInstance->level = 1;
    gameSystemInstance->game_over = false;
    gameSystemInstance->finished = false;
    initSettings(gameSystemInstance);
  }  

  return gameSystemInstance;
}

void initSettings(Game_System* gameSystemInstance) {
  gameSystemInstance->settings.screen_width = DEFAULT_SCREEN_WIDTH;
  gameSystemInstance->settings.screen_height = DEFAULT_SCREEN_HEIGHT;
  gameSystemInstance->settings.volume = 50;
  gameSystemInstance->settings.music_on = true;
  gameSystemInstance->settings.sfx_on = true;
}

void initPlayer(Player *player, const char *name, P_TYPE type, P_WEAPON weapon) {
    player->name = strdup(name);
    player->type = type;
    player->weapon = weapon;
    player->position.x = 0.0;
    player->position.y = 0.0;
    player->health = 100.0;
    player->speed = 1.0;
    player->acceleration = 0.1;
    player->fire_rate = 0.5;
    player->score = 0;

    // TODO: Make dictionary for infos related to each type of character.
}

void initEnemy(Enemy *enemy, E_TYPE type, E_WEAPON weapon) {
    enemy->type = type;
    enemy->weapon = weapon;
    enemy->health = 50;
    enemy->speed = 0.8;
    enemy->acceleration = 0.05;
    enemy->fire_rate = 1.0;

    // TODO: Make dictionary for infos related to each type of enemy.
}