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
#include <raylib.h>
#include <raymath.h>

// ***************************
// Private Function Prototypes
// ***************************
static Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                          Vector2 position, int ID);
static void clearPlayer(Player **player);
static void AddPlayerWeapon(Player *player, P_WEAPON weapon);
static void animationController(Player *player);

/* setupPlayers
 *
 * Setup the players in the game. Create a new player and adds it to
 * the state of the game to draw and update.
 *
 */
void setupPlayers() {
  Player *player =
      initPlayer("Marcus", CAT, P_GUN,
                 (Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0}, 0);

  player->object.animator = (Animator){
      .isFinished = false,
      .currentState = IDLE,
  };

  player->object.animator.animations[IDLE] = (SpriteAnimation){
      .frameNames = {"Meow-Knight_Idle_0_0", "Meow-Knight_Idle_1_0",
                     "Meow-Knight_Idle_2_0", "Meow-Knight_Idle_3_0",
                     "Meow-Knight_Idle_4_0", "Meow-Knight_Idle_5_0"},
      .numOfFrames = 6,
      .fps = 8,
      .currentFrame = 0,
      .frameCount = 0,
      .isLooping = true,
      .isFinished = false,
  };
  player->object.animator.animations[RUN] = (SpriteAnimation){
      .frameNames = {"Meow-Knight_Run_0_0", "Meow-Knight_Run_1_0",
                     "Meow-Knight_Run_2_0", "Meow-Knight_Run_3_0",
                     "Meow-Knight_Run_4_0", "Meow-Knight_Run_5_0",
                     "Meow-Knight_Run_6_0", "Meow-Knight_Run_7_0"},
      .numOfFrames = 8,
      .fps = 12,
      .currentFrame = 0,
      .frameCount = 0,
      .isLooping = true,
      .isFinished = false,
  };
  player->object.animator.animations[DODGE] = (SpriteAnimation){
      .frameNames =
          {
              "Meow-Knight_Dodge_0_0",
              "Meow-Knight_Dodge_1_0",
              "Meow-Knight_Dodge_2_0",
              "Meow-Knight_Dodge_3_0",
              "Meow-Knight_Dodge_4_0",
              "Meow-Knight_Dodge_5_0",
              "Meow-Knight_Dodge_6_0",
              "Meow-Knight_Dodge_7_0",
          },
      .numOfFrames = 8,
      .fps = 8,
      .currentFrame = 0,
      .frameCount = 0,
      .isLooping = false,
      .isFinished = false,
  };

  player->object.transform.scale = (Vector2){4, 4};

  AddPlayerWeapon(player, P_MISSILE_LAUNCHER);
}

/* drawPlayers
 *
 * Draw the players in the game.
 *
 */
void drawPlayers() {
  Player *players = gameState->players;
  int player_num = gameState->numOfPlayers;

  while (player_num--) {
    bool flip = (players->drawDirection == -1) ? true : false;
    drawAnimator(&(players->object.animator), &(players->object.transform),
                 WHITE, flip);
    players++;
  }
}

/* updatePlayers
 *
 * Update the players in the game.
 * it handles it's input and state too.
 *
 */
