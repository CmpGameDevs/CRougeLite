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

#include "defs.h"
#include "raylib.h"

#include "CRougeLite.h" // NOTE: declare global extern vars
#include <string.h>

//========================================================
// LOCAL VARIABLE DIFINATIONS (local to this file)
//========================================================
static const int DEFAULT_MAX_PLAYERS = 4;
static const int DEFAULT_MAX_ENEMIES = 100;

//========================================================
// Init Functions
//========================================================

Game_System *initGameSystem() {
  Game_System *gameSystemInstance = (Game_System *)malloc(sizeof(Game_System));
  if (gameSystemInstance != NULL) {
    // Initialize Players Related Variables
    gameSystemInstance->MAX_NUM_OF_PLAYERS = DEFAULT_MAX_PLAYERS;
    gameSystemInstance->players =
        (Player *)malloc(sizeof(Player) * DEFAULT_MAX_PLAYERS);
    gameSystemInstance->num_of_players = 0;

    // Initialize Enemies Related Variables
    gameSystemInstance->MAX_NUM_OF_ENEMIES = DEFAULT_MAX_ENEMIES;
    gameSystemInstance->enemies =
        (Enemy *)malloc(sizeof(Enemy) * DEFAULT_MAX_ENEMIES);
    gameSystemInstance->num_of_enemies = 0;

    // Initialize Other General Variables
    gameSystemInstance->level = 1;
    gameSystemInstance->game_over = false;
    gameSystemInstance->finished = false;
    initSettings(gameSystemInstance);
  }

  return gameSystemInstance;
}

void initSettings(Game_System *gameSystemInstance) {
  gameSystemInstance->settings.screen_width = SCREEN_WIDTH;
  gameSystemInstance->settings.screen_height = SCREEN_HEIGHT;
  gameSystemInstance->settings.volume = 50;
  gameSystemInstance->settings.music_on = true;
  gameSystemInstance->settings.sfx_on = true;
}

static void addPlayer(Player *player) {
  Game_System *game = getGameSystemInstance();
  Player *players = game->players;
  players[game->num_of_players++] = *player;
}

Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon) {
  Player *player = (Player *)malloc(sizeof(Player));
  player->name = strdup(name);
  player->type = type;
  player->weapon = weapon;
  player->position.x = SCREEN_WIDTH / 2;
  player->position.y = SCREEN_HEIGHT / 2;
  player->health = 100.0;
  player->speed = 5.0;
  player->acceleration = 0.1;
  player->fire_rate = 0.5;
  player->score = 0;

  // TODO: Make dictionary for infos related to each type of character.
  addPlayer(player);
  return player;
}

Enemy *initEnemy(E_TYPE type, E_WEAPON weapon) {
  Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
  enemy->type = type;
  enemy->weapon = weapon;
  enemy->health = 50;
  enemy->speed = 0.8;
  enemy->acceleration = 0.05;
  enemy->fire_rate = 1.0;

  // TODO: Make dictionary for infos related to each type of enemy.
  return enemy;
}
