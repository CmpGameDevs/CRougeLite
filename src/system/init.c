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
                .defense = {.value = 3, .nearHitValue = 6},
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
  dict[0].opcode = E_CIVILIAN;
  dict[0].entry.enemy = (Enemy){
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
      .ai = {.detectionRange = 100,
             .attackCooldown = 3,
             .dodgePercentage = 0,
             .speed = 2,
             .state = IDLE},
      .stats = {.health = {.maxHealth = 100, .currentHealth = 100},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 3, .nearHitValue = 6, .constant = 50},
                .speed = 5},
  };

  dict[1].opcode = E_FARMER;
  dict[1].entry.enemy = (Enemy){
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
      .ai = {.detectionRange = 100,
             .attackCooldown = 3,
             .dodgePercentage = 0,
             .speed = 2,
             .state = IDLE},
      .stats = {.health = {.maxHealth = 100, .currentHealth = 100},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 3, .nearHitValue = 6, .constant = 45},
                .speed = 5},
  };

  dict[2].opcode = E_KNIGHT;

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
  dict[0].opcode = P_GUN;
  dict[0].entry.weapon = (Weapon){
      .type = RANGED_WEAPON,
      .weapon.ranged = {
          .stats = {10, 0.5, 0, .weaponSprite = {}},
          .bulletInfo = {.bulletSpeed = 3, .bulletDamage = 10, 
            .bulletRange = 600, .bulletHealth = 20, 
            .isTracking = false,
            .critMultiplier = 1.05,
            .critChance = 0.05,
            .object = {.collider = {.bounds = {0, 0, 32, 32}}}
            },
          30,
          30,
          1}};

  dict[1].opcode = P_MISSILE_LAUNCHER;
  dict[1].entry.weapon = (Weapon){
      .type = RANGED_WEAPON,
      .weapon.ranged = {
          .stats = {10, 1, 0, .weaponSprite = {}},
          .bulletInfo = {.bulletSpeed = 2, .bulletDamage = 10, 
            .bulletRange = 1000, .bulletHealth = 10, 
            .isTracking = true,
            .critMultiplier = 1.05,
            .critChance = 0.05,
            .object = {.collider = {.bounds = {0, 0, 16, 16}}}
            },
          30,
          30,
          4}};

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
      .type = MELEE_WEAPON,

      .weapon.melee = {
          .stats = {10, 0.5, 0, .weaponSprite = {}},
          .slashInfo = {3, 10, true, .object = {}}}};

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
  gameState->settings.soundVolume = 50;
  gameState->settings.playMusic = true;
  gameState->settings.sfx_on = true;
  gameState->settings.showTextures = true;
  gameState->settings.showTexturesBounds = false;
  gameState->settings.showColliders = false;
  gameState->settings.showFPS = false;
  gameState->settings.showDebugMenu = true;
  gameState->settings.showInventory = true;
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

