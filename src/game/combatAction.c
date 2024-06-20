/***************************************************************
 *
 *
 *
 *     ██████╗ ██████╗ ███╗   ███╗██████╗  █████╗ ████████╗   █████╗  ██████╗████████╗██╗ ██████╗ ███╗   ██╗
 *    ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██╔══██╗╚══██╔══╝  ██╔══██╗██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║
 *    ██║     ██║   ██║██╔████╔██║██████╔╝███████║   ██║     ███████║██║        ██║   ██║██║   ██║██╔██╗ ██║
 *    ██║     ██║   ██║██║╚██╔╝██║██╔══██╗██╔══██║   ██║     ██╔══██║██║        ██║   ██║██║   ██║██║╚██╗██║
 *    ╚██████╗╚██████╔╝██║ ╚═╝ ██║██████╔╝██║  ██║   ██║     ██║  ██║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║
 *     ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═════╝ ╚═╝  ╚═╝   ╚═╝     ╚═╝  ╚═╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
 *
 *     Combat Action Module Header. (Game Object)
 *     Exposes the logic for the Combat Action object.
 *
 *     - Setup Combat Action
 *     - Update Combat Action
 *     - Draw Combat Action
 *     - Check for collision
 *     - Clear Combat Action
 *
 **************************************************************/

#include "combatAction.h"

#include "../system/anime.h"
// FIXME: delete me later
#include "../system/init.h"
#include <raymath.h>

// ***************************
// Private Function Prototypes
// ***************************
static bool checkCollision(Rectangle rect1, Rectangle rect2);
static bool bulletCollision(CombatAction *combatAction);
static bool slashCollision(CombatAction *combatAction);
static void drawBullet(CombatAction **combatAction);
static void drawSlash(CombatAction **combatAction);
static void clearCombatAction(CombatAction **combatAction);

/**
 *  initBullet - initialize a bullet object
 *
 * @param ID Player's ID
 * @param bulletInfo Bullet's information from the used weapon
 *
 * @return Pointer to the combat action object
 *
 * @details Initialize a bullet object and link it to the player
 * by `ID`, its information is provided by the fired weapon.
 *
 */
CombatAction *initBullet(int ID, BulletInfo bulletInfo, Vector2 pathInfo, Vector2 src, Vector2 dest)
{
  if (gameState->numOfCombatActions == DEFAULT_MAX_COMBAT_ACTIONS)
    return NULL;
  // Init bullet
  Bullet bullet;
  bullet.playerID = ID;
  bullet.bulletInfo = bulletInfo;
  bullet.startPosition = src;
  bullet.transform = (CTransform){src, 0, pathInfo.x, pathInfo.y, (Vector2){1, 1}};
  if (bullet.bulletInfo.isTracking && bullet.bulletInfo.enemyID >= 0)
    bullet.dest = gameState->enemies[bullet.bulletInfo.enemyID].object.transform.position;
  else
    bullet.dest = dest;

  // Init combatAction
  CombatAction *combatAction = (gameState->combatActions + gameState->numOfCombatActions++);
  combatAction->angle = GetAngleBetweenPoints(src, bullet.dest);
  combatAction->type = ACTION_BULLET;
  combatAction->action.bullet = bullet;
  return combatAction;
}

/**
 *  initRangedWeaponShoot - initialize a ranged weapon shoot object
 *
 * @param ID Player's ID
 * @param weapon Ranged weapon used
 *
 * @return Pointer to the combat action object
 *
 * @details Initialize a ranged weapon shoot object and link it to the player
 * by `ID`, its information is provided by the used ranged weapon.
 *
 */

void initRangedWeaponShoot(int ID, RangedWeapon weapon, Vector2 src, Vector2 dest)
{
  if (gameState->numOfCombatActions == DEFAULT_MAX_COMBAT_ACTIONS)
    return;
  int numOfBullets = weapon.numBullets;
  // TODO: Refactor this part of code
  //  Vector2 freq_amp[] = {{0, 0}, {.006, 50}, {-.004, 100}, {.01, 80}, {-.01, 60}};
  //    Vector2 freq_amp[] = {{0, 0}, {.006, 50}, {-.004, 40}, {.003, 30}, {-.0051, 20}};
  Vector2 freq_amp[] = {{0, 0}, {.053, 50}, {.061, 40}, {-.031, 30}, {-.03, 20}};
  while (numOfBullets--)
  {
    initBullet(ID, weapon.bulletInfo, freq_amp[numOfBullets], src, dest);
  }
}

/**
 *  initSlash - initialize a slash object
 *
 * @param ID Player's ID
 * @param bulletInfo Slash's information from the used melee weapon
 *
 * @return Pointer to the combat action object
 *
 * @details Initialize a slash object and link it to the player
 * by `ID`, its information is provided by the used melee weapon.
 *
 */
