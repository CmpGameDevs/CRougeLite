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
#include "../system/collision.h"
#include "../system/draw.h"
// FIXME: delete me later
#include "../system/init.h"
#include <math.h>
#include <raymath.h>
#include <stdbool.h>

// ***************************
// Local Global Variables
// ***************************
static unsigned int actionID = 0;

// ***************************
// Private Function Prototypes
// ***************************
static void checkHitObject(CombatAction *action);
static float calculateDamageTaken(float damage, Defense defense);
static void applyBulletDamage(BulletInfo *bulletInfo, Stats *stats);
static void damageEntity(CombatAction *action, Stats *stats, GameObject *object);
static void resolveTwoCombatActionsCollision(CombatAction *a, CombatAction *b);
static void drawBullet(CombatAction **combatAction);
static void drawSlash(CombatAction **combatAction);
static void clearCombatAction(CombatAction **combatAction);

/**
 *  initBullet - initialize a bullet object
 *
 * @param ID Player's ID
 * @param bulletInfo Bullet's information from the used weapon
 * @param pathInfo Path code for different path types
 * @param src Spawn position of bullet
 * @param dest The mouse click position
 * @param isFriendly Is the object shot by players or not
 * 
 * @return Pointer to the combat action object
 *
 * @details Initialize a bullet object and link it to the player or enemy
 * by `ID`, its information is provided by the fired weapon.
 *
 */
CombatAction *initBullet(int ID, BulletInfo bulletInfo, Vector2 pathInfo,
                         Vector2 src, Vector2 dest, bool isFriendly)
{
  if (gameState->numOfCombatActions == DEFAULT_MAX_COMBAT_ACTIONS)
    return NULL;
  // Init bullet
  Bullet bullet;
  GameObject *object = &(bulletInfo.object);
  bulletInfo.isFriendly = isFriendly;
  src.x -= object->collider.bounds.width / 2;
  src.y -= object->collider.bounds.height / 2;
  bullet.playerID = ID;
  bullet.startPosition = src;
  object->collider.bounds.x = src.x;
  object->collider.bounds.y = src.y;
  object->transform =
      (CTransform){src, 0, pathInfo.x, pathInfo.y, isFriendly ? (Vector2){3, 3} : (Vector2){2, 2}};

  if (bulletInfo.isTracking && bulletInfo.targetID >= 0) {
    GameObject *targetObject = NULL;
    if (isFriendly) {
      targetObject = &(gameState->enemies[bulletInfo.targetID].object);
    }
    else {
      targetObject = &(gameState->players[bulletInfo.targetID].object);
    }
    bullet.dest = (Vector2){
        targetObject->collider.bounds.x +
            targetObject->collider.bounds.width / 2,
        targetObject->collider.bounds.y +
            targetObject->collider.bounds.height / 2};
  } else {
    bullet.dest = dest;
  }

  object->rigidBody.type = BODY_DYNAMIC;
  object->animator = (Animator){
      .isFinished = false,
      .currentState = IDLE,
  };

  if (isFriendly) {
    object->animator.animations[IDLE] = (SpriteAnimation){
        .frameNames =
            {
                "fire_1_0_0",
                "fire_1_0_1",
                "fire_1_0_2",
                "fire_1_0_3",
                "fire_1_0_4",
                "fire_1_0_5",
            },
        .numOfFrames = 6,
        .fps = 16,
        .isLooping = true,
        .isFinished = false,
        .currentFrame = 0,
        .frameCount = 0,
    };
  } else {
    object->animator.animations[IDLE] = (SpriteAnimation){
        .frameNames =
            {
                "fire_2_0_0",
                "fire_2_0_1",
                "fire_2_0_2",
                "fire_2_0_3",
            },
        .numOfFrames = 4,
        .fps = 8,
        .isLooping = true,
        .isFinished = false,
        .currentFrame = 0,
        .frameCount = 0,
    };
  }

  bullet.bulletInfo = bulletInfo;

  // Init combatAction
  CombatAction *combatAction =
      (gameState->combatActions + gameState->numOfCombatActions++);
  combatAction->angle = GetAngleBetweenPoints(src, bullet.dest);
  combatAction->type = ACTION_BULLET;
  combatAction->action.bullet = bullet;
  combatAction->hit = initHitObject();
  combatAction->ID = actionID++;
  return combatAction;
}

