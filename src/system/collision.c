#include "collision.h"
#include <raylib.h>

// ***************************
// Local Global Variables
// ***************************
static int GRID_WIDTH, GRID_CELL_WIDTH, GRID_HEIGHT, GRID_CELL_HEIGHT;

// ***************************
// Private Function Prototypes
// ***************************
static void narrowPhaseCollision(void);
static void getGameObjectIndices(int *startX, int *startY, int *endX, int *endY, Collider2D *collider);
static GameObject *getGameObjectByIndices(Entity *entity);
static bool checkAABBCollision(const Rectangle a, const Rectangle b);
static bool resolveAABBCollision (GameObject *a, GameObject *b);

/**
 * initHitObject - initialize a hit struct to be used for collision handling
 * 
 * @return the hit struct object
 */
Hit initHitObject(void)
{
  Hit hit;
  hit.entities = NULL;
  hit.numOfEntities = hit.hitCount = hit.checkedCount = 0;
  return hit;
}

/**
 * addEntityToHitObject - add a collided entity to hit object
 * 
 * @param hit Pointer to the hit object
 * @param entity Pointer to the entity
 * 
 * @return false if the entity is already in the list, true otherwise
 */
bool addEntityToHitObject(Hit *hit, Entity *entity)
{
  Entity **entitiesPtr = &(hit->entities);
  for (int i = 0; i < hit->hitCount; i++)
    if ((*entitiesPtr + i)->ID == entity->ID &&
        (*entitiesPtr + i)->type == entity->type)  return false;

  if (hit->hitCount == hit->numOfEntities) {
    hit->numOfEntities = (!hit->numOfEntities ? 1 : hit->numOfEntities * 2);
    *entitiesPtr = realloc(*entitiesPtr, hit->numOfEntities * sizeof(Entity));
  }
  hit->entities[hit->hitCount++] = *entity;
  return true;
}

/**
 * clearHitObject - free used heap space
 * 
 * @param hit Pointer to the hit object
 */
void clearHitObject(Hit *hit)
{
  free(hit->entities);
}

/**
 * broadPhaseCollision - Handles the broad collision detection phase
 * and calls the Narrow Phase Collision Detections
 */
void broadPhaseCollision(void)
{
  Map *map = &(gameState->map);
  GRID_WIDTH = map->numOfCols;
  GRID_CELL_WIDTH = map->tileWidth * map->scale;
  GRID_HEIGHT = map->numOfRows;
  GRID_CELL_HEIGHT = map->tileHeight * map->scale;

  // Reset object count
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      map->grid[i][j].objectCount = 0;
    }
  }

  // Check Players
  Player *players = gameState->players;
  for (int k = 0; k < gameState->numOfPlayers; k++) {
    GameObject *object = &(players[k].object);
    int startX, startY, endX, endY;

    object->collider.isColliding = false;
    getGameObjectIndices(&startX, &startY, &endX, &endY, &(object->collider));
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &(map->grid[i][j]);
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].type = ENTITY_PLAYER;
          cell->objectIndices[cell->objectCount].ID = players[k].ID;
          cell->objectIndices[cell->objectCount++].entity.player = &(players[k]);
        }
      }
    }
  }

  // draw enemies colliders
  Enemy *enemies = gameState->enemies;
  for (int k = 0; k < gameState->numOfEnemies; k++) {
    GameObject *object = &(enemies[k].object);
    int startX, startY, endX, endY;

    object->collider.isColliding = false;
    getGameObjectIndices(&startX, &startY, &endX, &endY, &(object->collider));
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &(map->grid[i][j]);
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].type = ENTITY_ENEMY;
          cell->objectIndices[cell->objectCount].ID = enemies[k].ID;
          cell->objectIndices[cell->objectCount++].entity.enemy = &(enemies[k]);
        }
      }
    }
  }

  // draw bullet actions colliders
  CombatAction *actions = gameState->combatActions;
  for (int k = 0; k < gameState->numOfCombatActions; k++) {
    GameObject *object = NULL;
    int startX, startY, endX, endY;
    switch (actions[k].type) {
      case ACTION_BULLET:
        object = &(actions[k].action.bullet.bulletInfo.object);
        break;
      case ACTION_SLASH:
        object = &(actions[k].action.slash.slashInfo.object);
        break;
      default:
        break;
    }
    
    object->collider.isColliding = false;
    getGameObjectIndices(&startX, &startY, &endX, &endY, &(object->collider));
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &(map->grid[i][j]);
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].type = ENTITY_P_COMBAT_ACTION;
          cell->objectIndices[cell->objectCount].ID = actions[k].ID;
          cell->objectIndices[cell->objectCount++].entity.action = &(actions[k]);
        }
      }
    }
  }
  narrowPhaseCollision();
}

