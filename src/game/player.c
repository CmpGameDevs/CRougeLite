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
#include "../system/collision.h"
#include "../system/draw.h"

#include "../system/anime.h"
#include <raylib.h>
#include <raymath.h>

// ***************************
// Private Function Prototypes
// ***************************
static Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                          Vector2 position);
static void AddPlayerWeapon(Player *player, P_WEAPON weapon);
static void animationController(Player *player);
static int handlePlayerInput(Player *player);
static void handlePlayerMovement(Player *player, int bitmask);
static void handlePlayerInventory(Player *player, int bitmask);
static void handlePlayerInteraction(Player *player, int bitmask);
static bool canMovePlayer(float posX, float posY, Vector2 size);
static void clearPlayer(Player **player);

/**
 * setupPlayers - initialize all enemies manually to the game state
 */
void setupPlayers()
{
  Player *player =
      initPlayer("Marcus", CAT, P_GUN,
                 (Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0});

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

  for (int i = 0; i < player_num; i++)
  {
    bool flip = (players[i].drawDirection == -1) ? true : false;
    drawAnimator(&(players[i].object.animator), &(players[i].object.transform),
                 WHITE, flip);
    
    // Draw health bar if the player was damaged recently
    Vector2 playerCenter = {
      players[i].object.transform.position.x + players[i].object.collider.bounds.width / 2,
      players[i].object.transform.position.y
    };
    drawHealthBar(playerCenter, 
                  players[i].stats.health.currentHealth, 
                  players[i].stats.health.maxHealth, 
                  players[i].stats.health.lastUpdateTime);
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
  handlePlayerMovement(player, playerInput);

  if (playerInput)
  {
    // Swapping between weapons
    handlePlayerInventory(player, playerInput);

    // Interact with the map
    handlePlayerInteraction(player, playerInput);
  }

  // NOTE: animation controller is the state machine
  animationController(player);
  // NOTE:: this is the call that forwards the animation
  updateAnimator(&(player->object.animator));
}

/**
 * addToInventory - add an item to the player inventory
 * 
 * @param player The player to add the item to
 * @param itemId The ID of the item to add
 * 
 * @return true if the item was added successfully, false otherwise
 */
bool addToInventory(Player *player, int itemId) {
  CollectibleItem item = {
      .tileId = itemId,
      .count = 1
  };
  
  CollectibleItem *collectedItems = player->collectedItems;
  for (int i = 0; i < MAX_COLLECTED_ITEMS; i++)
  {
    if (collectedItems[i].tileId == itemId)
    {
      collectedItems[i].count++;
      return true;
    }
    if (collectedItems[i].tileId == -1)
    {
      collectedItems[i] = item;
      return true;
    }
  }
  
  return false;
}

/**
 * useFromInventory - remove an item from the player inventory
 * 
 * @param player The player to remove the item from
 * @param itemId The ID of the item to remove
 * 
 * @return true if the item was removed successfully, false otherwise
 */
bool useFromInventory(Player *player, int itemId) {
  CollectibleItem *collectedItems = player->collectedItems;
  int indexToRemove = -1;
  for (int i = 0; i < MAX_COLLECTED_ITEMS; i++)
  {
    if (collectedItems[i].tileId == itemId)
    {
      if (collectedItems[i].count > 1)
      {
        collectedItems[i].count--;
        return true;
      }
      indexToRemove = i;
      break;
    }
  }

  if (indexToRemove == -1)  return false;

  // Compact the inventory
  for (int i = indexToRemove; i < MAX_COLLECTED_ITEMS - 1; i++)
  {
    collectedItems[i] = collectedItems[i + 1];
  }
  collectedItems[MAX_COLLECTED_ITEMS - 1] = (CollectibleItem){-1, 0}; // Reset last item
  return true;
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
 *
 * @return Pointer to the new player object
 */
static Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                          Vector2 position)
{
  static unsigned int playerID = 0;
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
  printf("Adding Player #%d\n", playerID);
  player->name = strdup(name);
  player->ID = playerID++;
  player->type = type;
  player->inventory = initInventory();
  for (int i = 0; i < MAX_COLLECTED_ITEMS; i++)
  {
    player->collectedItems[i] = (CollectibleItem){-1, 0};
  }
  AddPlayerWeapon(player, weapon);
  player->object.transform.position = position;
  player->object.collider.bounds.x = position.x;
  player->object.collider.bounds.y = position.y;
  player->score = 0;
  player->drawDirection = 1;
  player->direction = RIGHT;
  player->fire = 0;
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
                          .interact = KEY_F,
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
  if (player->interactableTileIndex != -1 &&
      IsKeyDown(input.interact))
    bitmask |= INPUT_INTERACT;

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
  Vector2 position = player->object.transform.position;
  Vector2 newPosition = Vector2Add(player->object.transform.position, velocity);

  player->isMoving = (Vector2Length(velocity) > 0);

  if (velocity.x < 0)
  {
    player->drawDirection = -1;
  }
  else
  {
    player->drawDirection = 1;
  }

  // Get edges points
  // Subtract an offset from the top row and left column to account for "depth"
  Vector2 playerColliderSize = {
      .x = player->object.collider.bounds.width,
      .y = player->object.collider.bounds.height,
  };

  bool canMove = canMovePlayer(newPosition.x, newPosition.y, playerColliderSize);

  if (!canMove) {
    if (velocity.y != 0 &&
        canMovePlayer(position.x, newPosition.y, playerColliderSize)) {
      newPosition.x = position.x;
      velocity.x = 0;
    } else if (velocity.x != 0 &&
               canMovePlayer(newPosition.x, position.y, playerColliderSize)) {
      newPosition.y = position.y;
      velocity.y = 0;
    } else {
      player->isMoving = false;
      return;
    }
  }
  
  player->object.rigidBody.velocity = velocity;
  player->object.transform.position = newPosition;
  player->object.collider.bounds.x = newPosition.x;
  player->object.collider.bounds.y = newPosition.y;  
}

/**
 * canMovePlayer - check if the player can move to the new position
 * 
 * @param posX X position of the player
 * @param posY Y position of the player
 * @param size Size of the player collider
 * 
 * @return true if the player can move, false otherwise
 */
static bool canMovePlayer(float posX, float posY, Vector2 size)
{
  int offsetX = 30, offsetY = 40;
  Collider2D collider = {
      .bounds = (Rectangle){posX + offsetX / 2, posY + offsetY, size.x - offsetX, size.y - offsetY},
      .isColliding = false,
  };
  int startX, startY, endX, endY;

  getGameObjectIndices(&startX, &startY, &endX, &endY, &collider);
  return isWalkable(startY, startX) &&
         isWalkable(startY, endX) &&
         isWalkable(endY, startX) &&
         isWalkable(endY, endX);
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
  // Handle weapon selection
  WeaponsInventory *inventory = &(player->inventory);
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

/**
 * handlePlayerInteraction - handle player interaction with the map
 * 
 * @param player Pointer to the player object
 * @param bitmask Bitmask for the player's input
 */
static void handlePlayerInteraction(Player *player, int playerInput)
{
  int interactableTileIndex = player->interactableTileIndex;
  player->interactableTileIndex = -1; // Reset interaction

  if (!(playerInput & INPUT_INTERACT) ||
    interactableTileIndex == -1) return;

  int row = interactableTileIndex / gameState->map.numOfCols;
  int col = interactableTileIndex % gameState->map.numOfCols;
  int tileId = gameState->map.mapIds[row][col][LAYER_INTERACTABLE];
  if (tileId == -1) return;

  InteractableMapping *mapping = getInteractableMapping(tileId);
  if (mapping == NULL) return;

  int requiredItem = mapping->requiredItem;
  if (canInteractWith(tileId, player->collectedItems)) {
    toggleInteractable(row, col, player->collectedItems);
    useFromInventory(player, requiredItem);
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
