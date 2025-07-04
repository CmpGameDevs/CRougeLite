#include "collision.h"
#include "../game/player.h"
#include <raylib.h>

// ***************************
// Local Global Variables
// ***************************
static int GRID_WIDTH, GRID_CELL_WIDTH, GRID_HEIGHT, GRID_CELL_HEIGHT;

// ***************************
// Private Function Prototypes
// ***************************
static void narrowPhaseCollision(void);
static GameObject *getGameObjectByIndices(Entity *entity);
static bool checkAABBCollision(const Rectangle a, const Rectangle b);
static void resolveAABBCollision(GameObject *a, GameObject *b);
static Rectangle getTileCollisionBounds(int row, int col);
static void calculateCollisionSeparation(Rectangle objBounds, Rectangle tileBounds, Vector2 *separation);
static bool checkTileCollision(int row, int col, Entity *entity, bool resolveCollision);
static bool resolveEntityCollision(Entity *a, Entity *b);
static bool resolveTileCollision(Entity *entity, int row, int col, int layer);

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
    
    // For interactive objects, pad the collider bounds
    Collider2D paddedBounds = object->collider;
    paddedBounds.bounds.x -= 5;
    paddedBounds.bounds.y -= 5;
    paddedBounds.bounds.width += 10;
    paddedBounds.bounds.height += 10;
    
    getGameObjectIndices(&startX, &startY, &endX, &endY, &(paddedBounds));
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

/**
 * resolveEntityCollision - Resolve collision between two entities
 * 
 * @param a Pointer to the first entity
 * @param b Pointer to the second entity
 * 
 * @return true if the collision was resolved, false otherwise
 */
static bool resolveEntityCollision(Entity *a, Entity *b)
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

/**
 * getGameObjectByIndices - Get the GameObject from the entity indices
 * 
 * @param startX The starting X index of the GameObject
 * @param startY The starting Y index of the GameObject
 * @param endX The ending X index of the GameObject
 * @param endY The ending Y index of the GameObject
 * @param collider The Collider2D of the GameObject
 */
void getGameObjectIndices(int *startX, int *startY, int *endX, int *endY, Collider2D *collider)
{
  *startX = Clamp(collider->bounds.x / GRID_CELL_WIDTH, 0, GRID_WIDTH - 1);
  *startY = Clamp(collider->bounds.y / GRID_CELL_HEIGHT, 0, GRID_HEIGHT - 1);
  *endX = Clamp((collider->bounds.x + collider->bounds.width) / GRID_CELL_WIDTH, 0, GRID_WIDTH - 1);
  *endY = Clamp((collider->bounds.y + collider->bounds.height) / GRID_CELL_HEIGHT, 0, GRID_HEIGHT - 1);
}

// *****************
// PRIVATE FUNCTIONS
// *****************

/**
 * narrowPhaseCollision - Handles the narrow phase collision detection
 */
static void narrowPhaseCollision(void)
{
  Map *map = &(gameState->map);
  
  // Check tile collisions for all objects
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      GridCell *cell = &(map->grid[i][j]);
      int objectCount = cell->objectCount;
      
      for (int n = 0; n < objectCount; n++) {
        Entity *entity = &(cell->objectIndices[n]);
        checkTileCollision(i, j, entity, true);
      }
    }
  }
  
  // Check entity-to-entity collisions
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

/**
 * getGameObjectByIndices - Get the GameObject from an Entity based on its type
 * 
 * @param entity The Entity to get the GameObject from
 * 
 * @return Pointer to the GameObject associated with the Entity
 *         or NULL if the Entity type is not recognized
 */
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

/**
 * checkAABBCollision - Check if two Axis-Aligned Bounding Boxes (AABB) collide
 * 
 * @param a The first rectangle (AABB)
 * @param b The second rectangle (AABB)
 * 
 * @return true if the rectangles collide, false otherwise
 */
static bool checkAABBCollision(const Rectangle a, const Rectangle b)
{
  return (a.x < b.x + b.width &&
          a.x + a.width > b.x &&
          a.y < b.y + b.height &&
          a.y + a.height > b.y);
}

/**
 * resolveAABBCollision - Resolve collision between two Axis-Aligned Bounding Boxes (AABB)
 * 
 * @param a Pointer to the first GameObject
 * @param b Pointer to the second GameObject
 */
