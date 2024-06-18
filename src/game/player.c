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
                                               8, true);

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

// FIXME: This function needs major refactor and optmization
/* updatePlayers
 *
 * Update the players in the game.
 * it handles it's input and state too.
 *
 */
void updatePlayers() {
  // NOTE:
  // 1,2,5,9 theses number if you sum any two numbers you will get a unique
  /// number
  // not in the list and not equal the sum of any other two numbers same for 3
  // numbers. I choose these numbers to make it easy to know the angle of
  // movement without making alot of conditions on key press. 1,2,5,9 w,a,s,d

  GameState *game = gameState;
  int selected_player = 0;
  Player *player = ((game->players) + selected_player);
  Vector2 *pos = &(player->object.rigidBody.position);
  DIRECTIONS *direction = &(player->direction);
  int speed = Vector2Length(player->object.rigidBody.velocity);

  static int angles[18]; // the size is the sum of the 4 numbers for fast
                         // retrieval of the angle.
  memset(angles, -1, sizeof(angles));
  angles[W + D + A] = angles[W] = -90;
  angles[W + S + A] = angles[A] = 180;
  angles[A + S + D] = angles[S] = 90;
  angles[W + S + D] = angles[D] = 0;
  angles[W + A] = -135;
  angles[W + D] = -45;
  angles[S + D] = 45;
  angles[S + A] = 135;

  player->isMoving = false;

  int sum = 0;

  if (IsKeyDown(KEY_ESCAPE)) {
    game->isFinished = true;
  }
  if (IsKeyDown(KEY_W)) {
    sum += 1;
    *direction = UP;
  }
  if (IsKeyDown(KEY_S)) {
    sum += 5;
    *direction = DOWN;
  }
  if (IsKeyDown(KEY_A)) {
    sum += 2;
    *direction = LEFT;
    player->drawDirection = -1;
  }
  if (IsKeyDown(KEY_D)) {
    sum += 9;
    *direction = RIGHT;
    player->drawDirection = 1;
  }
  if (angles[sum] == -1)
    return;
  player->isMoving = true;
  pos->x += speed * cos(angles[sum] * DEG2RAD);
  pos->y += speed * sin(angles[sum] * DEG2RAD);
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

static Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
                          Vector2 position, int ID) {
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
    if (cmp < 0)
      l = mid + 1;
    else
      r = mid - 1;
  }

  player->name = strdup(name);
  player->ID = ID;
  player->type = type;
  player->object.weapon = initWeapon(weapon, true);
  player->object.rigidBody.position = position;
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

static void clearPlayer(Player **player) {
  if (player == NULL || *player == NULL)
    return;

  free((*player)->name);
  free(*player);
  *player = NULL;
}
