#include "init.h"

//========================================================
// LOCAL VARIABLE DEFINITIONS (local to this file)
//========================================================
static void initCharacterDictionary()
{
  Dictionary *dict = malloc(sizeof(Dictionary) * (NUM_OF_P_TYPE + 1));

  if (dict == NULL)
  {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }
  printf("Allocated memory for character dictionary\n");
  dict[NUM_OF_P_TYPE].opcode = -1;
  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = CAT;
  dict[0].entry.player = (Player){
      .type = CAT,
      .stats = {.health = {.maxHealth = 100, .currentHealth = 100},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 3, .nearHitValue = 6, .constant = 10},
                .speed = 5},
      .object = {
          .rigidBody = {.velocity = (Vector2){0, 0},
                        .acceleration = (Vector2){0, 0},
                        .drag = 1.0,
                        .type = BODY_GHOST},
          .collider = {.bounds = {0, 0, 64, 64}},
          .spriteRenderer = {},
          .animator = {},
      }};

  dict[1].opcode = WEREWOLF;
  dict[2].opcode = PYROMANIAC;
  dict[3].opcode = KNIGHT;

  gameState->characterDictionary = dict;
}

static void initEnemyDictionary()
{
  Dictionary *dict = malloc(sizeof(Dictionary) * (NUM_OF_E_TYPE + 1));

  if (dict == NULL)
  {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }
  printf("Allocated memory for enemy dictionary\n");
  dict[NUM_OF_E_TYPE].opcode = -1;
  // Add in asc order
  // Because we fetch the info using binary search.
  dict[E_CIVILIAN].opcode = E_CIVILIAN;
  dict[E_CIVILIAN].entry.enemy = (Enemy){
      .name = "Civilian",
      .object =
          {
              .rigidBody = {.velocity = (Vector2){5, 5},
                            .acceleration = (Vector2){0, 0},
                            .drag = 1.0,
                            .type = BODY_DYNAMIC},
              .collider = {.bounds = {0, 0, 64, 64}},
              .spriteRenderer = {},
              .animator = {0},
          },
      .ai = {.detectionRange = 400,
             .attackCooldown = 3,
             .dodgePercentage = 0,
             .state = IDLE},
      .stats = {.health = {.maxHealth = 100, .currentHealth = 100},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 3, .nearHitValue = 6, .constant = 50},
                .speed = 3},
  };

  dict[E_FARMER].opcode = E_FARMER;
  dict[E_FARMER].entry.enemy = (Enemy){
      .name = "Farmer",
      .object =
          {
              .rigidBody = {.velocity = (Vector2){5, 5},
                            .acceleration = (Vector2){0, 0},
                            .drag = 1.0,
                            .type = BODY_DYNAMIC},
              .collider = {.bounds = {0, 0, 64, 64}},
              .spriteRenderer = {},
              .animator = {},
          },
      .ai = {.detectionRange = 350,
             .attackCooldown = 2,
             .dodgePercentage = 0,
             .minDistanceToAttack = 0,
             .state = IDLE},
      .stats = {.health = {.maxHealth = 100, .currentHealth = 100},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 5, .nearHitValue = 6, .constant = 35},
                .speed = 3},
  };

  dict[E_KNIGHT].opcode = E_KNIGHT;

  dict[E_SLIME].opcode = E_SLIME;
  dict[E_SLIME].entry.enemy = (Enemy){
      .name = "Slime",
      .object =
          {
              .rigidBody = {.velocity = (Vector2){5, 5},
                            .acceleration = (Vector2){0, 0},
                            .drag = 1.0,
                            .type = BODY_DYNAMIC},
              .collider = {.bounds = {0, 0, 64, 64}},
              .spriteRenderer = {},
              .animator = {},
          },
      .ai = {.detectionRange = 500,
             .attackCooldown = 4,
             .dodgePercentage = 0,
             .minDistanceToAttack = 6,
             .state = IDLE},
      .stats = {.health = {.maxHealth = 150, .currentHealth = 150},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 3, .nearHitValue = 6, .constant = 15},
                .speed = 1.5},
  };

  gameState->enemyDictionary = dict;
}

static void initPlayerWeaponDictionary()
{
  Dictionary *dict = malloc(sizeof(Dictionary) * (NUM_OF_P_WEAPON + 1));

  if (dict == NULL)
  {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }
  printf("Allocated memory for player weapon dictionary\n");

  dict[NUM_OF_P_WEAPON].opcode = -1;

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = P_FIRE_BALL;
  dict[0].entry.weapon = (Weapon){
      .name = "Fire Ball",
      .type = RANGED_WEAPON,
      .weapon.ranged = {
          .stats = {10, 0.5, 0, .weaponSprite = {}},
          .bulletInfo = {.bulletSpeed = 4, .bulletDamage = 25, 
            .bulletRange = 600, .bulletHealth = 40, 
            .isTracking = false,
            .critMultiplier = 1.05,
            .critChance = 0.1,
            .object = {.collider = {.bounds = {0, 0, 28, 28}}}
            },
          .maxAmmo = 1000,
          .ammo = 1000,
          .numBullets = 1}};

  dict[1].opcode = P_LONG_SWORD;
  dict[1].entry.weapon = (Weapon){
      .name = "Long Sword",
      .type = MELEE_WEAPON,
      .weapon.melee = {
          .stats = {25, 1.2, 0, .weaponSprite = {}},
          .slashInfo = {
              .criticalChance = 0.25,
              .slashRange = 80,
              .slashDamage = 35,
              .isActive = false,
              .duration = 0.4f,
              .startTime = 0.0f,
              .object = {
                  .collider = {.bounds = {0, 0, 80, 30}},
                  .transform = {{0, 0}, 0, 0, 0, {2, 2}},
                  .rigidBody = {.type = BODY_STATIC}
              }
          }
      }};

  dict[2].opcode = P_MISSILE_LAUNCHER;
  dict[2].entry.weapon = (Weapon){
      .name = "Missile Launcher",
      .type = RANGED_WEAPON,
      .weapon.ranged = {
          .stats = {10, 1, 0, .weaponSprite = {}},
          .bulletInfo = {.bulletSpeed = 3, .bulletDamage = 10, 
            .bulletRange = 1000, .bulletHealth = 10, 
            .isTracking = true,
            .critMultiplier = 1.05,
            .critChance = 0.05,
            .object = {.collider = {.bounds = {0, 0, 16, 16}}}
            },
          .maxAmmo = 1000,
          .ammo = 1000,
          .numBullets = 4}};

  gameState->playerWeaponDictionary = dict;
}

