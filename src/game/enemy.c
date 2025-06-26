/***************************************************************
 *
 *
 *
 *    ███████╗███╗   ██╗███████╗███╗   ███╗██╗   ██╗
 *    ██╔════╝████╗  ██║██╔════╝████╗ ████║╚██╗ ██╔╝
 *    █████╗  ██╔██╗ ██║█████╗  ██╔████╔██║ ╚████╔╝
 *    ██╔══╝  ██║╚██╗██║██╔══╝  ██║╚██╔╝██║  ╚██╔╝
 *    ███████╗██║ ╚████║███████╗██║ ╚═╝ ██║   ██║
 *    ╚══════╝╚═╝  ╚═══╝╚══════╝╚═╝     ╚═╝   ╚═╝
 *
 *     Enemy Module Header. (Game Object)
 *     Exposes the logic for the enemy object.
 *
 *     - Setup Enemy
 *     - Update Enemy
 *     - Draw Enemy
 *     - Clear Enemy
 *
 **************************************************************/

#include "enemy.h"
#include "../system/anime.h"
#include "../system/A-Star.h"
#include "../system/map.h"

#include <raylib.h>
#include <raymath.h>

// ***************************
// Private Function Prototypes
// ***************************
static Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, Vector2 position);
static void deleteEnemy(Enemy **enemy);
static void clearEnemy(Enemy **enemy);

/**
 * setupEnemies - initialize all enemies manually to the game state
 */
void setupEnemies()
{
  const Settings *const settings = &(gameState->settings);
  initEnemy(E_CIVILIAN, E_SWORD, (Vector2){200, 978});
  initEnemy(E_FARMER, E_SWORD, (Vector2){1190.46, 356.01});
  //! This sets the initial path to the player
  updateEnemyPath(true);
}

/**
 * drawEnemies - draw enemies on the scene with debug path visualization
 */
void drawEnemies()
{
  Enemy *enemies = gameState->enemies;
  int enemy_num = gameState->numOfEnemies;
  GameState *game_system = gameState;

  if (enemies == NULL)
    return;

  for (int i = 0; i < enemy_num; i++)
  {
    // Draw the enemy sprite
    bool flip = (enemies[i].drawDirection == -1) ? true : false;
    drawAnimator(&(enemies[i].object.animator), &(enemies[i].object.transform), WHITE, flip);
  }
}

/**
 * updateEnemies - update the enemies objects every game tick
 */
void updateEnemies()
{
  Enemy *enemies = gameState->enemies;

  updateEnemyPath(false);

  Vector2 direction = {0, 0};

  for (int i = 0; i < gameState->numOfEnemies; i++)
  {
    updateAnimator(&enemies[i].object.animator);
    Vector2 velocity = {0, 0};
    enemies[i].ai.state = IDLE;

    if (enemies[i].ai.path)
    {
      enemies[i].ai.state = RUN;
      // Get current target waypoint
      CoordPair nextStep = enemies[i].ai.path[enemies[i].ai.currentPathIndex];
      Vector2 enemyPos = enemies[i].object.transform.position;
      int colliderWidth = enemies[i].object.collider.bounds.width;
      int colliderHeight = enemies[i].object.collider.bounds.height;
      int cellWidth = gameState->map.tileWidth * gameState->map.scale;
      int cellHeight = gameState->map.tileHeight * gameState->map.scale;

      // Calculate cell boundaries
      Rectangle cellBounds = {
          nextStep.second * cellWidth, // x position
          nextStep.first * cellHeight, // y position
          cellWidth,                   // width
          cellHeight                   // height
      };

      Vector2 enemyCenterPos = {
          enemyPos.x + colliderWidth / 2,
          enemyPos.y + colliderHeight / 2};

      Vector2 nextCellPos = {
          nextStep.second * cellWidth + cellWidth / 2,
          nextStep.first * cellHeight + cellHeight / 2};
      direction = Vector2Subtract(nextCellPos, enemyCenterPos);

      // Debug the distance
      float distance = Vector2Length(direction);

      bool insideCell =
          enemyCenterPos.x >= cellBounds.x &&
          enemyCenterPos.x <= cellBounds.x + cellBounds.width &&
          enemyCenterPos.y >= cellBounds.y &&
          enemyCenterPos.y <= cellBounds.y + cellBounds.height;

      // Move to next waypoint if inside the cell
      if (insideCell)
      {

        enemies[i].ai.currentPathIndex++;
        // If we still have waypoints, calculate new direction
        if (enemies[i].ai.currentPathIndex < enemies[i].ai.pathLength)
        {
          nextStep = enemies[i].ai.path[enemies[i].ai.currentPathIndex];
          nextCellPos = (Vector2){
              nextStep.second * cellWidth + cellWidth / 2,
              nextStep.first * cellHeight + cellHeight / 2};
          direction = Vector2Subtract(nextCellPos, enemyCenterPos);
        }
        else
        {
          // Path complete
          free(enemies[i].ai.path);
          enemies[i].ai.path = NULL;
          enemies[i].ai.currentPathIndex = 0;
          enemies[i].ai.pathLength = 0;
          direction = (Vector2){0, 0};
        }
      }

      if (Vector2Length(direction) > 0)
      {
        // Normalize direction and scale by speed
        velocity = Vector2Scale(Vector2Normalize(direction), enemies[i].ai.speed);
      }
    }

    Vector2 position = Vector2Add(enemies[i].object.transform.position, velocity);

    if (Vector2Length(velocity) > 0)
    {
      enemies[i].isMoving = true;
    }
    else
    {
      enemies[i].isMoving = false;
    }

    if (velocity.x < 0)
    {
      enemies[i].drawDirection = -1;
    }
    else if(velocity.x > 0)
    {
      enemies[i].drawDirection = 1;
    }

    // NOTE: this makes the player unable to go out of frame
    enemies[i].object.rigidBody.velocity = velocity;
    // TODO: enemy clamping is removed for now restore later
    //  enemy->object.transform.position =
    //  Vector2Clamp(position, (Vector2){0, 0},
    //  (Vector2){GetScreenWidth() - 64, GetScreenHeight() - 64});
    enemies[i].object.transform.position = position;
    enemies[i].object.collider.bounds.x = position.x;
    enemies[i].object.collider.bounds.y = position.y;

    // FIXME: replace with sprite size

    // Remove Enemy when it dies
    if (enemies[i].stats.health.currentHealth <= 0)
    {
      printf("Killed Enemy\n");
      Enemy *enemy = enemies + i;
      deleteEnemy(&enemy);
      i--;
    }
  }
}

