#include "weapon.h"

static void initPlayerWeaponDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary) * (NUM_OF_P_WEAPON));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = P_GUN;
  dict[0].entry.weapon = (Weapon){
      .type = RANGED_WEAPON,
      .weapon.ranged = {
          .stats = {10, 0.5, 0, .weaponSprite = {LoadTexture("./src/"), 5, 5}},
          .bulletInfo = {3, 10, 100, 10,
                         .bulletSprite = {LoadTexture("./src/"), 5, 5}},
          30,
          30}};

  gameState->playerWeaponDictionary = dict;
}

static void initEnemyWeaponDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary) * (NUM_OF_E_WEAPON));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // Add in asc order
  // Because we fetch the info using binary search.
  // dict[0].opcode = E_SWORD;
  // dict[0].entry.weapon = (Weapon){
  //     .type = MELEE_WEAPON,
  //
  //     .weapon.melee = {
  //         .stats = {10, 0.5, 0, .weaponSprite = {LoadTexture("./src/"), 5,
  //         5}}, .slashInfo = {3, 10, true,
  //                       .slashSprite = {LoadTexture("./src/"), 5, 5}}}};
  //
  gameState->enemyWeaponDictionary = dict;
}

Weapon initWeapon(int opcode, bool isPlayer) {
  Dictionary *dict = (isPlayer ? gameState->playerWeaponDictionary
                               : gameState->enemyWeaponDictionary);
  int l = 0, r = (isPlayer ? NUM_OF_P_WEAPON : NUM_OF_E_WEAPON) - 1;

  while (l <= r) {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - opcode;
    if (!cmp) {
      return dict[mid].entry.weapon;
    }
    if (cmp < 0)
      l = mid + 1;
    else
      r = mid - 1;
  }

  fprintf(stderr, "Error: Player Weapon Dictionary Failed");
  exit(EXIT_FAILURE);
}