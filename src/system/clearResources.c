/*********************************************************
 *
 *
 *     ██████╗██╗     ███████╗ █████╗ ███╗   ██╗
 *    ██╔════╝██║     ██╔════╝██╔══██╗████╗  ██║
 *    ██║     ██║     █████╗  ███████║██╔██╗ ██║
 *    ██║     ██║     ██╔══╝  ██╔══██║██║╚██╗██║
 *    ╚██████╗███████╗███████╗██║  ██║██║ ╚████║
 *     ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝
 *
 *    Clean and clear all resources used in the game
 *
 *
 *********************************************************/

#include "../CRougeLite.h" // NOTE: declare global extern vars
#include "../game/player.h"

//========================================================
// LOCAL VARIABLE DIFINATIONS (local to this file)
//========================================================

//========================================================
// Init Functions
//========================================================

void clearGameState() {
  int enemy_num = gameState->numOfEnemies;
  Enemy *enemies = gameState->enemies;

  clearPlayers();

  while (enemy_num--) {
    // printf("Deleting Enemy of Type: %d\n", enemies->name);
    clearEnemy(&enemies);
    enemies++;
  }
}

void clearEnemy(Enemy **enemy) {
  if (enemy == NULL || *enemy == NULL)
    return;

  free(*enemy);
  *enemy = NULL;
}
