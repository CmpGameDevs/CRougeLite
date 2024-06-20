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
#include "../game/enemy.h"
#include "../game/combatAction.h"

// ***************************
// Private Function Prototypes
// ***************************
static void clearDictionary(Dictionary *dict);
static void clearDictionaryItem(Dictionary **dict);

//========================================================
// Init Functions
//========================================================

void clearGameState() {
  int enemy_num = gameState->numOfEnemies;
  Enemy *enemies = gameState->enemies;

  clearPlayers();
  clearEnemies();
  clearCombatActions();

  clearDictionary(gameState->characterDictionary);
  printf("Deleted all directories\n");
  clearDictionary(gameState->enemyDictionary);
  clearDictionary(gameState->playerWeaponDictionary);
  clearDictionary(gameState->enemyWeaponDictionary);
  freeResource((void *)gameState);
}

void freeResource(void *item) {
  if (item == NULL) return;
  free(item);
}

// *****************
// PRIVATE FUNCTIONS
// *****************
// FIXME: BROKEN???
static void clearDictionary(Dictionary *dict) {
  if (dict == NULL) return;

  while (dict->opcode != -1) {
    clearDictionaryItem(&dict);
    dict++;
  }
  clearDictionaryItem(&dict);
  printf("Deleted Dictionary\n");
}

static void clearDictionaryItem(Dictionary **dict) {
  if (dict == NULL || *dict == NULL)
    return;


  free(*dict);
  *dict = NULL;
}

void clearMap() {
  Game_System *game_system = getGameSystemInstance();
  Map *map = &(game_system->map);
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);

  for (int i = 0; i < tiles_mapper->numOfTiles; i++) {
    free(tiles_mapper->mapper[i]);
    tiles_mapper->mapper[i] = NULL;
  }

  for (int i = 0; i < tiles_mapper->numOfTiles; i++) {
    if (map->isTexturesLoaded[i])
      UnloadTexture(map->textures[i]);
  }

  free(map->textures);
  free(map->isTexturesLoaded);
}