bool resolveEntityCollision(Entity *a, Entity *b)
{
  switch (a->type) {
    case ENTITY_PLAYER:
      return true;
      // Player Collision System
      break;
    case ENTITY_ENEMY:
      return true;
      // Enemy Collision System
      break;
    case ENTITY_E_COMBAT_ACTION:
      return addEntityToHitObject(&(a->entity.action->hit), b);
      //resolveCombatActionCollision(a->entity.action, b, false);
      break;
    case ENTITY_P_COMBAT_ACTION:
      return addEntityToHitObject(&(a->entity.action->hit), b);
      //resolveCombatActionCollision(a->entity.action, b, true);
      break;
    default:
      break;
  }
}

// *****************
// PRIVATE FUNCTIONS
// *****************

static void narrowPhaseCollision(void)
{
  Map *map = &(gameState->map);
  // Reset object count
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      GridCell *cell = &(map->grid[i][j]);
      int objectCount = cell->objectCount;
      for (int n = 0; n < objectCount; n++) {
        Entity *entityA = &(cell->objectIndices[n]);
        GameObject *objectA = getGameObjectByIndices(entityA);
        for (int m = n + 1; m < objectCount; m++) {
          Entity *entityB = &(cell->objectIndices[m]);
          GameObject *objectB = getGameObjectByIndices(entityB);

          if (checkAABBCollision(objectA->collider.bounds, objectB->collider.bounds)) {
            if (resolveEntityCollision(entityA, entityB) && resolveEntityCollision(entityB, entityA))
              resolveAABBCollision(objectA, objectB);
          }
        }
      }
    }
  }
}

static void getGameObjectIndices(int *startX, int *startY, int *endX, int *endY, Collider2D *collider)
{
  *startX = Clamp(collider->bounds.x / GRID_CELL_WIDTH, 0, GRID_WIDTH - 1);
  *startY = Clamp(collider->bounds.y / GRID_CELL_HEIGHT, 0, GRID_HEIGHT - 1);
  *endX = Clamp((collider->bounds.x + collider->bounds.width) / GRID_CELL_WIDTH, 0, GRID_WIDTH - 1);
  *endY = Clamp((collider->bounds.y + collider->bounds.height) / GRID_CELL_HEIGHT, 0, GRID_HEIGHT - 1);
}

static GameObject *getGameObjectByIndices(Entity *entity)
{
  Player *players = gameState->players;
  Enemy *enemies = gameState->enemies;
  CombatAction *actions = gameState->combatActions;

  switch (entity->type)
  {
  case ENTITY_PLAYER:
    return &(entity->entity.player->object);
  case ENTITY_ENEMY:
    return &(entity->entity.enemy->object);
  case ENTITY_P_COMBAT_ACTION:
    switch (entity->entity.action->type)
    {
    case ACTION_BULLET:
      return &(entity->entity.action->action.bullet.bulletInfo.object);
    case ACTION_SLASH:
      return &(entity->entity.action->action.slash.slashInfo.object);
    default:
      break;
    }
    break;
  default:
    break;
  }

  return NULL;
}

static bool checkAABBCollision(const Rectangle a, const Rectangle b)
{
  return (a.x < b.x + b.width &&
          a.x + a.width > b.x &&
          a.y < b.y + b.height &&
          a.y + a.height > b.y);
}

static bool resolveAABBCollision (GameObject *a, GameObject *b)
{
  a->collider.isColliding = true;
  b->collider.isColliding = true;
  Rectangle *boundsA = &(a->collider.bounds);
  Rectangle *boundsB = &(b->collider.bounds);
  Vector2 *positionA = &(a->transform.position);
  Vector2 *positionB = &(b->transform.position);
  Vector2 tempA = *positionA, tempB = *positionB;
  float centerDiffX = fabs((boundsA->x + (boundsA->width / 2.0)) - (boundsB->x + (boundsB->width / 2.0)));
  float optimalDiffX = (boundsA->width + boundsB->width) / 2.0;
  float centerDiffY = fabs((boundsA->y + (a->collider.bounds.height / 2.0)) - (boundsB->y + (boundsB->height / 2.0)));
  float optimalDiffY = (boundsA->height + boundsB->height) / 2.0;
  float overlapX = optimalDiffX - centerDiffX;
  float overlapY = optimalDiffY - centerDiffY;

  if (overlapX < overlapY) {
    if (tempA.x < tempB.x) {
      tempA.x -= overlapX / 2;
      tempB.x += overlapX / 2;
    } else {
      tempA.x += overlapX / 2;
      tempB.x -= overlapX / 2;
    }
  } else {
    if (tempA.y < tempB.y) {
      tempA.y -= overlapY / 2;
      tempB.y += overlapY / 2;
    } else {
      tempA.y += overlapY / 2;
      tempB.y -= overlapY / 2;
    }
  }
  
  if (a->rigidBody.type == BODY_DYNAMIC) {
    *positionA = tempA;
    boundsA->x = positionA->x;
    boundsA->y = positionA->y;
  }

  if (b->rigidBody.type == BODY_DYNAMIC) {
    *positionB = tempB;
    boundsB->x = positionB->x;
    boundsB->y = positionB->y;
  }
}