/**
 *  initRangedWeaponShoot - initialize a ranged weapon shoot object
 *
 * @param ID Player's ID
 * @param weapon Ranged weapon used
 *
 * @return Pointer to the combat action object
 * @param src Spawn position of bullet
 * @param dest The mouse click position
 * @param isFriendly Is the object shot by players or not
 *
 * @details Initialize a ranged weapon shoot object and link it to the player
 * by `ID`, its information is provided by the used ranged weapon.
 *
 */
void initRangedWeaponShoot(int ID, RangedWeapon weapon, Vector2 src,
                           Vector2 dest, bool isFriendly)
{
  if (gameState->numOfCombatActions == DEFAULT_MAX_COMBAT_ACTIONS)
    return;
  int numOfBullets = weapon.numBullets;
  // TODO: Refactor this part of code
  //  Vector2 freq_amp[] = {{0, 0}, {.006, 50}, {-.004, 100}, {.01, 80}, {-.01,
  //  60}};
  //    Vector2 freq_amp[] = {{0, 0}, {.006, 50}, {-.004, 40}, {.003, 30},
  //    {-.0051, 20}};
  Vector2 freq_amp[] = {
      {0, 0}, {.053, 50}, {.061, 40}, {-.031, 30}, {-.03, 20}};
  CombatAction *action;
  while (numOfBullets--)
  {
    action = initBullet(ID, weapon.bulletInfo, freq_amp[numOfBullets], src, dest, isFriendly);
    action->isFriendly = isFriendly;
  }
}

/**
 *  initSlash - initialize a slash object
 *
 * @param ID Player's ID
 * @param slashInfo Slash's information from the used melee weapon
 * @param src Spawn position of slash
 * @param dest The target position (typically mouse click position)
 * @param isFriendly Is the object created by players or enemies
 *
 * @return Pointer to the combat action object
 *
 * @details Initialize a slash object and link it to the player/enemy
 * by `ID`, its information is provided by the used melee weapon.
 *
 */
