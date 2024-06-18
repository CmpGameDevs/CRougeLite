#include "weapon.h"

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

Inventory initInventory() {
  Inventory inventory;
  inventory.MAX_NUM_OF_WEAPONS = 2;
  inventory.weapons = malloc(sizeof(Weapon)*inventory.MAX_NUM_OF_WEAPONS);
  inventory.currentNumOfWeapons = 0;
  inventory.currentWeapon = 0;
  return inventory;
}