/**
 * updateEnemyPath - update the path of all enemies to the closest
 **/
void updateEnemyPath(bool forceUpdate)
{
  GameState *game_system = gameState;
  if (!game_system || game_system->numOfPlayers <= 0 || (game_system->players[0].isMoving == false && !forceUpdate))
    return;

  Map *map = &(game_system->map);
  Player *player = &(game_system->players[0]);

  // Convert player position to grid coordinates
  Vector2 playerPos = player->object.transform.position;
  int colliderWidth = player->object.collider.bounds.width;
  int colliderHeight = player->object.collider.bounds.height;
  playerPos.x += colliderWidth / 2;
  playerPos.y += colliderHeight / 2;
  int playerRow = (int)((playerPos.y) / (map->tileHeight * map->scale));
  int playerCol = (int)((playerPos.x) / (map->tileWidth * map->scale));

  // Define directions for surrounding cells (8 directions)
  int dx[] = {-1, 0, 1, 0, -1, -1, 1, 1};
  int dy[] = {0, -1, 0, 1, -1, 1, -1, 1};

  // Find all walkable cells surrounding the player
  CoordPair surroundingCells[8];
  int numSurroundingCells = 0;

  for (int i = 0; i < 8; i++)
  {
    int newRow = playerRow + dy[i];
    int newCol = playerCol + dx[i];

    // Check if cell is valid and walkable
    if (isValid(newRow, newCol, map->numOfRows, map->numOfCols) && isWalkable(newRow, newCol))
    {
      surroundingCells[numSurroundingCells].first = newRow;
      surroundingCells[numSurroundingCells].second = newCol;
      numSurroundingCells++;
    }
  }

  // If no surrounding cells are available, use the player's cell
  if (numSurroundingCells == 0)
  {
    surroundingCells[0].first = playerRow;
    surroundingCells[0].second = playerCol;
    numSurroundingCells = 1;
  }

  // For each enemy, calculate and draw path to closest surrounding cell
  for (int i = 0; i < game_system->numOfEnemies; i++)
  {
    Enemy *enemy = &(game_system->enemies[i]);

    Vector2 enemyPos = enemy->object.transform.position;
    colliderWidth = enemy->object.collider.bounds.width;
    colliderHeight = enemy->object.collider.bounds.height;
    
    enemyPos.x += colliderWidth / 2;
    enemyPos.y += colliderHeight / 2;

    int corners[4][2] = {
        {enemyPos.x - colliderWidth / 2, enemyPos.y - colliderHeight / 2}, // Top-left
        {enemyPos.x + colliderWidth / 2, enemyPos.y - colliderHeight / 2}, // Top-right
        {enemyPos.x - colliderWidth / 2, enemyPos.y + colliderHeight / 2}, // Bottom-left
        {enemyPos.x + colliderWidth / 2, enemyPos.y + colliderHeight / 2}  // Bottom-right
    };

    float distance = INT_MAX;

    for(int j = 0; j < 4; j++){
      // Calculate distance from enemy to player corners
      int cornerX = corners[j][0];
      int cornerY = corners[j][1];
      distance = fmin(distance,Vector2Distance((Vector2){cornerX, cornerY}, playerPos));
    }

    if (distance > enemy->ai.detectionRange)
    {
      free(enemy->ai.path);
      enemy->ai.path = NULL;
      enemy->ai.pathLength = 0;
      enemy->ai.currentPathIndex = 0;
      continue;
    }

    // Convert enemy position to grid coordinates
    int enemyRow = (int)((enemyPos.y) / (map->tileHeight * map->scale));
    int enemyCol = (int)((enemyPos.x) / (map->tileWidth * map->scale));
    CoordPair enemyCoord = {.first = enemyRow, .second = enemyCol};

    // Find path to each surrounding cell and keep the shortest one
    CoordPair *shortestPath = NULL;
    int shortestPathLength = 0;

    for (int j = 0; j < numSurroundingCells; j++)
    {
      int pathLength = 0;
      CoordPair *path = aStarSearch(enemyCoord, surroundingCells[j], &pathLength);

      if (path && (shortestPath == NULL || pathLength < shortestPathLength))
      {
        if (shortestPath)
        {
          free(shortestPath);
        }
        shortestPath = path;
        shortestPathLength = pathLength;
      }
      else if (path)
      {
        free(path);
      }
    }

    if (enemy->ai.path)
    {
      free(enemy->ai.path);
    }
    enemy->ai.path = shortestPath;
    enemy->ai.pathLength = shortestPathLength;
    enemy->ai.currentPathIndex = 1;
  }
}

