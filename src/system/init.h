
#ifndef INIT_H
#define INIT_H

#include "../CRougeLite.h" // NOTE: declare global extern vars
#include "atlas.h"
#include <string.h>
// TODO: DELETE AFTER ENEMY REFACTOR
#include "../game/weapon.h"

void initSettings();
float GetAngleBetweenPoints(Vector2 point1, Vector2 point2);

#endif // INIT_H