CombatAction *initSlash(int ID, SlashInfo slashInfo, Vector2 src,
                        Vector2 dest, bool isFriendly)
{
  if (gameState->numOfCombatActions == DEFAULT_MAX_COMBAT_ACTIONS)
    return NULL;
  
  // Calculate direction and angle
  float angle = GetAngleBetweenPoints(src, dest);
  float angleRad = angle * (3.14159265f / 180.0f);
  Vector2 direction = {cosf(angleRad), sinf(angleRad)};
  
  float slashOffset = slashInfo.slashRange * 0.5f;
  Vector2 slashCenter = {
    src.x + direction.x * slashOffset,
    src.y + direction.y * slashOffset
  };
  
  // Init slash
  Slash slash;
  GameObject *object = &(slashInfo.object);
  
  float originalWidth = object->collider.bounds.width;
  float originalHeight = object->collider.bounds.height;
  
  float absCosDeg = fabsf(cosf(angleRad));
  float absSinDeg = fabsf(sinf(angleRad));
  
  float newWidth = originalWidth * absCosDeg + originalHeight * absSinDeg;
  float newHeight = originalWidth * absSinDeg + originalHeight * absCosDeg;
  
  object->collider.bounds.width = newWidth;
  object->collider.bounds.height = newHeight;
  
  object->collider.bounds.x = slashCenter.x - newWidth / 2;
  object->collider.bounds.y = slashCenter.y - newHeight / 2;
  
  slash.playerID = ID;
  object->transform = (CTransform){slashCenter, angle, 0, 0, (Vector2){4, 4}};

  // Set slash timing
  slashInfo.startTime = GetTime();
  slashInfo.isActive = true;
  if (slashInfo.duration <= 0) {
    slashInfo.duration = 0.3f;
  }
  
  object->animator = (Animator){
      .isFinished = false,
      .currentState = IDLE,
  };
  
  int attackCount = 1;
  
  if (isFriendly) {
    Player *player = &(gameState->players[ID]);
    float currentTime = GetTime();
    
    // Reset attack count if too much time has passed since last attack (combo timeout)
    if (currentTime - player->lastAttackTime > 1.8f) {
      player->attackCount = 0;
    }
    
    player->attackCount++;
    player->lastAttackTime = currentTime;
    attackCount = player->attackCount;
    
    if (attackCount > 2) {
      attackCount = 1;
      player->attackCount = 1;
    }
    
    if (attackCount == 1) {
      object->animator.animations[IDLE] = (SpriteAnimation){
          .frameNames = {
              "Slash_color4_frame1",
              "Slash_color4_frame2", 
              "Slash_color4_frame3",
              "Slash_color4_frame4",
          },
          .numOfFrames = 4,
          .fps = 12,
          .isLooping = false,
          .isFinished = false,
          .currentFrame = 0,
          .frameCount = 0,
      };
    } else {
      object->animator.animations[IDLE] = (SpriteAnimation){
          .frameNames = {
              "Slash_color4_frame5",
              "Slash_color4_frame6",
              "Slash_color4_frame7", 
              "Slash_color4_frame8",
              "Slash_color4_frame9",
          },
          .numOfFrames = 5,
          .fps = 12,
          .isLooping = false,
          .isFinished = false,
          .currentFrame = 0,
          .frameCount = 0,
      };
    }
  } else {
    Enemy *enemy = &(gameState->enemies[ID]);
    float currentTime = GetTime();
    
    // Reset attack count if too much time has passed since last attack (combo timeout)
    if (currentTime - enemy->lastAttackTime > 3.5f) {
      enemy->attackCount = 0;
    }
    
    enemy->attackCount++;
    enemy->lastAttackTime = currentTime;
    attackCount = enemy->attackCount;
    
    if (attackCount > 2) {
      attackCount = 1;
      enemy->attackCount = 1;
    }
    
    if (attackCount == 1) {
      object->animator.animations[IDLE] = (SpriteAnimation){
          .frameNames = {
              "Slash_color2_frame1",
              "Slash_color2_frame2",
              "Slash_color2_frame3", 
              "Slash_color2_frame4",
          },
          .numOfFrames = 4,
          .fps = 10,
          .isLooping = false,
          .isFinished = false,
          .currentFrame = 0,
          .frameCount = 0,
      };
    } else {
      object->animator.animations[IDLE] = (SpriteAnimation){
          .frameNames = {
              "Slash_color2_frame5",
              "Slash_color2_frame6",
              "Slash_color2_frame7",
              "Slash_color2_frame8",
              "Slash_color2_frame9",
          },
          .numOfFrames = 5,
          .fps = 10,
          .isLooping = false,
          .isFinished = false,
          .currentFrame = 0,
          .frameCount = 0,
      };
    }
  }
  
  slash.slashInfo = slashInfo;

  // Init combatAction
  CombatAction *combatAction =
      &(gameState->combatActions[gameState->numOfCombatActions++]);
  combatAction->angle = angle;
  combatAction->type = ACTION_SLASH;
  combatAction->action.slash = slash;
  combatAction->hit = initHitObject();
  combatAction->ID = actionID++;
  combatAction->isFriendly = isFriendly;
  return combatAction;
}

/**
 * updateCombatActions - update the combat actions
 */
