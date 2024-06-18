#ifndef WEAPON_H
#define WEAPON_H

#include "../CRougeLite.h" // NOTE: declare global extern vars

Weapon initWeapon(int opcode, bool isPlayer);
Inventory initInventory();
void useRangedWeapon();
void useMeleeWeapon();
void drawWeapon();

#endif // INPUT_H
