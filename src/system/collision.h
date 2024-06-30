#ifndef COLLISION_H
#define COLLISION_H


#include "../CRougeLite.h"
// FIXME: add hit struct later maybe
#include "../game/combatAction.h"

Hit initHitObject(void);
bool addEntityToHitObject(Hit *hit, Entity *entity);
void clearHitObject(Hit *hit);
void broadPhaseCollision(void);
bool resolveEntityCollision(Entity *a, Entity *b);

#endif // COLLISION_H