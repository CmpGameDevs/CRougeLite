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

#include <raylib.h>
#include <raymath.h>

// ***************************
// Private Function Prototypes
// ***************************
static Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, Vector2 position);
static void clearEnemy(Enemy **enemy);

/**
 * setupEnemies - initialize all enemies manually to the game state
 */
void setupEnemies()
{
  const Settings *const settings = &(gameState->settings);
  initEnemy(E_CIVILIAN, E_SWORD, (Vector2){128, 128});

  initEnemy(E_FARMER, E_SWORD, (Vector2){GetScreenWidth() - 128 - 64, 128});
}

/**
 * drawEnemies - draw enemies on the scene
 */
void drawEnemies()
{
  Enemy *enemies = gameState->enemies;
  int enemy_num = gameState->numOfEnemies;

  if (enemies == NULL)
    return;

  while (enemy_num--)
  {
    bool flip = (enemies->drawDirection == -1) ? true : false;
    drawAnimator(&enemies->object.animator, &enemies->object.transform, WHITE,
                 flip);
    enemies++;
  }
}

/**
 * updateEnemies - update the enemies objects every game tick
 */
void updateEnemies()
{
  Enemy *enemies = gameState->enemies;
  double speed = enemies->stats.speed;

  Vector2 direction = {0, 0};
  if (IsKeyDown(KEY_UP))
    direction.y -= 1;
  if (IsKeyDown(KEY_DOWN))
    direction.y += 1;
  if (IsKeyDown(KEY_LEFT))
    direction.x -= 1;
  if (IsKeyDown(KEY_RIGHT))
    direction.x += 1;

  for (int i = 0; i < gameState->numOfEnemies; i++)
  {
    updateAnimator(&enemies[i].object.animator);

    Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed);

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
    else
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
      deleteEnemy(&(enemies + i));
      i--;
    }
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

  enemy->type = type;
  enemy->object.transform.position = position;
  enemy->object.collider.bounds.x = position.x;
  enemy->object.collider.bounds.y = position.y;
  enemy->object.transform.scale = (Vector2){4, 4};
  enemy->weapon = initWeapon(weapon, false);
  return enemy;
}

/**
 * deleteEnemy - remove the enemy object from the game state
 */
static void deleteEnemy(Enemy **enemy)
{
  Enemy *enemies = &(gameState->enemies);
  *enemy = enemies[--(gameState->numOfEnemies)];
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

  free(*enemy);
  *enemy = NULL;
}