static void initEnemyWeaponDictionary()
{
  Dictionary *dict = malloc(sizeof(Dictionary) * (NUM_OF_E_WEAPON + 1));

  if (dict == NULL)
  {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }
  printf("Allocated memory for enemy weapon dictionary\n");
  dict[NUM_OF_E_WEAPON].opcode = -1;

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = E_SWORD;
  dict[0].entry.weapon = (Weapon){
      .name = "Enemy Sword",
      .type = MELEE_WEAPON,
      .weapon.melee = {
          .stats = {10, 0.5, 0, .weaponSprite = {}},
          .slashInfo = {
              .criticalChance = 0.15,
              .slashRange = 60,
              .slashDamage = 18,
              .isActive = false,
              .duration = 0.3f,
              .startTime = 0.0f,
              .object = {
                  .collider = {.bounds = {0, 0, 60, 24}},
                  .transform = {{0, 0}, 0, 0, 0, {1.5, 1.5}},
                  .rigidBody = {.type = BODY_STATIC}
              }
          }
      }};

  dict[1].opcode = E_FIRE_BALL;
  dict[1].entry.weapon = (Weapon){
      .name = "Fire Ball",
      .type = RANGED_WEAPON,
      .weapon.ranged = {
          .stats = {10, 0.5, 0, .weaponSprite = {}},
          .bulletInfo = {.bulletSpeed = 5.5, .bulletDamage = 10, 
            .bulletRange = 600, .bulletHealth = 10, 
            .isTracking = false,
            .critMultiplier = 1.05,
            .critChance = 0.05,
            .object = {.collider = {.bounds = {0, 0, 21, 21}}}
            },
          .maxAmmo = 1000,
          .ammo = 1000,
          .numBullets = 1}};

  gameState->enemyWeaponDictionary = dict;
}

//========================================================
// Init Functions
//========================================================

GameState *initGameState()
{
  GameState *gameSystemInstance = (GameState *)malloc(sizeof(GameState));
  if (gameSystemInstance != NULL)
  {
    // Initialize Players Related Variables
    gameSystemInstance->players =
        (Player *)malloc(sizeof(Player) * DEFAULT_MAX_PLAYERS);
    gameSystemInstance->numOfPlayers = 0;

    // Initialize Enemies Related Variables
    gameSystemInstance->enemies =
        (Enemy *)malloc(sizeof(Enemy) * DEFAULT_MAX_ENEMIES);
    gameSystemInstance->numOfEnemies = 0;
    // Initialize Bullets Related Variables
    gameSystemInstance->combatActions = (CombatAction *)malloc(
        sizeof(CombatAction) * DEFAULT_MAX_COMBAT_ACTIONS);
    gameSystemInstance->numOfCombatActions = 0;

    // Initialize Other General Variables
    gameSystemInstance->level = 1;
    gameSystemInstance->isGameStarted = false;
    gameSystemInstance->isGameOver = false;
    gameSystemInstance->isFinished = false;
    gameSystemInstance->atlasImages = NULL;

    gameSystemInstance->characterDictionary = NULL;
    gameSystemInstance->enemyDictionary = NULL;
    gameSystemInstance->playerWeaponDictionary = NULL;
    gameSystemInstance->enemyWeaponDictionary = NULL;
    gameSystemInstance->settings = (Settings){};
    gameSystemInstance->camera = (Camera2D){};
  }

  return gameSystemInstance;
}

void initDictionary()
{
  initCharacterDictionary();
  initEnemyDictionary();
  initPlayerWeaponDictionary();
  initEnemyWeaponDictionary();
}

void initSettings()
{
  gameState->settings.screenWidth = SCREEN_WIDTH;
  gameState->settings.screenHeight = SCREEN_HEIGHT;
  gameState->settings.musicVolume = 50;
  gameState->settings.sfxVolume = 0.5f;
  gameState->settings.playMusic = true;
  gameState->settings.sfx_on = true;
  gameState->settings.showTextures = true;
  gameState->settings.showTexturesBounds = false;
  gameState->settings.showColliders = false;
  gameState->settings.showFPS = false;
  gameState->settings.showDebugMenu = true;
  gameState->settings.showUI = true;
  gameState->settings.zoom = 1.0;
}

static void addEnemy(Enemy *enemy)
{
  Enemy *enemies = gameState->enemies;
  enemies[gameState->numOfEnemies++] = *enemy;
}

float GetAngleBetweenPoints(Vector2 point1, Vector2 point2)
{
  float deltaX = point2.x - point1.x;
  float deltaY = point2.y - point1.y;
  float angle =
      atan2f(deltaY, deltaX) * (180.0f / PI); // Convert radians to degrees
  return angle;
}