void updateCombatActions()
{
  CombatAction *actions = gameState->combatActions;

  for (int i = 0; i < gameState->numOfCombatActions; i++)
  {
    checkHitObject(actions + i);
  }

  // Check Health and Duration
  for (int i = 0; i < gameState->numOfCombatActions; i++)
  {
    switch (actions[i].type)
    {
    case ACTION_BULLET:
      if (actions[i].action.bullet.bulletInfo.bulletHealth <= 0)
      {
        actions[i] = actions[--(gameState->numOfCombatActions)];
        i--;
      }
      break;
    case ACTION_SLASH:
      {
        SlashInfo *slashInfo = &(actions[i].action.slash.slashInfo);
        float currentTime = GetTime();
        if (!slashInfo->isActive || (currentTime - slashInfo->startTime) > slashInfo->duration)
        {
          actions[i] = actions[--(gameState->numOfCombatActions)];
          i--;
        }
      }
      break;
    default:
      break;
    }
  }
}

/**
 * resolveCombatActionCollision - resolve the collision of a combat
 * action object with other entity
 *
 * @param action Pointer to the combat action object
 * @param entity Pointer to the hit entity
 */
void resolveCombatActionCollision(CombatAction *action, Entity *entity)
{
  bool isFriendly = action->isFriendly;
  switch (entity->type)
  {
  case ENTITY_PLAYER:
    if (isFriendly)
      return;
    printf("Player #%d took", entity->ID);
    damageEntity(action, &(entity->entity.player->stats), &(entity->entity.player->object));
    break;
  case ENTITY_ENEMY:
    if (!isFriendly)
      return;
    printf("Enemy #%d took", entity->ID);
    damageEntity(action, &(entity->entity.enemy->stats), &(entity->entity.enemy->object));
    // TODO: add score to the player (maybe each enemy has its own score).
    break;
  case ENTITY_E_COMBAT_ACTION:
    if (!isFriendly)
      return;
    resolveTwoCombatActionsCollision(action, entity->entity.action);
    break;
  case ENTITY_P_COMBAT_ACTION:
    if (isFriendly)
      return;
    resolveTwoCombatActionsCollision(action, entity->entity.action);
    break;
  default:
    break;
  }
}

/**
 * drawCombatActions - draw all combat actions and update them
 */
void drawCombatActions()
{
  int x = 320, y = 96;
  int actions = gameState->numOfCombatActions;
  CombatAction *combatActions = gameState->combatActions;

  for (int i = 0; i < gameState->numOfCombatActions; i++)
  {
    if (combatActions->type == ACTION_BULLET)
    {
      drawBullet(&combatActions);
    }
    else if (combatActions->type == ACTION_SLASH)
    {
      drawSlash(&combatActions);
    }
    combatActions++;
  }
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
  free(gameState->combatActions);
  printf("Deleted combat actions\n");
}

// *****************
// PRIVATE FUNCTIONS
// *****************

/**
 * checkHitObject - check the hit object and handle any hit entities
 * 
 * @param action Pointer to the combat action
 */
static void checkHitObject(CombatAction *action)
{
  Hit *hit = &(action->hit);
  if (hit->hitCount == 0) return;

  Entity *entities = hit->entities;
  for (int i = hit->checkedCount; i < hit->hitCount; i++)
  {
    resolveCombatActionCollision(action, entities + i);
    hit->checkedCount++;
  }
}

/**
 * calculateDamageTaken - calculate the damage output with the
 * entity's defense.
 * 
 * @param damage The final damage of the combat action object
 * @param defense The entity's defense stats
 * 
 * @return The damage after considering the entity's defense
 */
static float calculateDamageTaken(float damage, Defense defense)
{
  float defenseNormalizer = defense.value + defense.constant;
  float defenseEffectiveness = defense.value / (defenseNormalizer == 0 ? 1 : defenseNormalizer);
  return damage * (1 - defenseEffectiveness);
}

/**
 * applyBulletDamage - apply the bullet's damage to the hit entity
 * and update the bullet's stats
 * 
 * @param bulletInfo Pointer to the bullet's info
 * @param stats Pointer to the entity's stats
 * 
 * @details calculate the bullet's damage after checking for critical
 * hits, defense, and variance. After that update the bullet's stats.
 */
