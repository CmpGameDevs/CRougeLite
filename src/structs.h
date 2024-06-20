/****************************************************************
 *
 *
 *    ███████╗████████╗██████╗ ██╗   ██╗ ██████╗████████╗███████╗
 *    ██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔════╝╚══██╔══╝██╔════╝
 *    ███████╗   ██║   ██████╔╝██║   ██║██║        ██║   ███████╗
 *    ╚════██║   ██║   ██╔══██╗██║   ██║██║        ██║   ╚════██║
 *    ███████║   ██║   ██║  ██║╚██████╔╝╚██████╗   ██║   ███████║
 *    ╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ╚═════╝   ╚═╝   ╚══════╝
 *
 *
 *   Structs and Enums for the game.
 *
 *
 *****************************************************************/

#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <raylib.h>

// TODO: Make enum for all stats related to the specified types
// instead of no encapsulation.

typedef enum
{
  W = 1,
  A = 2,
  S = 5,
  D = 9,
} KEYS;

typedef enum
{
  WEREWOLF,
  PYROMANIAC,
  KNIGHT,
  NUM_OF_P_TYPE
} P_TYPE;

typedef enum
{
  LONG_SWORD,
  NUM_OF_P_WEAPON
} P_WEAPON;

typedef enum
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
} DIRECTIONS;
typedef struct
{
  double width;
  double height;
  //TODO: Add more properties
}RigidBody2d;

typedef struct
{
  int playerID;
  double bulletSpeed;
  double bulletDamage;
  double bulletRange;
  double bulletHealth;
  double angle;
  Vector2 position;
  Texture2D texture;
  RigidBody2d body;
} Bullet;

typedef struct
{
  // Player Info
  char *name;
  int ID;
  RigidBody2d body;
  // Player Selection
  P_TYPE type;
  P_WEAPON weapon;

  // Player Stats
  double reloadTime;
  Vector2 position;
  double health;
  double speed;
  double acceleration;
  double fireRate;
  int score;
  int fire;
  int drawDirection;    // 1 for right, -1 for left
  bool isMoving;
  DIRECTIONS direction; // to get info on the direction the player is facing.
} Player;


typedef enum
{
  E_CIVILIAN,
  E_FARMER,
  E_KNIGHT,
  NUM_OF_E_TYPE
} E_TYPE;

typedef enum
{
  E_SWORD,
  NUM_OF_E_WEAPON
} E_WEAPON;

typedef struct
{
  int health;
  E_TYPE type;
  E_WEAPON weapon;
  Texture2D texture;

  double speed;
  double acceleration;
  double fire_rate;
  Vector2 position;
  RigidBody2d body;
    int drawDirection;    // 1 for right, -1 for left
  bool isMoving;
} Enemy;

typedef struct
{
  int screen_width;
  int screen_height;
  int volume;
  bool music_on;
  bool sfx_on;
} Settings;


typedef struct AtlasImage
{
  char *filename;
  Rectangle source;
  Vector2 origin;
  struct AtlasImage *next;
} AtlasImage;

typedef struct SpriteAnimation {
  int numOfFrames;
  char **frameNames;
  int currentFrame;   // NOTE: still not used
  int framesPerSecond;
  bool loop;          // NOTE: still not used
  bool finished;      // NOTE: still not used
} SpriteAnimation;

typedef struct TilesMapper {
  unsigned int numOfTiles;
  char *mapper[MAX_TILES_NUM];
  char *path;         // Path of the mapper file
} TilesMapper;

typedef struct Map {
  unsigned int currentLevel;   // NOTE: maybe convert it to a Level struct
  char *currentLevelPath;
  TilesMapper tilesMapper;
  int mapIds[MAX_ROW_NUM][MAX_COL_NUM][MAX_CELL_ID];
  bool loaded;
  unsigned int numOfRows;
  unsigned int numOfCols;
  Texture2D *textures;
  bool *isTexturesLoaded;
} Map;

typedef struct
{
  int num_of_players;
  Player *players;
  int num_of_enemies;
  Enemy *enemies;
  int num_of_bullets;
  Bullet *bullets;
  int level;
  bool game_over;
  bool finished;
  Texture2D atlasTexture;
  AtlasImage *atlasImages;

  Settings settings;
  Map map;
} Game_System;
#endif // STRUCTS_H
