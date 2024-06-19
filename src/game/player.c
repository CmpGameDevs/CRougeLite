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
    Vector2 pos = players->object.transform.position;
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

  Vector2 velocity =
      Vector2Scale(Vector2Normalize(direction), speed);
  
  Vector2 position = Vector2Add(
      player->object.transform.position, velocity);

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
  printf("Deleted all Players\n");
}

// *****************
// PRIVATE FUNCTIONS
// *****************

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
  Weapon selectedWeapon = initWeapon(weapon, true);
  player->inventory = initInventory();
  player->inventory.weapons[player->inventory.currentNumOfWeapons++] = selectedWeapon;
  player->object.transform.position = position;
  player->score = 0;
  player->drawDirection = 1;
  player->direction = RIGHT;
  player->fire = 0;
  player->ID = ID;
  player->experience = (Experience){.xp = 0, .level = 0};
  // TODO: Make dictionary for infos related to each type of character.
  // Input
  player->input = (Input){.up = KEY_W, .down = KEY_S, .left = KEY_A, .right = KEY_D, .action = KEY_E };

  return player;
}

static void clearPlayer(Player **player) {
  if (player == NULL || *player == NULL)
    return;

  free((*player)->name);
  free(*player);
  *player = NULL;
}