static void applyBulletDamage(BulletInfo *bulletInfo, Stats *stats)
{
  // Determine if it's a critical hit
  float isCritical = (rand() / (float)RAND_MAX) < bulletInfo->critChance;
  float damage = bulletInfo->bulletDamage * (isCritical ? bulletInfo->critMultiplier : 1.0f);

  // Calculate the final damage taken
  float damageTaken = calculateDamageTaken(damage, stats->defense);

  // Apply random variance (±10%)
  float variance = 0.1f * damageTaken * ((rand() / (float)RAND_MAX) * 2.0f - 1.0f);
  damageTaken += variance;

  stats->health.currentHealth = fmax(0, stats->health.currentHealth - damageTaken);
  printf(" %.2f damage (%s), remaining health: %f\n", damageTaken, isCritical ? "Critical Hit" : "Normal Hit", stats->health.currentHealth);

  // Damage the bullet itself
  bulletInfo->bulletHealth = fmax(0, bulletInfo->bulletHealth - bulletInfo->bulletDamage);
}

/**
 * applySlashDamage - apply the slash's damage to the hit entity
 * 
 * @param slashInfo Pointer to the slash's info
 * @param stats Pointer to the entity's stats
 * 
 * @details calculate the slash's damage after checking for critical
 * hits, defense, and variance.
 */
static void applySlashDamage(SlashInfo *slashInfo, Stats *stats)
{
  // Determine if it's a critical hit
  float isCritical = (rand() / (float)RAND_MAX) < slashInfo->criticalChance;
  float critMultiplier = isCritical ? 1.5f : 1.0f;
  float damage = slashInfo->slashDamage * critMultiplier;

  // Calculate the final damage taken
  float damageTaken = calculateDamageTaken(damage, stats->defense);

  // Apply random variance (±10%)
  float variance = 0.1f * damageTaken * ((rand() / (float)RAND_MAX) * 2.0f - 1.0f);
  damageTaken += variance;

  stats->health.currentHealth = fmax(0, stats->health.currentHealth - damageTaken);
  printf(" %.2f damage (%s), remaining health: %f\n", damageTaken, isCritical ? "Critical Hit" : "Normal Hit", stats->health.currentHealth);
}

/**
 * damageEntity - Register damage to an entity's stats
 *
 * @param action Pointer to the combat action
 * @param stats Pointer to the entity's stats
 * @param object Pointer to the entity's game object
 */
static void damageEntity(CombatAction *action, Stats *stats, GameObject *object)
{
  // Calculate Damage Taken
  CombatActionType type = action->type;
  switch (type)
  {
  case ACTION_BULLET:
    BulletInfo *bulletInfo = &(action->action.bullet.bulletInfo);
    applyBulletDamage(bulletInfo, stats);
    printf("Bullet #%d taken %.2f damage, remaining health: %f\n", action->ID, bulletInfo->bulletDamage, bulletInfo->bulletHealth);
    stats->health.lastUpdateTime = GetTime();
    break;
  case ACTION_SLASH:
    {
      SlashInfo *slashInfo = &(action->action.slash.slashInfo);
      applySlashDamage(slashInfo, stats);
      printf("Slash #%d taken %.2f damage, remaining health: %f\n", action->ID, slashInfo->slashDamage, stats->health.currentHealth);
      stats->health.lastUpdateTime = GetTime();
    }
    break;
  default:
    break;
  }

  // Apply damage animation if applicable
  if (object == NULL) return;
  float health = stats->health.currentHealth;
  if (health > 0.0f)
    setState(&(object->animator), TAKE_DAMAGE);
  else
    setState(&(object->animator), DIE);
}

/**
 * resolveTwoCombatActionsCollision - Resolve collision between two combat actions
 *
 * @param a Pointer to the first combat action
 * @param b Pointer to the second combat action
 */
