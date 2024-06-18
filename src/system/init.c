#include "init.h"

//========================================================
// LOCAL VARIABLE DEFINITIONS (local to this file)
//========================================================
static void initEnemyDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary) * (NUM_OF_E_TYPE));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = E_CIVILIAN;
  dict[0].entry.enemy = (Enemy){
      .name = "Civilian",
      .object =
          {
              .rigidBody = {.velocity = (Vector2){5, 5},
                            .acceleration = (Vector2){0, 0},
                            1.0,
                            false},
              .collider = {.offset = (Vector2){0, 0}, 32, 32},
              .spriteRenderer = {.texture = LoadTexture("./src/"), 32, 32},
              .animator = {0},
          },
      .ai = {.detectionRange = 100,
             .attackCooldown = 3,
             .dodgePercentage = 0,
             .speed = 2,
             .state = FLEE},
      .stats = {.health = {.maxHealth = 100, .currentHealth = 100},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 3, .nearHitValue = 6}},
  };

  dict[1].opcode = E_FARMER;
  dict[1].entry.enemy = (Enemy){
      .name = "Farmer",
      .object =
          {
              .rigidBody = {.velocity = (Vector2){5, 5},
                            .acceleration = (Vector2){0, 0},
                            1.0,
                            false},
              .collider = {.offset = (Vector2){0, 0}, 32, 32},
              .spriteRenderer = {.texture = LoadTexture("./src/"), 32, 32},
              .animator = {},
          },
      .ai = {.detectionRange = 100,
             .attackCooldown = 3,
             .dodgePercentage = 0,
             .speed = 2,
             .state = FLEE},
      .stats = {.health = {.maxHealth = 100, .currentHealth = 100},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 3, .nearHitValue = 6}}};

  dict[2].opcode = E_KNIGHT;

  gameState->enemyDictionary = dict;
}

//========================================================
// Init Functions
//========================================================

GameState *initGameState() {
  GameState *gameSystemInstance = (GameState *)malloc(sizeof(GameState));
  if (gameSystemInstance != NULL) {
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
    // initSettings(gameSystemInstance);

    gameState->characterDictionary = NULL;
    gameState->enemyDictionary = NULL;
    gameState->playerWeaponDictionary = NULL;
    gameState->enemyWeaponDictionary = NULL;
    // initPlayerWeaponDictionary();
  }

  return gameSystemInstance;
}
void initDictionary() {
  initCharacterDictionary();
  initEnemyDictionary();
  initPlayerWeaponDictionary();
  initEnemyWeaponDictionary();
}

void initSettings() {
  gameState->settings.screenWidth = SCREEN_WIDTH;
  gameState->settings.screenHeight = SCREEN_HEIGHT;
  gameState->settings.musicVolume = 50;
  gameState->settings.soundVolume = 50;
  gameState->settings.music_on = true;
  gameState->settings.sfx_on = true;
}

static void addEnemy(Enemy *enemy) {
  Enemy *enemies = gameState->enemies;
  enemies[gameState->numOfEnemies++] = *enemy;
}

static void addBullet(Bullet *bullet, float angle) {
  CombatAction *combatActions = gameState->combatActions;
  int idx = gameState->numOfCombatActions++;
  combatActions[idx].type = ACTION_BULLET;
  combatActions[idx].action.bullet = *bullet;
  combatActions[idx].angle = angle;
}

float GetAngleBetweenPoints(Vector2 point1, Vector2 point2) {
  float deltaX = point2.x - point1.x;
  float deltaY = point2.y - point1.y;
  float angle =
      atan2f(deltaY, deltaX) * (180.0f / PI); // Convert radians to degrees
  return angle;
}

Bullet *initBullet(int ID, BulletInfo *bulletInfo, Vector2 src, Vector2 dest) {

  Bullet *bullet = (Bullet *)malloc(sizeof(Bullet));
  bullet->playerID = ID;
  bullet->bulletInfo = *bulletInfo;
  bullet->startPosition = src;
  bullet->transform = (CTransform){src, 0, (Vector2){1, 1}};
  bullet->bulletInfo.rigidBody.position = src;
  addBullet(bullet, GetAngleBetweenPoints(src, dest));
  return bullet;
}

Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, Vector2 position) {
  Dictionary *dict = gameState->enemyDictionary;
  Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
  int l = 0, r = NUM_OF_E_TYPE - 1;

  while (l <= r) {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - type;
    if (!cmp) {
      *enemy = dict[mid].entry.enemy;
      printf("Added enemy of type: %s\n", enemy->name);
      break;
    }
    if (cmp < 0)
      l = mid + 1;
    else
      r = mid - 1;
  }
  enemy->type = type;
  enemy->object.rigidBody.position = position;
  enemy->weapon = initWeapon(weapon, false);
  addEnemy(enemy);
  return enemy;
}
