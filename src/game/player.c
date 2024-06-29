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
static void AddPlayerWeapon(Player *player, P_WEAPON weapon);
static void animationController(Player *player);
static int handlePlayerInput(Player *player);
static void handlePlayerMovement(Player *player, int bitmask);
static void handlePlayerInventory(Player *player, int bitmask);
static void clearPlayer(Player **player);

/**
 * setupPlayers - initialize all enemies manually to the game state
 */
void setupPlayers()
{
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

/**
 * drawPlayers - draw players on the scene
 */
void drawPlayers()
{
  Player *players = gameState->players;
  int player_num = gameState->numOfPlayers;

  while (player_num--)
  {
    bool flip = (players->drawDirection == -1) ? true : false;
    drawAnimator(&(players->object.animator), &(players->object.transform),
                 WHITE, flip);
    players++;
  }
}

/**
 * updatePlayers - update the players objects every game tick
 */
void updatePlayers()
{
  Player *player = gameState->players;
  Input input = player->input;
  double speed = player->stats.speed;

  int playerInput = handlePlayerInput(player);

  if (playerInput)
  {
    handlePlayerMovement(player, playerInput);
    // Swapping between weapons
    handlePlayerInventory(player, playerInput);
  }

  // NOTE: animation controller is the state machine
  animationController(player);
  // NOTE:: this is the call that forwards the animation
  updateAnimator(&(player->object.animator));
}

/**
 * clearPlayers - free players array from heap
 */
void clearPlayers()
{
  int player_num = gameState->numOfPlayers;
  Player *players = gameState->players;

  while (player_num--)
  {
    printf("Deleting Player: %s\n", players->name);
    clearPlayer(&players);
    players++;
  }
  printf("Deleted all Players\n");
}

// *****************
// PRIVATE FUNCTIONS
// *****************

/**
 * initPlayer - initialize a new player object and add it to the game sate
 *
 * @param name Player name
 * @param type Player type
 * @param weapon Player weapon
 * @param position Player spawn position
 * @param ID Player ID
 *
 * @return Pointer to the new player object
 */
static Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                          Vector2 position, int ID)
{
  Settings settings = gameState->settings;
  Dictionary *dict = gameState->characterDictionary;
  Player *player = &(gameState->players[gameState->numOfPlayers++]);
  int l = 0, r = NUM_OF_E_TYPE - 1;

  while (l <= r)
  {
    int mid = l + (r - l) / 2;
    int cmp = dict[mid].opcode - type;
    if (!cmp)
    {
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
  player->object.collider.bounds.x = position.x;
  player->object.collider.bounds.y = position.y;
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
  for (int i = 0; i < player->inventory.MAX_NUM_OF_WEAPONS; i++)
  {
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

/**
 * addPlayer - add player to the game state array
 */
static void addPlayer(Player *player)
{
  Player *players = gameState->players;
  players[gameState->numOfPlayers++] = *player;
}

static void AddPlayerWeapon(Player *player, P_WEAPON weapon)
{
  Weapon newWeapon = initWeapon(weapon, true);
  player->inventory.weapons[player->inventory.currentNumOfWeapons++] =
      newWeapon;
}

/**
 * animationController
 */
static void animationController(Player *player)
{
  if (IsKeyPressed(KEY_SPACE))
    setState(&(player->object.animator), DODGE);

  // NOTE: this meaning that looping animation have less priority than
  // non-looping animations like dodge and attack
  if (player->object.animator.isFinished == true)
  {
    if (player->isMoving)
    {
      setState(&(player->object.animator), RUN);
    }
    else
    {
      setState(&(player->object.animator), IDLE);
    }
  }
}

/**
 * handlePlayerInput - handle the player input every game tick
 *
 * @param player Pointer to the player object
 *
 * @return Bitmask for the player's input
 */
static int handlePlayerInput(Player *player)
{
  Input input = player->input;
  int bitmask = 0;
  if (IsKeyDown(input.up))
    bitmask |= INPUT_UP;
  if (IsKeyDown(input.down))
    bitmask |= INPUT_DOWN;
  if (IsKeyDown(input.left))
    bitmask |= INPUT_LEFT;
  if (IsKeyDown(input.right))
    bitmask |= INPUT_RIGHT;

  for (int i = 0; i < player->inventory.currentNumOfWeapons; i++)
  {
    if (IsKeyPressed(player->input.weapons[i]))
    {
      bitmask |= (INPUT_INVENTORY_1 << i);
      break;
    }
  }

  player->input.mouseWheelMove = GetMouseWheelMove();

  return bitmask;
}

/**
 * handlePlayerMovement - update player movement according to
 * the player input
 *
 * @param player Pointer to the player object
 * @param playerInput Bitmask for the player's input
 */
static void handlePlayerMovement(Player *player, int playerInput)
{
  Vector2 direction = {0, 0};
  if (playerInput & INPUT_UP)
    direction.y -= 1;
  if (playerInput & INPUT_DOWN)
    direction.y += 1;
  if (playerInput & INPUT_LEFT)
    direction.x -= 1;
  if (playerInput & INPUT_RIGHT)
    direction.x += 1;

  double speed = player->stats.speed;
  Vector2 velocity = Vector2Scale(Vector2Normalize(direction), speed);
  Vector2 position = Vector2Add(player->object.transform.position, velocity);

  if (Vector2Length(velocity) > 0)
  {
    player->isMoving = true;
  }
  else
  {
    player->isMoving = false;
  }

  if (velocity.x < 0)
  {
    player->drawDirection = -1;
  }
  else
  {
    player->drawDirection = 1;
  }

  // NOTE: this makes the player unable to go out of frame
  player->object.rigidBody.velocity = velocity;
  // TODO: position clamping is removed for now
  // player->object.transform.position =
  // Vector2Clamp(position, (Vector2){0, 0},
  // (Vector2){GetScreenWidth() - 64, GetScreenHeight() - 64});
  player->object.transform.position = position;
  player->object.collider.bounds.x = position.x;
  player->object.collider.bounds.y = position.y;
}

/**
 * handlePlayerInventory - update player selected weapon according to
 * the player input
 *
 * @param player Pointer to the player object
 * @param bitmask Bitmask for the player's input
 */
static void handlePlayerInventory(Player *player, int playerInput)
{
  Inventory *inventory = &(player->inventory);
  for (int i = 0; i < inventory->currentNumOfWeapons; i++)
  {
    if (playerInput & (INPUT_INVENTORY_1 << i))
    {
      inventory->currentWeapon = i;
      break;
    }
  }

  float mouseWheelMove = player->input.mouseWheelMove;

  if (mouseWheelMove != 0)
  {
    inventory->currentWeapon += (int)mouseWheelMove;
    if (inventory->currentWeapon < 0)
    {
      inventory->currentWeapon =
          inventory->currentNumOfWeapons - 1;
    }
    else if (inventory->currentWeapon >=
             inventory->currentNumOfWeapons)
    {
      inventory->currentWeapon = 0;
    }
  }
}


static void clearPlayer(Player **player)
{
  if (player == NULL || *player == NULL)
    return;

  clearInventory(&((*player)->inventory));
  free((*player)->name);
  free((*player)->input.weapons);
  free(*player);
  *player = NULL;
}