CombatAction *initSlash(int ID, SlashInfo slashInfo, Vector2 src, Vector2 dest)
{
  if (gameState->numOfCombatActions == DEFAULT_MAX_COMBAT_ACTIONS)
    return NULL;
  // Init slash
  Slash slash;
  slash.playerID = ID;
  slash.slashInfo = slashInfo;
  slash.transform = (CTransform){src, 0, 0, 0, (Vector2){1, 1}};

  // Init combatAction
  CombatAction *combatAction = &(gameState->combatActions[gameState->numOfCombatActions++]);
  combatAction->angle = GetAngleBetweenPoints(src, dest);
  printf("%f\n", combatAction->angle);
  combatAction->type = ACTION_SLASH;
  combatAction->action.slash = slash;
  return combatAction;
}

/**
 * drawCombatActions - draw all combat actions and update them
 */
void drawCombatActions()
{
  int x = 320, y = 96;
  int actions = gameState->numOfCombatActions;
  CombatAction *combatActions = gameState->combatActions;

  SpriteAnimation fireAnime = createSpriteAnimation(6,
                                                    (char *[]){
                                                        "fire_1_0_0",
                                                        "fire_1_0_1",
                                                        "fire_1_0_2",
                                                        "fire_1_0_3",
                                                        "fire_1_0_4",
                                                        "fire_1_0_5",
                                                    },
                                                    12, true);

  for (int i = 0; i < gameState->numOfCombatActions; i++)
  {
    if (combatActions->type == ACTION_BULLET)
    {
      combatActions->anime = fireAnime;
      drawBullet(&combatActions);
    }
    combatActions++;
  }
  disposeSpriteAnimation(&fireAnime);
}

/**
 * clearCombatActions - free combat actions array from heap
 */
void clearCombatActions()
{
  printf("Deleting combat actions\n");
  int combatActionNum = gameState->numOfCombatActions;
  CombatAction *combatActions = gameState->combatActions;

  while (combatActionNum--)
  {
    clearCombatAction(&combatActions);
    combatActions++;
  }
  printf("Deleted combat actions\n");
}

// *****************
// PRIVATE FUNCTIONS
// *****************

/**
 * checkCollision - check for collision
 *
 * @param rect1 combat action boundary box
 * @param rect2 enemy boundary box
 *
 * @return true if the combat action collides, false otherwise.
 *
 */
static bool checkCollision(Rectangle rect1, Rectangle rect2)
{
  // collision x-axis?
  bool collisionX =
      rect1.x + rect1.width >= rect2.x && rect2.x + rect2.width >= rect1.x;
  // collision y-axis?
  bool collisionY =
      rect1.y + rect1.height >= rect2.y && rect2.y + rect2.height >= rect1.y;
  // collision only if on both axes
  return collisionX && collisionY;
}

/**
 * bulletCollision - check for bullet collision with the borders and enemies
 *
 * @param combatAction pointer to the combat action object holding the bullet
 *
 * @return true if the bullet collides, false otherwise.
 *
 */
static bool bulletCollision(CombatAction *combatAction)
{
  Bullet *bullet = &combatAction->action.bullet;
  Collider2D bulletCollider = bullet->bulletInfo.collider;
  Vector2 bulletPosition = RotatePoint(bullet->transform.position, bullet->startPosition, combatAction->angle * DEG2RAD);
  for (int j = 0; j < gameState->numOfEnemies; j++)
  {
    Enemy *enemy = &gameState->enemies[j];
    Vector2 enemyPosition = enemy->object.transform.position;
    Collider2D enemyCollider = enemy->object.collider;

    if (checkCollision((Rectangle){bulletPosition.x,
                                   bulletPosition.y,
                                   bulletCollider.width,
                                   bulletCollider.height},
                       (Rectangle){enemyPosition.x,
                                   enemyPosition.y,
                                   enemyCollider.width,
                                   enemyCollider.height}))
    {
      // Clear combatAction
      // TODO: decrease the bullet's health and indicate that it is currently
      // on a certain enemy (store the enemy id?) so that it affect different enemies.
      *combatAction =
          gameState->combatActions[--(gameState->numOfCombatActions)];
      // Damage Enemy
      enemy->stats.health.currentHealth -= bullet->bulletInfo.bulletDamage;
      if (enemy->stats.health.currentHealth <= 0)
      {
        gameState->enemies[j] = gameState->enemies[gameState->numOfEnemies - 1];
        gameState->numOfEnemies--;
        // TODO: add score to the player (maybe each enemy has its own score).
      }
      return true;
    }
  }
  return false;
}

/**
 * slashCollision - check for slash collision with the borders and enemies
 *
 * @param combatAction pointer to the combat action object holding the slash
 *
 * @return true if the slash collides, false otherwise.
 *
 */
