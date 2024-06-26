#ifndef COLLISION_H
#define COLLISION_H


#include "../CRougeLite.h"
// FIXME: add hit struct later maybe
#include "../game/combatAction.h"

void broadPhaseCollision(void);
void getGameObjectIndices(int *startX, int *startY, int *endX, int *endY, Collider2D *collider);
void resolveEntityCollision(Entity *a, Entity *b);

#endif // COLLISION_H