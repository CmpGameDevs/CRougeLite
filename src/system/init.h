
#ifndef INIT_H
#define INIT_H

#include "../CRougeLite.h" // NOTE: declare global extern vars
#include "atlas.h"
#include <string.h>

void initSettings(GameState *gameState);
Weapon initWeapon(int opcode, bool isPlayer);

#endif // INIT_H
