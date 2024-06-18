#include "player.h"

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

static void addPlayer(Player *player) {
  Player *players = gameState->players;
  players[gameState->numOfPlayers++] = *player;
}

Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon,
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