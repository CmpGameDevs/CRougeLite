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
static bool CheckAABBCollision(const Rectangle a, const Rectangle b);

void updateMapGrid(void)
{
  Map *map = &(gameState->map);
  GridCell **grid = &(map->grid);
  int GRID_WIDTH = map->numOfCols;
  int GRID_CELL_WIDTH = map->tileWidth;
  int GRID_HEIGHT = map->numOfRows;
  int GRID_CELL_HEIGHT = map->tileHeight;
  printf("W: %d, H: %d\n", GRID_WIDTH, GRID_HEIGHT);

  // Reset object count
  for (int i = 0; i < GRID_WIDTH; i++) {
    for (int j = 0; j < GRID_HEIGHT; j++) {
      grid[i][j].objectCount = 0;
    }
  }

  // Check Players
  Player *players = gameState->players;
  for (int k = 0; k < gameState->numOfPlayers; k++) {
    GameObject *object = &(players[k].object);
    int startX, startY, endX, endY;
    getGameObjectIndices(&startX, &startY, &endX, &endY, &(object->collider));
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &grid[i][j];
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].first = k;
          cell->objectIndices[cell->objectCount].second = ENTITY_PLAYER;
        }
      }
    }
  }

  // draw enemies colliders
  Enemy *enemies = gameState->enemies;
  for (int k = 0; k < gameState->numOfPlayers; k++) {
    GameObject *object = &(enemies[k].object);
    int startX, startY, endX, endY;
    getGameObjectIndices(&startX, &startY, &endX, &endY, &(object->collider));
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &grid[i][j];
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].first = k;
          cell->objectIndices[cell->objectCount].second = ENTITY_PLAYER;
        }
      }
    }
  }

  // draw bullet actions colliders
  CombatAction *actions = gameState->combatActions;
  for (int k = 0; k < gameState->numOfPlayers; k++) {
    GameObject *object = NULL;
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
    int startX, startY, endX, endY;
    getGameObjectIndices(&startX, &startY, &endX, &endY, &(object->collider));
    for (int i = startY; i <= endY; i++) {
      for (int j = startX; j <= endX; j++) {
        GridCell *cell = &grid[i][j];
        if (cell->objectCount < MAX_OBJECTS_PER_CELL) {
          cell->objectIndices[cell->objectCount].first = k;
          cell->objectIndices[cell->objectCount].second = ENTITY_PLAYER;
        }
      }
    }
  }
}

void resolveCollision(GameObject *a, GameObject *b)
{

}

void updateGameObjectPosition(GameObject *a, Vector2 newPosition)
{
  
}

static void getGameObjectIndices(int *startX, int *startY, int *endX, int *endY, Collider2D *collider)
{
  *startX = collider->bounds.x / GRID_CELL_WIDTH;
  *startY = collider->bounds.y / GRID_CELL_HEIGHT;
  *endX = (collider->bounds.x + collider->bounds.width) / GRID_CELL_WIDTH;
  *endY = (collider->bounds.y + collider->bounds.height) / GRID_CELL_HEIGHT;
}

static bool CheckAABBCollision(const Rectangle a, const Rectangle b)
{
  return (a.x < b.x + b.width &&
          a.x + a.width > b.x &&
          a.y < b.y + b.height &&
          a.y + a.height > b.y);
}