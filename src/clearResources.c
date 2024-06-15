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

void clearGameSystem() {
  Game_System *game = getGameSystemInstance();
  int player_num = game->num_of_players;
  int enemy_num = game->num_of_enemies;
  Player *players = game->players;
  Enemy *enemies = game->enemies;
  while(player_num--) {
    printf("Deleting Player: %s\n", players->name);
    clearPlayer(&players);
    players++;
  }

  while(enemy_num--) {
    printf("Deleting Enemy of Type: %d\n", enemies->type);
    clearEnemy(&enemies);
    enemies++;
  }
}

void clearPlayer(Player** player) {
  if (player == NULL || *player == NULL) return;

  free((*player)->name);
  free(*player);
  *player = NULL;
}

void clearEnemy(Enemy** enemy) {
  if (enemy == NULL || *enemy == NULL) return;

  free(*enemy);
  *enemy = NULL;
}