static void resolveAABBCollision(GameObject *a, GameObject *b)
{
  if (!a || !b) return;
  if (a->rigidBody.type == BODY_GHOST || b->rigidBody.type == BODY_GHOST) return;
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

/**
 * calculateCollisionSeparation - Helper function to calculate separation vector for collision resolution
 * 
 * @param objBounds The bounds of the colliding object
 * @param tileBounds The bounds of the tile being collided with
 * @param separation Pointer to the separation vector to modify
 */
static void calculateCollisionSeparation(Rectangle objBounds, Rectangle tileBounds, Vector2 *separation) {
  float overlapX = fmin(objBounds.x + objBounds.width, tileBounds.x + tileBounds.width) -
                 fmax(objBounds.x, tileBounds.x);
  float overlapY = fmin(objBounds.y + objBounds.height, tileBounds.y + tileBounds.height) -
                 fmax(objBounds.y, tileBounds.y);
  
  // Choose minimum separation
  if (overlapX < overlapY) {
    // Separate horizontally
    if (objBounds.x < tileBounds.x) {
      separation->x -= overlapX;
    } else {
      separation->x += overlapX;
    }
  } else {
    // Separate vertically
    if (objBounds.y < tileBounds.y) {
      separation->y -= overlapY;
    } else {
      separation->y += overlapY;
    }
  }
}

/**
 * getTileCollisionBounds - Get the collision rectangle for a tile at row, col
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * 
 * @return The collision bounds of the tile as a Rectangle
 */
static Rectangle getTileCollisionBounds(int row, int col) {
  Map *map = &(gameState->map);
  float tileWidth = map->tileWidth * map->scale;
  float tileHeight = map->tileHeight * map->scale;
  
  return (Rectangle){
    col * tileWidth,
    row * tileHeight,
    tileWidth,
    tileHeight
  };
}

/**
 * checkTileCollision - Check if a GameObject collides with tiles in the specified cell
 * 
 * @param row The row index of the cell to check
 * @param col The column index of the cell to check
 * @param entity The Entity to check for collisions
 * @param resolveCollision Whether to resolve the collision by moving the object
 * 
 * @return true if there was a collision, false otherwise
 */
static bool checkTileCollision(int row, int col, Entity *entity, bool resolveCollision) {
  if (!entity) return false;
  
  Map *map = &(gameState->map);
  
  // Bounds check for the cell
  if (row < 0 || row >= map->numOfRows || col < 0 || col >= map->numOfCols) {
    return false;
  }
  
  GameObject *object = getGameObjectByIndices(entity);
  Rectangle objectBounds = object->collider.bounds;
  bool hasCollision = false;
  Vector2 separation = {0, 0};
  BodyType objectType = object->rigidBody.type;
  
  // Check all layers in this specific cell
  for (int layer = LAYER_WALLS; layer < MAX_CELL_ID; layer++) {
    // Skip empty tiles
    if (map->mapIds[row][col][layer] == -1) continue;

    TileProperties tileProperties = getTileProperties(map->mapIds[row][col][layer]);
    
    // Skip walkable tiles that don't have special properties
    if (isPropertySet(tileProperties, TILE_PROP_WALKABLE) &&
        !isPropertySet(tileProperties, TILE_PROP_INTERACTIVE) &&
        !isPropertySet(tileProperties, TILE_PROP_COLLECTIBLE) &&
        !isPropertySet(tileProperties, TILE_PROP_DESTRUCTIBLE)) {
      continue;
    }

    Rectangle tileBounds = getTileCollisionBounds(row, col);
    
    if (isPropertySet(tileProperties, TILE_PROP_INTERACTIVE)) {
      // Adjust bounds for interactive tiles to allow interaction
      tileBounds.x -= 5;
      tileBounds.y -= 5;
      tileBounds.width += 10;
      tileBounds.height += 10;
    } else if (isPropertySet(tileProperties, TILE_PROP_COLLECTIBLE)) {
      // Adjust bounds for collectible tiles to make collection logical
      tileBounds.x += 10;
      tileBounds.y += 10;
      tileBounds.width -= 20;
      tileBounds.height -= 20;
    }

    if (checkAABBCollision(objectBounds, tileBounds)) {
      hasCollision = true;
      object->collider.isColliding = true;
      
      if (!isPropertySet(tileProperties, TILE_PROP_WALKABLE)) {
        if (resolveCollision && object->rigidBody.type == BODY_DYNAMIC) {
          calculateCollisionSeparation(objectBounds, tileBounds, &separation);
        }
      }

      resolveTileCollision(entity, row, col, layer);
    }
  }
  
  // Apply collision resolution if needed
  if (resolveCollision && object->rigidBody.type == BODY_DYNAMIC && 
      (separation.x != 0 || separation.y != 0)) {
    object->transform.position.x += separation.x;
    object->transform.position.y += separation.y;
    object->collider.bounds.x = object->transform.position.x;
    object->collider.bounds.y = object->transform.position.y;
  }
  
  return hasCollision;
}

/**
 * resolveTileCollision - Resolve collision with a tile
 * 
 * @param entity The Entity that is colliding with the tile
 * @param row The row index of the tile
 * @param col The column index of the tile
 * @param layer The layer index of the tile
 * 
 * @return true if the collision was resolved, false otherwise
 */

static bool resolveTileCollision(Entity *entity, int row, int col, int layer) {
  if (!entity) return false;
  
  Map *map = &(gameState->map);
  
  if (row < 0 || row >= map->numOfRows || col < 0 || col >= map->numOfCols) {
    return false;
  }

  int tileId = map->mapIds[row][col][layer];
  if (tileId == -1) {
    return true;
  }
  
  TileProperties tileProperties = getTileProperties(tileId);
  EntityType entityType = entity->type;
  
  switch (entityType) {
    case ENTITY_PLAYER:
      Player *player = entity->entity.player;
      if (isTileCollectible(tileId) && addToInventory(player, tileId)) {
        playSoundEffect("pickup");
        removePickable(row, col);
        return true;
      }
      if (isTileInteractive(tileId)) {
        player->interactableTileIndex = row * map->numOfCols + col;
        return true;
      }
      break;
    case ENTITY_P_COMBAT_ACTION:
    case ENTITY_E_COMBAT_ACTION:
      if (isPropertySet(tileProperties, TILE_PROP_SOLID)) {
        // Handle combat action collision with solid tiles
        CombatAction *action = entity->entity.action;
        if (action->type == ACTION_BULLET) {
          Bullet *bullet = &(action->action.bullet);
          bullet->bulletInfo.bulletHealth = 0;
        }
      }
      break;
    default:
      break;
  }
  return false;
}