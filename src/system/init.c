#include "init.h"

//========================================================
// LOCAL VARIABLE DEFINITIONS (local to this file)
//========================================================
static Dictionary* initWeaponDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary)*(NUM_OF_P_WEAPON + NUM_OF_E_WEAPON));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  dict[0].opcode = P_GUN;
  dict[0].entry.weapon = (Weapon){RANGED_WEAPON, .weapon.ranged = {
      .stats = {10, 0.5, 0, .weaponSprite = {LoadTexture("./src/"), 5, 5}},
      .bulletInfo = {3, 10, 100, 10, .bulletSprite = {LoadTexture("./src/"), 5, 5}},
      30, 30}};
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
    gameSystemInstance->bullets =
        (Bullet *)malloc(sizeof(Bullet) * DEFAULT_MAX_BULLETS);
    gameSystemInstance->num_of_bullets = 0;

    // Initialize Other General Variables
    gameSystemInstance->level = 1;
    gameSystemInstance->game_over = false;
    gameSystemInstance->finished = false;
    gameSystemInstance->atlasImages = NULL;
    initSettings(gameSystemInstance);
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
  Game_System *game = getGameSystemInstance();
  Player *players = game->players;
  players[game->numOfPlayers++] = *player;
}

static void addEnemy(Enemy *enemy)
{
  Game_System *game = getGameSystemInstance();
  Enemy *enemies = game->enemies;
  enemies[game->num_of_enemies++] = *enemy;
}

static void addBullet(Bullet *bullet)
{
  Game_System *game = getGameSystemInstance();
  Bullet *bullets = game->bullets;
  bullets[game->num_of_bullets++] = *bullet;
}

float GetAngleBetweenPoints(Vector2 point1, Vector2 point2)
{
  float deltaX = point2.x - point1.x;
  float deltaY = point2.y - point1.y;
  float angle =
      atan2f(deltaY, deltaX) * (180.0f / PI); // Convert radians to degrees
  return angle;
}

Bullet *initBullet(P_WEAPON weapon, int playerID, RigidBody2d body, Vector2 src, Vector2 dest)
{

  Bullet *bullet = (Bullet *)malloc(sizeof(Bullet));
  bullet->bulletDamage = 10.0;
  bullet->bulletHealth = 10.0;
  bullet->bulletRange = 10.0;
  bullet->bulletSpeed = 8.0;
  bullet->playerID = playerID;
  bullet->position = src;
  bullet->body = body;
  bullet->angle = GetAngleBetweenPoints(src, dest);
  addBullet(bullet);
  return bullet;
}

Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon, int ID) {
  Settings settings = gameState->settings;
Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon, RigidBody2d body,Vector2 position, int ID)
{
  Player *player = (Player *)malloc(sizeof(Player));
  player->name = strdup(name);
  player->ID = ID;
  player->type = type;
  player->weapon = weapon;
  player->health = 100.0;
  player->speed = 5.0;
  player->acceleration = 0.1;
  player->score = 0;
  player->fireRate = 0.5;
  player->drawDirection = 1;
  player->direction = RIGHT;
  player->fire = 0;
  player->ID = ID;
  player->body = body;
  player->position = position;
  player->reloadTime = 0.0;
  // TODO: Make dictionary for infos related to each type of character.
  addPlayer(player);
  return player;
}

Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, RigidBody2d body,Vector2 position)
{
  Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
  enemy->type = type;
  enemy->weapon = weapon;
  enemy->health = 50;
  enemy->speed = 0.8;
  enemy->acceleration = 0.05;
  enemy->fire_rate = 1.0;
  enemy->body = body;
  enemy->drawDirection = 1;
  enemy->isMoving = false;
  enemy->position = position;
  addEnemy(enemy);
  // TODO: Make dictionary for infos related to each type of enemy.
  return enemy;
}
