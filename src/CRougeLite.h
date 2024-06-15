/*********************************************************
 *
 *
*    ██╗  ██╗███████╗ █████╗ ██████╗ ███████╗██████╗ 
*    ██║  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██╔══██╗
*    ███████║█████╗  ███████║██║  ██║█████╗  ██████╔╝
*    ██╔══██║██╔══╝  ██╔══██║██║  ██║██╔══╝  ██╔══██╗
*    ██║  ██║███████╗██║  ██║██████╔╝███████╗██║  ██║
*    ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝
 *
 *    Main header file for includes and shared resources.
 *
 *********************************************************/

#ifndef CROUGELITE_H
#define CROUGELITE_H
#include <raylib.h>

//========================================================
// Defines
//========================================================
#define NULL ((void *)0)

//========================================================
// Structs & Enums
//========================================================
// TODO: Make enum for all stats related to the specified types
// instead of no encapsulation.

typedef enum {
  WEREWOLF,
  PYROMANIAC,
  NUM_OF_P_TYPE
} P_TYPE;

typedef enum {
  NUM_OF_P_WEAPON
} P_WEAPON;

typedef struct {
  // Player Info
  char *name;

  // Player Selection
  P_TYPE type;
  P_WEAPON weapon;

  // Player Stats
  Vector2 position;
  double health;
  double speed;
  double acceleration;
  double fire_rate;

  int score
} Player;

typedef enum {
  CIVILIAN,
  FARMER,
  KNIGHT,
  NUM_OF_E_TYPE
} E_TYPE;

typedef enum {
  NUM_OF_E_WEAPON
} E_WEAPON;

typedef struct {
  int health;
  E_TYPE type;
  E_WEAPON weapon;
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

  Settings settings;
} Game_System;

//========================================================
// Global Shared Variables
//========================================================
extern Music music;

//========================================================
// Global Functions
//========================================================
Game_System *getGameSystemInstance();

// Init Functions
void initGameSystem(Game_System* gameSystemInstance);
void initSettings(Game_System* gameSystemInstance);
void initPlayer(Player *player, const char *name, P_TYPE type, P_WEAPON weapon);
void initEnemy(Enemy *enemy, E_TYPE type, E_WEAPON weapon);

// Clear Resources Functions
void clearGameSystem();
void clearPlayer(Player **player);
void clearEnemy(Enemy **enemy);

#endif // CROUGELITE_H


/*********************************************************
 *
 *
 *      ███████╗ ██████╗ ███████╗
 *      ██╔════╝██╔═══██╗██╔════╝
 *      █████╗  ██║   ██║█████╗
 *      ██╔══╝  ██║   ██║██╔══╝
 *      ███████╗╚██████╔╝██║
 *      ╚══════╝ ╚═════╝ ╚═╝
 *
 *
 *********************************************************/
