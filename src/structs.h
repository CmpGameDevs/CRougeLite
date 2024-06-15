#ifndef STRUCTS_H
#define STRUCTS_H

#include <raylib.h>

// TODO: Make enum for all stats related to the specified types
// instead of no encapsulation.

typedef enum {
  WEREWOLF,
  PYROMANIAC,
  KNIGHT,
  NUM_OF_P_TYPE
} P_TYPE;

typedef enum {
  LONG_SWORD,
  NUM_OF_P_WEAPON
} P_WEAPON;

typedef struct {
  // Player Info
  char *name;

  // Player Selection
  P_TYPE type;
  P_WEAPON weapon;
  Texture2D texture;

  // Player Stats
  Vector2 position;
  double health;
  double speed;
  double acceleration;
  double fire_rate;
  int score;
} Player;

typedef enum {
  E_CIVILIAN,
  E_FARMER,
  E_KNIGHT,
  NUM_OF_E_TYPE
} E_TYPE;

typedef enum {
  NUM_OF_E_WEAPON
} E_WEAPON;

typedef struct {
  int health;
  E_TYPE type;
  E_WEAPON weapon;
  Texture2D texture;

  double speed;
  double acceleration;
  double fire_rate;
} Enemy;

typedef struct {
  int screen_width;
  int screen_height;
  int volume;
  bool music_on;
  bool sfx_on;
} Settings;

typedef struct {
  int MAX_NUM_OF_PLAYERS;
  int num_of_players;
  Player* players;
  int MAX_NUM_OF_ENEMIES;
  int num_of_enemies;
  Enemy* enemies;
  int level;
  bool game_over;
  bool finished;

  Settings settings;
} Game_System;

#endif // STRUCTS_H
