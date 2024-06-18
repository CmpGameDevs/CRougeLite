#ifndef WEAPON_H
#define WEAPON_H

#include "../CRougeLite.h" // NOTE: declare global extern vars
#include "atlas.h"
#include <string.h>

Weapon initWeapon(int opcode, bool isPlayer);
void useRangedWeapon();
void useMeleeWeapon();
void drawWeapon();

#endif // INPUT_H