/**
 * clearEnemies - free enemies array from heap
 */
void clearEnemies()
{
  int enemyNum = gameState->numOfEnemies;
  Enemy *enemies = gameState->enemies;

  // FIXME: this is probably buged
  printf("Deleting Enemies\n");
  while (enemyNum--)
  {
    clearEnemy(&enemies);
    enemies--;
  }
  free(gameState->enemies);
  printf("Deleted all Enemies\n");
}

// *****************
// PRIVATE FUNCTIONS
// *****************

/**
 * initEnemy - initialize a new enemy object and add it to the game sate
 *
 * @param type The enemy type
 * @param weapon The enemy weapon
 * @param position The enemy spawn position
 *
 * @return Pointer to the new enemy object
 */
static Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, Vector2 position)
{
  static unsigned int ID = 0;
  Dictionary *dict = gameState->enemyDictionary;
  Enemy *enemy = &(gameState->enemies[gameState->numOfEnemies++]);
  int l = 0, r = NUM_OF_E_TYPE - 1;

  while (l <= r)
  {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - type;
    if (!cmp)
    {
      *enemy = dict[mid].entry.enemy;
      printf("Added enemy of type: %s\n", enemy->name);
      break;
    }
    if (cmp < 0)
      l = mid + 1;
    else
      r = mid - 1;
  }

  enemy->object.animator = (Animator){
      .isFinished = false,
      .currentState = IDLE,
  };

  switch (type)
  {
  case E_CIVILIAN:
    enemy->object.animator.animations[IDLE] = (SpriteAnimation){
        .frameNames =
            {
                "vampire_1",
                "vampire_2",
                "vampire_3",
                "vampire_4",
            },
        .numOfFrames = 4,
        .fps = 8,
        .isLooping = true,
        .isFinished = false,
        .currentFrame = 0,
        .frameCount = 0,
    };
    break;
  case E_FARMER:
  default:
    enemy->object.animator.animations[IDLE] = (SpriteAnimation){
        .frameNames =
            {
                "skeleton_1",
                "skeleton_2",
                "skeleton_3",
                "skeleton_4",
            },
        .numOfFrames = 4,
        .fps = 8,
        .isLooping = true,
        .isFinished = false,
        .currentFrame = 0,
        .frameCount = 0,
    };
    break;
  }

  enemy->ID = ID++;
  enemy->type = type;
  enemy->object.transform.position = position;
  enemy->object.collider.bounds.x = position.x;
  enemy->object.collider.bounds.y = position.y;
  enemy->object.collider.bounds.width = 48;
  enemy->object.collider.bounds.height = 48;
  enemy->object.transform.scale = (Vector2){4, 4};
  enemy->weapon = initWeapon(weapon, false);
  enemy->ai = (EnemyAI){
      .patrolStart = (Vector2){0, 0},
      .patrolEnd = (Vector2){0, 0},
      .detectionRange = 200.0f,
      .attackCooldown = 1.0f,
      .lastAttackTime = 0.0f,
      .dodgePercentage = 0.1f,
      .speed = 2.5f,
      .state = IDLE,
      .path = NULL,
      .currentPathIndex = 0,
      .pathLength = 0,
  };

  return enemy;
}

/**
 * deleteEnemy - remove the enemy object from the game state
 *
 * @param enemy Pointer to a pointer to the enemy object
 */
static void deleteEnemy(Enemy **enemy)
{
  Enemy *enemies = gameState->enemies;
  *enemy = (enemies + --(gameState->numOfEnemies));
  // TODO: rework clearEnemy and call it
  //  clearEnemy(enemy);
}

/**
 * clearEnemy - free the enemy object from the memory
 */
static void clearEnemy(Enemy **enemy)
{
  if (enemy == NULL || *enemy == NULL)
    return;
}
