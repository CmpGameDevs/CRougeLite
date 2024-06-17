#include "init.h"

//========================================================
// LOCAL VARIABLE DEFINITIONS (local to this file)
//========================================================
static void initCharacterDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary)*(NUM_OF_P_TYPE));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = CAT;
  dict[0].entry.character = 
  (GameObject)
  {
    .rigidBody = {.velocity = (Vector2){5, 5}, .acceleration = (Vector2){0, 0}, 1.0, false},
    .collider = {.offset = (Vector2){0, 0}, 32, 32},
    .spriteRenderer = {.texture = LoadTexture("./src/"), 32, 32},
    .animator = {},
    .stats = {
      .health = {.maxHealth = 100, .currentHealth = 100},
      .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
      .defense = {.value = 3, .nearHitValue = 6}
    }
  };

  dict[1].opcode = WEREWOLF;
  dict[2].opcode = PYROMANIAC;
  dict[3].opcode = KNIGHT;

  gameState->characterDictionary = dict;
}

static void initEnemyDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary)*(NUM_OF_E_TYPE));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = E_CIVILIAN;
  dict[0].entry.enemy = (Enemy) {
    .name = "Civilian",
    .object = {
      .rigidBody = {.velocity = (Vector2){5, 5}, .acceleration = (Vector2){0, 0}, 1.0, false},
      .collider = {.offset = (Vector2){0, 0}, 32, 32},
      .spriteRenderer = {.texture = LoadTexture("./src/"), 32, 32},
      .animator = {},
      .stats = {
        .health = {.maxHealth = 100, .currentHealth = 100},
        .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
        .defense = {.value = 3, .nearHitValue = 6}
      }
    },
    .ai = {
      .detectionRange = 100,
      .attackCooldown = 3,
      .dodgePercentage = 0,
      .speed = 2,
      .state = FLEE
    }
  };

  dict[1].opcode = E_FARMER;
  dict[2].opcode = E_KNIGHT;

  gameState->enemyDictionary = dict;
}

static void initPlayerWeaponDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary)*(NUM_OF_P_WEAPON));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = P_GUN;
  dict[0].entry.weapon = (Weapon){RANGED_WEAPON, .weapon.ranged = {
      .stats = {10, 0.5, 0, .weaponSprite = {LoadTexture("./src/"), 5, 5}},
      .bulletInfo = {3, 10, 100, 10, .bulletSprite = {LoadTexture("./src/"), 5, 5}},
      30, 30}};

  gameState->playerWeaponDictionary = dict;
}

static void initEnemyWeaponDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary)*(NUM_OF_E_WEAPON));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = E_SWORD;
  dict[0].entry.weapon = (Weapon){MELEE_WEAPON, .weapon.melee = {
      .stats = {10, 0.5, 0, .weaponSprite = {LoadTexture("./src/"), 5, 5}},
      .slashInfo = {3, 10, true, .slashSprite = {LoadTexture("./src/"), 5, 5}}
      }};

  gameState->enemyWeaponDictionary = dict;
}

static Weapon initWeapon(int opcode, bool isPlayer) {
  Dictionary *dict = (isPlayer ? gameState->playerWeaponDictionary : gameState->enemyWeaponDictionary);
  int l = 0, r = (isPlayer ? NUM_OF_P_WEAPON : NUM_OF_E_WEAPON) - 1;

  while (l <= r) {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - opcode;
    if (!cmp) {
      return dict[mid].entry.weapon;
    }
    if (cmp < 0)  l = mid + 1;
    else r = mid - 1;
  }

  fprintf(stderr, "Error: Player Weapon Dictionary Failed");
  exit(EXIT_FAILURE);
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
    gameSystemInstance->combatActions =
        (CombatAction *)malloc(sizeof(CombatAction) * DEFAULT_MAX_COMBAT_ACTIONS);
    gameSystemInstance->numOfCombatActions = 0;

    // Initialize Other General Variables
    gameSystemInstance->level = 1;
    gameSystemInstance->isGameOver = false;
    gameSystemInstance->isFinished = false;
    gameSystemInstance->atlasImages = NULL;
    initSettings(gameSystemInstance);
    initPlayerWeaponDictionary();
  }

  return gameSystemInstance;
}

void initSettings() {
  gameState->settings.screenWidth = SCREEN_WIDTH;
  gameState->settings.screenHeight = SCREEN_HEIGHT;
  gameState->settings.musicVolume = 50;
  gameState->settings.music_on = true;
  gameState->settings.sfx_on = true;
}

static void addPlayer(Player *player)
{
  Player *players = gameState->players;
  players[gameState->numOfPlayers++] = *player;
}

static void addEnemy(Enemy *enemy)
{
  Enemy *enemies = gameState->enemies;
  enemies[gameState->numOfEnemies++] = *enemy;
}

static void addBullet(Bullet *bullet, float angle)
{
  CombatAction *combatActions = gameState->combatActions;
  int idx = gameState->numOfCombatActions++;
  combatActions[idx].type = ACTION_BULLET;
  combatActions[idx].action.bullet = *bullet;
  combatActions[idx].angle = angle;
}

float GetAngleBetweenPoints(Vector2 point1, Vector2 point2)
{
  float deltaX = point2.x - point1.x;
  float deltaY = point2.y - point1.y;
  float angle =
      atan2f(deltaY, deltaX) * (180.0f / PI); // Convert radians to degrees
  return angle;
}

Bullet *initBullet(int ID, BulletInfo* bulletInfo, Vector2 src, Vector2 dest)
{

  Bullet *bullet = (Bullet *)malloc(sizeof(Bullet));
  bullet->playerID = ID;
  bullet->bulletInfo = *bulletInfo;
  bullet->startPosition = src;
  bullet->transform = (Transform){src, 0, (Vector2){1, 1}};
  addBullet(bullet, GetAngleBetweenPoints(src, dest));
  return bullet;
}

Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,Vector2 position, int ID)
{
  Settings settings = gameState->settings;
  Dictionary *dict = gameState->characterDictionary;
  Player *player = (Player *)malloc(sizeof(Player));
  int l = 0, r = NUM_OF_E_TYPE - 1;

  while (l <= r) {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - type;
    if (!cmp) {
      player->object = dict[mid].entry.character;
      break;
    }
    if (cmp < 0)  l = mid + 1;
    else r = mid - 1;
  }
  player->name = strdup(name);
  player->ID = ID;
  player->type = type;
  player->object.weapon = initWeapon(weapon, true);
  player->object.transform.position = position;
  player->score = 0;
  player->drawDirection = 1;
  player->direction = RIGHT;
  player->fire = 0;
  player->ID = ID;
  player->experience = (Experience){.xp = 0, .level = 0};
  // TODO: Make dictionary for infos related to each type of character.
  addPlayer(player);
  return player;
}

Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, Vector2 position)
{
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
    if (cmp < 0)  l = mid + 1;
    else r = mid - 1;
  }
  enemy->object.transform.position = position;
  enemy->object.weapon = initWeapon(weapon, false);
  addEnemy(enemy);
  return enemy;
}