static bool slashCollision(CombatAction *combatAction)
{
  Slash *slash = &combatAction->action.slash;
  for (int j = 0; j < gameState->numOfEnemies; j++)
  {
    Enemy *enemy = &gameState->enemies[j];
    Vector2 slashPosition = slash->transform.position;
    Vector2 enemyPosition = enemy->object.transform.position;
    Collider2D slashCollider = slash->slashInfo.collider;
    Collider2D enemyCollider = enemy->object.collider;

    if (checkCollision((Rectangle){slashPosition.x,
                                   slashPosition.y,
                                   slashCollider.width,
                                   slashCollider.height},
                       (Rectangle){enemyPosition.x,
                                   enemyPosition.y,
                                   enemyCollider.width,
                                   enemyCollider.height}))
    {
      // Clear combatAction
      // TODO: decrease the bullet's health and indicate that it is currently
      // on a certain enemy (store the enemy id?) so that it affect different enemies.
      *combatAction =
          gameState->combatActions[--(gameState->numOfCombatActions)];
      // Damage Enemy
      enemy->stats.health.currentHealth -= slash->slashInfo.slashDamage;
      if (enemy->stats.health.currentHealth <= 0)
      {
        gameState->enemies[j] = gameState->enemies[gameState->numOfEnemies - 1];
        gameState->numOfEnemies--;
        // TODO: add score to the player (maybe each enemy has its own score).
      }
      return true;
    }
  }
  return false;
}

static void drawBullet(CombatAction **combatActions)
{
  CombatAction *combatAction = *combatActions;
  Bullet *bullet = &(combatAction->action.bullet);
  Vector2 *pos = &(bullet->transform.position); // position of the bullet in the direction of +ve x-axis
  Vector2 rotated;
  if (bullet->bulletInfo.isTracking && bullet->bulletInfo.enemyID >= 0)
  {
    Vector2 enemyPos = gameState->enemies[bullet->bulletInfo.enemyID].object.transform.position;
    if (!Vector2Equals(bullet->dest, enemyPos))
    {
      // first we get the true position of the bullet on the line between the start and the destination of the bullet.
      // we can do this by rotate the point on the x-axis by the angle between the start and the destination of the bullet.
      // we don't care about the y-axis because it is function of x so we want to modify the x only
      // be careful that we are not on the origin so we have to translate it to the new origin with is the start position of the bullet
      // x coordinate is already translated to the new origin but the y we have to put it with the bullet->startPosition.y instead of 0
      // then we change the destination to the new destination
      // the new x will be the current position of the bullet (after apply the rotation to the x-axis) same for the new start position
      // after that we calculate the new angle between the new start and the new destination
      rotated = RotatePoint((Vector2){pos->x, bullet->startPosition.y}, bullet->startPosition, combatAction->angle * DEG2RAD);
      bullet->dest = enemyPos;
      pos->x = rotated.x;
      bullet->startPosition = rotated;
      combatAction->angle = GetAngleBetweenPoints(bullet->startPosition, bullet->dest);
    }
  }

  rotated = RotatePoint(*pos, bullet->startPosition, combatAction->angle * DEG2RAD); // the true position of the bullet that it should be drawn at.
  Rectangle dest = (Rectangle){rotated.x, rotated.y, bullet->bulletInfo.collider.width, bullet->bulletInfo.collider.height};
  drawSpriteAnimationPro(&(combatAction->anime), dest, (Vector2){0, 0}, 0, WHITE, false);

  pos->x += bullet->bulletInfo.bulletSpeed;

  bullet->bulletInfo.bulletRange -= bullet->bulletInfo.bulletSpeed;

  Vector2 transformedDest = RotatePoint(bullet->dest, bullet->startPosition, -combatAction->angle * DEG2RAD);

  pos->y = path(pos->x, bullet->transform.frequency, bullet->transform.amplitude, transformedDest) + bullet->startPosition.y;

  if (bullet->bulletInfo.bulletRange <= 0)
  {
    *combatAction = gameState->combatActions[--(gameState->numOfCombatActions)];
    (*combatActions)--;
  }
  else
    (*combatActions) -= bulletCollision(combatAction);
}

static void drawSlash(CombatAction **combatActions)
{
  CombatAction *combatAction = *combatActions;
  Slash *slash = &combatAction->action.slash;
  Vector2 *pos = &slash->transform.position;
  Rectangle dest = {pos->x, pos->y, slash->slashInfo.collider.width,
                    slash->slashInfo.collider.height};
  drawSpriteAnimationPro(&(combatAction->anime), dest, (Vector2){0, 0}, 0, WHITE,
                         false);
  combatAction->angle++;
  (*combatActions) -= slashCollision(combatAction);
}

static void clearCombatAction(CombatAction **combatAction)
{
  if (combatAction == NULL || *combatAction == NULL)
    return;

  free(*combatAction);
  *combatAction = NULL;
}
