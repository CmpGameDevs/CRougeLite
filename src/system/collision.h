#ifndef COLLISION_H
#define COLLISION_H


#include "../CRougeLite.h"

void updateMapGrid(void);
void resolveCollision(GameObject *a, GameObject *b);
void updateGameObjectPosition(GameObject *a, Vector2 newPosition);

#endif // COLLISION_H