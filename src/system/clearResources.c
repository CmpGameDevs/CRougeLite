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
#include "../game/combatAction.h"
#include "../game/enemy.h"
#include "../game/player.h"

// ***************************
// Private Function Prototypes
// ***************************
static void clearDictionary(Dictionary **dict);

//========================================================
// Init Functions
//========================================================

void clearGameState() {
  int enemy_num = gameState->numOfEnemies;
  Enemy *enemies = gameState->enemies;

  clearPlayers();
  clearEnemies();
  clearCombatActions();

  clearDictionary(&gameState->characterDictionary);
  clearDictionary(&gameState->enemyDictionary);
  clearDictionary(&gameState->playerWeaponDictionary);
  clearDictionary(&gameState->enemyWeaponDictionary);
  freeResource((void *)gameState);
}

void freeResource(void *item) {
  if (item == NULL)
    return;
  free(item);
}

// *****************
// PRIVATE FUNCTIONS
// *****************
static void clearDictionary(Dictionary **dict) {
  if (dict == NULL || *dict == NULL)
    return;

  free(*dict);
  *dict = NULL;
}

void clearMap() {
  Map *map = &(gameState->map);
  TilesMapper *tiles_mapper = &(gameState->map.tilesMapper);

  for (int i = 0; i < tiles_mapper->numOfTiles; i++) {
    free(tiles_mapper->tileInfo[i].filename);
    tiles_mapper->tileInfo[i].filename = NULL;
  }
}
