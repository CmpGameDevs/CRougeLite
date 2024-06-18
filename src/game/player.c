/***************************************************************
 *
 *
 *    ██████╗ ██╗      █████╗ ██╗   ██╗███████╗██████╗
 *    ██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝██╔════╝██╔══██╗
 *    ██████╔╝██║     ███████║ ╚████╔╝ █████╗  ██████╔╝
 *    ██╔═══╝ ██║     ██╔══██║  ╚██╔╝  ██╔══╝  ██╔══██╗
 *    ██║     ███████╗██║  ██║   ██║   ███████╗██║  ██║
 *    ╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝
 *
 *     Player Module Unit. (Game Object)
 *     Exposes the logic for the player object.
 *
 *     - Setup Player
 *     - Update Player
 *     - Draw Player
 *     - Clear Player
 *
 **************************************************************/

#include "player.h"

#include "../system/anime.h"
// FIXME: delete me later
#include "../system/init.h"
#include <raymath.h>

// ***************************
// Private Function Prototypes
// ***************************
static void initCharacterDictionary() {
  Dictionary *dict = malloc(sizeof(Dictionary) * (NUM_OF_P_TYPE));

  if (dict == NULL) {
    fprintf(stderr, "Error: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // Add in asc order
  // Because we fetch the info using binary search.
  dict[0].opcode = CAT;
  dict[0].entry.character = (GameObject){
      .rigidBody = {.velocity = (Vector2){5, 5},
                    .acceleration = (Vector2){0, 0},
                    1.0,
                    false},
      .collider = {.offset = (Vector2){0, 0}, 32, 32},
      .spriteRenderer = {.texture = LoadTexture("./src/"), 32, 32},
      .animator = {},
      .stats = {.health = {.maxHealth = 100, .currentHealth = 100},
                .attack = {.power = 1.0f, .cooldown = 5, .speed = 1.0f},
                .defense = {.value = 3, .nearHitValue = 6}}};

  dict[1].opcode = WEREWOLF;
  dict[2].opcode = PYROMANIAC;
  dict[3].opcode = KNIGHT;

  gameState->characterDictionary = dict;
}

static Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                          Vector2 position, int ID);
static void clearPlayer(Player **player);

/* setupPlayers
 *
 * Setup the players in the game. Create a new player and adds it to
 * the state of the game to draw and update.
 *
 */
void setupPlayers() {
  Player *player = initPlayer("Marcus", CAT, P_GUN,
                              (Vector2){gameState->settings.screenWidth / 2.0,
                                        gameState->settings.screenHeight / 2.0},
                              0);
}

/* drawPlayers
 *
 * Draw the players in the game.
 *
 */
void drawPlayers() {
  Player *players = gameState->players;
  int player_num = gameState->numOfPlayers;

  // FIXME: This is a temporary solution
  // Move this animation into the animator object
  // And setup them up at the setup of the player.
  SpriteAnimation idle = createSpriteAnimation(6,
                                               (char *[]){
                                                   "Meow-Knight_Idle_0_0",
                                                   "Meow-Knight_Idle_1_0",
                                                   "Meow-Knight_Idle_2_0",
                                                   "Meow-Knight_Idle_3_0",
                                                   "Meow-Knight_Idle_4_0",
                                                   "Meow-Knight_Idle_5_0",
                                               },
                                               6, true);

  SpriteAnimation walk = createSpriteAnimation(8,
                                               (char *[]){
                                                   "Meow-Knight_Run_0_0",
                                                   "Meow-Knight_Run_1_0",
                                                   "Meow-Knight_Run_2_0",
                                                   "Meow-Knight_Run_3_0",
                                                   "Meow-Knight_Run_4_0",
                                                   "Meow-Knight_Run_5_0",
                                                   "Meow-Knight_Run_6_0",
                                                   "Meow-Knight_Run_7_0",
                                               },
                                               12, true);

  while (player_num--) {
    Vector2 pos = players->object.rigidBody.position;
    bool flip = (players->drawDirection == -1) ? true : false;
    if (players->isMoving) {
      drawSpriteAnimationPro(&walk, (Rectangle){pos.x, pos.y, 64, 64},
                             (Vector2){0, 0}, 0, WHITE, flip);
    } else {
      drawSpriteAnimationPro(&idle, (Rectangle){pos.x, pos.y, 64, 64},
                             (Vector2){0, 0}, 0, WHITE, flip);
    }

    players++;
  }

  disposeSpriteAnimation(&idle);
  disposeSpriteAnimation(&walk);
}

/* updatePlayers
 *
 * Update the players in the game.
 * it handles it's input and state too.
 *
 */
void updatePlayers() {
  Player *player = gameState->players;
  double speed = player->stats.speed;

  Vector2 direction = {0, 0};
  if (IsKeyDown(KEY_W))
    direction.y -= 1;
  if (IsKeyDown(KEY_S))
    direction.y += 1;
  if (IsKeyDown(KEY_A))
    direction.x -= 1;
  if (IsKeyDown(KEY_D))
    direction.x += 1;

  player->object.rigidBody.velocity =
      Vector2Scale(Vector2Normalize(direction), speed);
  player->object.rigidBody.position = Vector2Add(
      player->object.rigidBody.position, player->object.rigidBody.velocity);

  if (Vector2Length(player->object.rigidBody.velocity) > 0) {
    player->isMoving = true;
  } else {
    player->isMoving = false;
  }

  if (player->object.rigidBody.velocity.x < 0) {
    player->drawDirection = -1;
  } else {
    player->drawDirection = 1;
  }

  // NOTE: this makes the player unable to go out of frame
  player->object.rigidBody.position =
      Vector2Clamp(player->object.rigidBody.position, (Vector2){0, 0},
                   (Vector2){gameState->settings.screenWidth - 64,
                             gameState->settings.screenHeight - 64});
  // FIXME: replace with sprite size
}

void clearPlayers() {
  int player_num = gameState->numOfPlayers;
  Player *players = gameState->players;

  while (player_num--) {
    printf("Deleting Player: %s\n", players->name);
    clearPlayer(&players);
    players++;
  }
}

// *****************
// PRIVATE FUNCTIONS
// *****************

static void addPlayer(Player *player) {
  Player *players = gameState->players;
  players[gameState->numOfPlayers++] = *player;
}

static PPlayer *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                   Vector2 position, int ID) {
  Settings settings = gameState->settings;
  Dictionary *dict = gameState->characterDictionary;
  Player *player = &(gameState->players[gameState->numOfPlayers++]);
  int l = 0, r = NUM_OF_E_TYPE - 1;

  while (l <= r) {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - type;
    if (!cmp) {
      player->object = dict[mid].entry.character;
      break;
    }
    if (cmp < 0)
      l = mid + 1;
    else
      r = mid - 1;
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
  return player;
                   }

static void clearPlayer(Player **player) {
  if (player == NULL || *player == NULL)
    return;

  free((*player)->name);
  free(*player);
  *player = NULL;
}
