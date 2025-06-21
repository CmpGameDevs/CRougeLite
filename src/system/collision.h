#ifndef COLLISION_H
#define COLLISION_H


#include "../CRougeLite.h"
// FIXME: add hit struct later maybe
#include "../game/combatAction.h"
#include "map.h"

/**
 * initHitObject - initialize a hit struct to be used for collision handling
 * 
 * @return the hit struct object
 */
Hit initHitObject(void);

/**
 * addEntityToHitObject - add a collided entity to hit object
 * 
 * @param hit Pointer to the hit object
 * @param entity Pointer to the entity
 * 
 * @return false if the entity is already in the list, true otherwise
 */
bool addEntityToHitObject(Hit *hit, Entity *entity);

/**
 * clearHitObject - free used heap space
 * 
 * @param hit Pointer to the hit object
 */
void clearHitObject(Hit *hit);

/**
 * broadPhaseCollision - Handles the broad collision detection phase
 * and calls the Narrow Phase Collision Detections
 */
void broadPhaseCollision(void);

/**
 * getGameObjectByIndices - Get the GameObject from the entity indices
 * 
 * @param startX The starting X index of the GameObject
 * @param startY The starting Y index of the GameObject
 * @param endX The ending X index of the GameObject
 * @param endY The ending Y index of the GameObject
 * @param collider The Collider2D of the GameObject
 */
void getGameObjectIndices(int *startX, int *startY, int *endX, int *endY, Collider2D *collider);

#endif // COLLISION_H