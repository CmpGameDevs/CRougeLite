/***************************************************************
 *
 *
 *    
 *    ██╗    ██╗███████╗ █████╗ ██████╗  ██████╗ ███╗   ██╗
 *    ██║    ██║██╔════╝██╔══██╗██╔══██╗██╔═══██╗████╗  ██║
 *    ██║ █╗ ██║█████╗  ███████║██████╔╝██║   ██║██╔██╗ ██║
 *    ██║███╗██║██╔══╝  ██╔══██║██╔═══╝ ██║   ██║██║╚██╗██║
 *    ╚███╔███╔╝███████╗██║  ██║██║     ╚██████╔╝██║ ╚████║
 *     ╚══╝╚══╝ ╚══════╝╚═╝  ╚═╝╚═╝      ╚═════╝ ╚═╝  ╚═══╝
 *
 *     Weapon Module Header. (Game Object)
 *     Exposes the logic for the weapon object.
 *
 *     - Load Weapon
 *     - Use Weapon
 *     - Draw Weapon
 *
 **************************************************************/

#include "weapon.h"

Weapon initWeapon(int opcode, bool isPlayer)
{
  Dictionary *dict = (isPlayer ? gameState->playerWeaponDictionary
                               : gameState->enemyWeaponDictionary);
  int l = 0, r = (isPlayer ? NUM_OF_P_WEAPON : NUM_OF_E_WEAPON) - 1;

  while (l <= r)
  {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - opcode;
    if (!cmp)
    {
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

Inventory initInventory()
{
  Inventory inventory;
  inventory.MAX_NUM_OF_WEAPONS = 2;
  inventory.weapons = malloc(sizeof(Weapon) * inventory.MAX_NUM_OF_WEAPONS);
  inventory.currentNumOfWeapons = 0;
  inventory.currentWeapon = 0;
  return inventory;
}

void updateRangedWeapon(Weapon *weapon, bool isFired, int ID, Vector2 src, Vector2 dest, float deltaTime)
{
  float *reloadTime = &(weapon->weapon.ranged.stats.lastUseTime);

  float cooldown = weapon->weapon.ranged.stats.cooldown;

  int *ammo = &(weapon->weapon.ranged.ammo);

  int numBullets = weapon->weapon.ranged.numBullets;
  if (isFired && *ammo - numBullets > 0 && *reloadTime <= 0.0f)
  {
    initRangedWeaponShoot(ID, weapon->weapon.ranged, src, dest);

    *ammo -= numBullets;
    *reloadTime = cooldown;
  }

  if (*reloadTime > 0.0f)
    *reloadTime -= deltaTime;
}

void updateMeleeWeapon(Weapon *weapon, bool isFired, int ID, Vector2 src, Vector2 dest, float deltaTime)
{
  float *reloadTime = &(weapon->weapon.melee.stats.lastUseTime);

  float cooldown = weapon->weapon.melee.stats.cooldown;

  if (isFired && *reloadTime <= 0.0f)
  {
    initSlash(ID, weapon->weapon.melee.slashInfo, src, dest);

    *reloadTime = cooldown;
  }

  if (*reloadTime > 0.0f)
    *reloadTime -= deltaTime;
}

void clearInventory(Inventory *inventory) {
  if (inventory == NULL || inventory->weapons) return;

  free(inventory->weapons);
}