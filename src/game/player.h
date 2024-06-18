#ifndef PLAYER_H
#define PLAYER_H

#include "../CRougeLite.h" // NOTE: declare global extern vars
#include "atlas.h"
#include "weapon.h"
#include <string.h>

Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                   Vector2 position, int ID);
void updatePlayer();
void movePlayer();
void drawPlayer();

#endif // INPUT_H
