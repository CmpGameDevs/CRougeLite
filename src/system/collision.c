#include "collision.h"
#include <raylib.h>

// ***************************
// Local Global Variables
// ***************************
static int GRID_WIDTH, GRID_CELL_WIDTH, GRID_HEIGHT, GRID_CELL_HEIGHT;

// ***************************
// Private Function Prototypes
// ***************************
static void getGameObjectIndices(int *startX, int *startY, int *endX, int *endY, Collider2D *collider);
static void narrowPhaseCollision(void);
static GameObject *getGameObjectByIndices(Pair index);
static bool checkAABBCollision(const Rectangle a, const Rectangle b);

void updateMapGrid(void)
{
  printf("Updating Map Grid\n");
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
    printf("%d %d %d %d\n", startX, startY, endX, endY);
    printf("W: %d, H: %d\n", GRID_WIDTH, GRID_HEIGHT);
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &(map->grid[i][j]);
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].first = (void *)k;
          cell->objectIndices[cell->objectCount++].second = (void *)ENTITY_PLAYER;
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
    printf("%d %d %d %d\n", startX, startY, endX, endY);
    printf("W: %d, H: %d\n", GRID_WIDTH, GRID_HEIGHT);
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &(map->grid[i][j]);
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].first = (void *)k;
          cell->objectIndices[cell->objectCount++].second = (void *)ENTITY_ENEMY;
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
    printf("%d %d %d %d\n", startX, startY, endX, endY);
    printf("W: %d, H: %d\n", GRID_WIDTH, GRID_HEIGHT);
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &(map->grid[i][j]);
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].first = (void *)k;
          cell->objectIndices[cell->objectCount++].second = (void *)ENTITY_COMBAT_ACTION;
        }
      }
    }
  }
  narrowPhaseCollision();
  printf("Finished Map Grid\n");
}

void resolveCollision(GameObject *a, GameObject *b)
{
  a->collider.isColliding = true;
  b->collider.isColliding = true;
}

void updateGameObjectPosition(GameObject *a, Vector2 newPosition)
{

}

// *****************
// PRIVATE FUNCTIONS
// *****************
static void getGameObjectIndices(int *startX, int *startY, int *endX, int *endY, Collider2D *collider)
{
  *startX = collider->bounds.x / GRID_CELL_WIDTH;
  *startY = collider->bounds.y / GRID_CELL_HEIGHT;
  *endX = (collider->bounds.x + collider->bounds.width) / GRID_CELL_WIDTH;
  *endY = (collider->bounds.y + collider->bounds.height) / GRID_CELL_HEIGHT;
}

static void narrowPhaseCollision(void)
{
  Map *map = &(gameState->map);
  // Reset object count
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      GridCell *cell = &(map->grid[i][j]);
      int objectCount = cell->objectCount;
      for (int n = 0; n < objectCount; n++) {
        Pair indexA = cell->objectIndices[n];
        GameObject *objectA = getGameObjectByIndices(indexA);
        for (int m = n + 1; m < objectCount; m++) {
          Pair indexB = cell->objectIndices[m];
          GameObject *objectB = getGameObjectByIndices(indexB);

          if (checkAABBCollision(objectA->collider.bounds, objectB->collider.bounds)) {
            resolveCollision(objectA, objectB);
          }
        }
      }
    }
  }
}

static GameObject *getGameObjectByIndices(Pair index)
{
  Player *players = gameState->players;
  Enemy *enemies = gameState->enemies;
  CombatAction *actions = gameState->combatActions;

  switch ((EntityType)(index.second))
  {
  case ENTITY_PLAYER:
    return &(players[(int)(index.first)].object);
  case ENTITY_ENEMY:
    return &(enemies[(int)(index.first)].object);
  case ENTITY_COMBAT_ACTION:
    switch (actions[(int)(index.first)].type)
    {
    case ACTION_BULLET:
      return &(actions[(int)(index.first)].action.bullet.bulletInfo.object);
    case ACTION_SLASH:
      return &(actions[(int)(index.first)].action.slash.slashInfo.object);
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