void updatePlayers() {
  Player *player = gameState->players;
  Input input = player->input;
  double speed = player->stats.speed;

  Vector2 direction = {0, 0};
  if (IsKeyDown(input.up))
    direction.y -= 1;
  if (IsKeyDown(input.down))
    direction.y += 1;
  if (IsKeyDown(input.left))
    direction.x -= 1;
  if (IsKeyDown(input.right))
    direction.x += 1;

  // NOTE: animation controller is the state machine
  animationController(player);
  // NOTE:: this is the call that forwards the animation
  updateAnimator(&(player->object.animator));

  Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed);
  Vector2 position = Vector2Add(player->object.transform.position, velocity);

  if (Vector2Length(velocity) > 0) {
    player->isMoving = true;
  } else {
    player->isMoving = false;
  }

  if (velocity.x < 0) {
    player->drawDirection = -1;
  } else {
    player->drawDirection = 1;
  }

  // NOTE: this makes the player unable to go out of frame
  player->object.rigidBody.velocity = velocity;
  player->object.transform.position =
      Vector2Clamp(position, (Vector2){0, 0},
                   (Vector2){GetScreenWidth() - 64, GetScreenHeight() - 64});
  // FIXME: replace with sprite size

  // Swapping between weapons
  for (int i = 0; i < player->inventory.currentNumOfWeapons; i++) {
    if (IsKeyPressed(player->input.weapons[i])) {
      player->inventory.currentWeapon = i;
      break;
    }
  }

  float mouseWheelMove = GetMouseWheelMove();

  if (mouseWheelMove != 0) {
    player->inventory.currentWeapon += (int)mouseWheelMove;
    if (player->inventory.currentWeapon < 0) {
      player->inventory.currentWeapon =
          player->inventory.currentNumOfWeapons - 1;
    } else if (player->inventory.currentWeapon >=
               player->inventory.currentNumOfWeapons) {
      player->inventory.currentWeapon = 0;
    }
  }
}

void clearPlayers() {
  int player_num = gameState->numOfPlayers;
  Player *players = gameState->players;

  while (player_num--) {
    printf("Deleting Player: %s\n", players->name);
    clearPlayer(&players);
    players++;
  }
  printf("Deleted all Players\n");
}

// *****************
// PRIVATE FUNCTIONS
// *****************

static void animationController(Player *player) {
  if (IsKeyPressed(KEY_SPACE))
    setState(&(player->object.animator), DODGE);

  // NOTE: this meaning that looping animation have less priority than
  // non-looping animations like dodge and attack
  if (player->object.animator.isFinished == true) {
    if (player->isMoving) {
      setState(&(player->object.animator), RUN);
    } else {
      setState(&(player->object.animator), IDLE);
    }
  }
}

static void addPlayer(Player *player) {
  Player *players = gameState->players;
  players[gameState->numOfPlayers++] = *player;
}

static Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                          Vector2 position, int ID) {
  Settings settings = gameState->settings;
  Dictionary *dict = gameState->characterDictionary;
  Player *player = &(gameState->players[gameState->numOfPlayers++]);
  int l = 0, r = NUM_OF_E_TYPE - 1;

  while (l <= r) {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - type;
    if (!cmp) {
      *player = dict[mid].entry.player;
      break;
    }
    if (cmp < 0)
      l = mid + 1;
    else
      r = mid - 1;
  }
  printf("Adding Player #%d\n", ID);
  player->name = strdup(name);
  player->ID = ID;
  player->type = type;
  player->inventory = initInventory();
  AddPlayerWeapon(player, weapon);
  player->object.transform.position = position;
  player->score = 0;
  player->drawDirection = 1;
  player->direction = RIGHT;
  player->fire = 0;
  player->ID = ID;
  player->experience = (Experience){.xp = 0, .level = 0};
  // TODO: Make dictionary for infos related to each type of character.
  // Input
  int *weapons =
      (int *)malloc(sizeof(int) * player->inventory.MAX_NUM_OF_WEAPONS);
  for (int i = 0; i < player->inventory.MAX_NUM_OF_WEAPONS; i++) {
    weapons[i] = KEY_ONE + i;
  }

  player->input = (Input){.up = KEY_W,
                          .down = KEY_S,
                          .left = KEY_A,
                          .right = KEY_D,
                          .action = KEY_E,
                          .weapons = weapons};

  return player;
}

static void AddPlayerWeapon(Player *player, P_WEAPON weapon) {
  Weapon newWeapon = initWeapon(weapon, true);
  player->inventory.weapons[player->inventory.currentNumOfWeapons++] =
      newWeapon;
}

static void clearPlayer(Player **player) {
  if (player == NULL || *player == NULL)
    return;

  clearInventory(&((*player)->inventory));
  free((*player)->name);
  free((*player)->input.weapons);
  free(*player);
  *player = NULL;
}