static void resolveTwoCombatActionsCollision(CombatAction *a, CombatAction *b)
{
}

/**
 * drawBullet - draw bullet on the scene
 * 
 * @param combatActions Pointer to a pointer pointing to the bullet object
 */
static void drawBullet(CombatAction **combatActions)
{
  CombatAction *combatAction = *combatActions;
  Bullet *bullet = &(combatAction->action.bullet);
  GameObject *object = &(bullet->bulletInfo.object);
  Vector2 *pos = &(object->transform.position);

  Vector2 rotated = RotatePoint(*pos, bullet->startPosition, -combatAction->angle * DEG2RAD);

  if (bullet->bulletInfo.isTracking && bullet->bulletInfo.targetID >= 0)
  {
    GameObject *targetObject = NULL;
    if (bullet->bulletInfo.isFriendly)
    {
      targetObject = &(gameState->enemies[bullet->bulletInfo.targetID].object);
    }
    else
    {
      targetObject = &(gameState->players[bullet->bulletInfo.targetID].object);
    }
    Vector2 targetPos = (Vector2){
        targetObject->collider.bounds.x +
            targetObject->collider.bounds.width / 2,
        targetObject->collider.bounds.y +
            targetObject->collider.bounds.height / 2};
    if (!Vector2Equals(bullet->dest, targetPos))
    {
      bullet->dest = targetPos;
      bullet->startPosition = RotatePoint((Vector2){rotated.x, bullet->startPosition.y}, bullet->startPosition, combatAction->angle * DEG2RAD);
      rotated.x = bullet->startPosition.x;
      combatAction->angle =
          GetAngleBetweenPoints(bullet->startPosition, bullet->dest);
    }
  }
  rotated.x += bullet->bulletInfo.bulletSpeed;

  Vector2 transformedDest = RotatePoint(bullet->dest, bullet->startPosition,
                                        -combatAction->angle * DEG2RAD);

  rotated.y = path(rotated.x, object->transform.frequency, object->transform.amplitude, transformedDest) + bullet->startPosition.y;

  bullet->bulletInfo.bulletRange -= bullet->bulletInfo.bulletSpeed;

  rotated = RotatePoint(rotated, bullet->startPosition, combatAction->angle * DEG2RAD);

  *pos = rotated;
  object->collider.bounds.x = pos->x;
  object->collider.bounds.y = pos->y;

  if (bullet->bulletInfo.bulletRange <= 0)
  {
    *combatAction = gameState->combatActions[--(gameState->numOfCombatActions)];
    (*combatActions)--;
  }

  updateAnimator(&(object->animator));
  drawAnimator(&(object->animator), &object->transform, WHITE, false);
}

/**
 * drawSlash - draw slash on the scene
 * 
 * @param combatActions Pointer to a pointer pointing to the slash object
 */
static void drawSlash(CombatAction **combatActions)
{
  CombatAction *combatAction = *combatActions;
  Slash *slash = &combatAction->action.slash;
  GameObject *object = &(slash->slashInfo.object);
  
  updateAnimator(&(object->animator));
  
  SpriteAnimation *anim = &object->animator.animations[object->animator.currentState];
  Rectangle src = getSrcRect(&object->animator);
  CTransform *transform = &object->transform;

  Rectangle dest = (Rectangle){
    transform->position.x, transform->position.y,
    src.width * transform->scale.x,
    src.height * transform->scale.y
  };
  Vector2 origin = (Vector2){
    src.width * transform->scale.x / 2,
    src.height * transform->scale.y / 2
  };
  
  drawAtlasSpritePro(anim->frameNames[anim->currentFrame],
                     dest, origin, object->transform.rotation, WHITE, false);
}

/**
 * clearCombatAction - free the combat action object from the memory 
 */
static void clearCombatAction(CombatAction **combatAction)
{
  if (combatAction == NULL || *combatAction == NULL)
    return;

  clearHitObject(&((*combatAction)->hit));
}
