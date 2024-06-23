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

#include <raymath.h>

// ***************************
// Private Function Prototypes
// ***************************
static Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, Vector2 position);
static void clearEnemy(Enemy **enemy);

/* setupPlayers
 *
 * Setup the players in the game. Create a new player and adds it to
 * the state of the game to draw and update.
 *
 */
void setupEnemies()
{
  const Settings *const settings = &(gameState->settings);
  initEnemy(E_CIVILIAN, E_SWORD, (Vector2){128, 128});

  initEnemy(E_FARMER, E_SWORD,
            (Vector2){settings->screenWidth - 128 - 64, 128});
}

/* drawPlayers
 *
 * Draw the players in the game.
 *
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

void updateEnemies()
{
  Enemy *enemy = gameState->enemies;
  double speed = enemy->stats.speed;

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
    updateAnimator(&enemy->object.animator);

    Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed);

    Vector2 position = Vector2Add(enemy->object.transform.position, velocity);

    if (Vector2Length(velocity) > 0)
    {
      enemy->isMoving = true;
    }
    else
    {
      enemy->isMoving = false;
    }

    if (velocity.x < 0)
    {
      enemy->drawDirection = -1;
    }
    else
    {
      enemy->drawDirection = 1;
    }

    // NOTE: this makes the player unable to go out of frame
    enemy->object.rigidBody.velocity = velocity;
    enemy->object.transform.position =
        Vector2Clamp(position, (Vector2){0, 0},
                     (Vector2){gameState->settings.screenWidth - 64,
                               gameState->settings.screenHeight - 64});

    // FIXME: replace with sprite size
    enemy++;
  }
}

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
  enemy->object.transform.scale = (Vector2){4, 4};
  enemy->weapon = initWeapon(weapon, false);
  return enemy;
}

static void clearEnemy(Enemy **enemy)
{
  if (enemy == NULL || *enemy == NULL)
    return;

  free(*enemy);
  *enemy = NULL;
}
