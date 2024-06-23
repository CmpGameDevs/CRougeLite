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

/*============================================================================
 *                                  ENUMS
 *
 *    Put here all sort of types you want make sure that anychange here
 *    will be reflected in the whole code. (e.g. Switch cases, etc.)
 *===========================================================================*/

// TODO: Make enum for all stats related to the specified types
// instead of no encapsulation.

// FIXME: this probably should be refactored and removed.

typedef enum
{
  ACTION_NONE,
  ACTION_BULLET,
  ACTION_SLASH
} CombatActionType;

typedef enum
{
  RANGED_WEAPON,
  MELEE_WEAPON,
  NUM_OF_WEAPON_TYPES
} WeaponType;

typedef enum
{
  E_SWORD,
  NUM_OF_E_WEAPON
} E_WEAPON;

typedef enum
{
  CAT,
  WEREWOLF,
  PYROMANIAC,
  KNIGHT,
  NUM_OF_P_TYPE
} P_TYPE;

typedef enum
{
  P_GUN,
  P_LONG_SWORD,
  P_MISSILE_LAUNCHER,
  NUM_OF_P_WEAPON
} P_WEAPON;

typedef enum
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
} DIRECTIONS;

typedef enum
{
  E_CIVILIAN,
  E_FARMER,
  E_KNIGHT,
  NUM_OF_E_TYPE
} E_TYPE;

typedef enum
{
  // NOTE: those are used for animation as more as you need
  // NOTE: if the number exceeds the max change the def in defs
  // cause are stack allocated
  IDLE,
  WALK,
  RUN,
  DODGE,
  ATTACK1,
  ATTACK2,
  TAKE_DAMAGE,
  DIE,
} STATE;

/*============================================================================
 *                                  STRUCTS
 *
 *    Here are all the structs (Think of them as object data that you use)
 *    Change here are expensive as these Almost types and used or all over
 *    the place so make sure every thing is OK after changes.
 *
 *===========================================================================*/

// **********************
// GENERAL ENGINE STRUCTS
// **********************

typedef struct CTransform
{
  Vector2 position;
  float rotation;
  float frequency;
  float amplitude;
  Vector2 scale;
} CTransform;

typedef struct
{
  Vector2 velocity;
  Vector2 acceleration;
  float drag;
  bool isKinematic; // Kinematic object is typically not affected by physics
                    // forces but can still interact with other objects in
                    // certain ways.
} RigidBody2D;

typedef struct
{
  Vector2 offset;
  float width;
  float height;
} Collider2D;

typedef struct AtlasImage
{
  char *filename;
  Rectangle source;
  Vector2 origin;
  struct AtlasImage *next;
} AtlasImage;

// FIXME: Rework SpriteRenderer or remove it if not used.
// Atlas Image could do the same work if it has the dest rect.
typedef struct
{
  Texture2D texture;
  int width;
  int height;
} SpriteRenderer;

typedef struct SpriteAnimation
{
  char *frameNames[MAX_FRAMES_PER_ANIMATION];
  int currentFrame;
  int frameCount;
  int numOfFrames;
  int fps;
  bool isLooping;
  bool isFinished;
} SpriteAnimation;

typedef struct
{
  SpriteAnimation animations[MAX_ANIMATION_STATES];
  int currentState;
  bool isFinished;
} Animator;

typedef struct
{
  int up;
  int down;
  int left;
  int right;
  int shoot;
  int action;
  int *weapons;
} Input;

typedef struct
{
  CTransform transform;
  RigidBody2D rigidBody;
  Collider2D collider;
  SpriteRenderer spriteRenderer;
  Animator animator;
} GameObject;

// *********************
// GAME SPECIFIC STRUCTS
// *********************

typedef struct
{
  int currentHealth;
  int maxHealth;
} Health;

// IDK if those affect the other structs or not (like leveling up)
typedef struct
{
  float power;
  float speed;
  float cooldown;
} Attack;

typedef struct
{
  int value;
  int nearHitValue; // Blocked on the last second.
  // TODO: Add defense for different type of attacks?
} Defense;

typedef struct
{
  int xp;
  int level;
} Experience;

typedef struct
{
  Health health;
  Attack attack;
  Defense defense;
  double speed;
} Stats;

typedef struct
{
  float bulletSpeed;
  float bulletDamage;
  float bulletRange;
  float bulletHealth;
  bool isTracking;
  int pathCode;
  int enemyID;
  RigidBody2D rigidBody;
  Collider2D collider;
} BulletInfo;

typedef struct
{
  int playerID;
  BulletInfo bulletInfo;
  Vector2 startPosition; // To know if the bullet exceeded the range.
  Vector2 dest;
  Animator animator;
  CTransform transform;
} Bullet;

typedef struct
{
  float slashRange;
  float slashDamage;
  bool isActive;
  SpriteRenderer slashSprite;
  Collider2D collider;
} SlashInfo;

typedef struct
{
  int playerID;
  SlashInfo slashInfo;
  CTransform transform;
} Slash;

typedef union
{
  Bullet bullet;
  Slash slash;
} CombatActionUnion;

typedef struct
{
  float angle;
  CombatActionUnion action;
  CombatActionType type;
} CombatAction;

typedef struct
{
  int damage;
  float cooldown;
  float lastUseTime;
  SpriteRenderer weaponSprite;
} WeaponStats;

typedef struct
{
  WeaponStats stats;
  BulletInfo bulletInfo;
  int maxAmmo;
  int ammo;
  int numBullets;
} RangedWeapon;

typedef struct
{
  WeaponStats stats;
  SlashInfo slashInfo;
} MeleeWeapon;

typedef union
{
  RangedWeapon ranged;
  MeleeWeapon melee;
} WeaponUnion;

typedef struct
{
  const char *name;
  WeaponType type;
  WeaponUnion weapon;
} Weapon;

typedef struct
{
  int MAX_NUM_OF_WEAPONS;
  int currentNumOfWeapons;
  int currentWeapon;
  Weapon *weapons;
} Inventory;

typedef struct
{
  Vector2 patrolStart;
  Vector2 patrolEnd;
  float detectionRange;
  float attackCooldown;
  float lastAttackTime;
  float dodgePercentage; // Dodge or Parry or Block. Or do these three
                         // separately??
  float speed;
  STATE state;
} EnemyAI;

typedef struct
{
  char *name;
  GameObject object;
  Stats stats;
  Weapon weapon;
  E_TYPE type;
  EnemyAI ai;
  bool isMoving;
  int drawDirection; // 1 for right, -1 for left
} Enemy;

typedef struct
{
  // Player Info
  char *name;
  int ID;

  // Player Selection
  P_TYPE type;

  // Player Stats
  Stats stats;
  Inventory inventory;
  GameObject object;
  Input input;
  Experience experience;
  int score;
  int fire;
  int drawDirection; // 1 for right, -1 for left
  bool isMoving;
  DIRECTIONS direction; // to get info on the direction the player is facing.
} Player;

// ******************
// GAME STATE STRUCTS
// ******************

typedef struct
{
  int screenWidth;
  int screenHeight;
  bool fullscreen;
  int musicVolume;
  int soundVolume;
  bool sfx_on;
  bool playMusic;
  bool showTextures;
  bool showColliders;
  bool showFPS;
} Settings;

typedef union
{
  Weapon weapon;
  Enemy enemy;
  Player player;
} DictionaryEntry;

typedef struct
{
  int opcode;
  DictionaryEntry entry;
} Dictionary;

typedef struct TilesMapper
{
  unsigned int numOfTiles;
  char *mapper[MAX_TILES_NUM];
  char *path; // Path of the mapper file
} TilesMapper;

typedef struct Map
{
  unsigned int currentLevel; // NOTE: maybe convert it to a Level struct
  char *currentLevelPath;
  TilesMapper tilesMapper;
  int mapIds[MAX_ROW_NUM][MAX_COL_NUM][MAX_CELL_ID];
  bool loaded;
  unsigned int numOfRows;
  unsigned int numOfCols;
  Texture2D *textures;
  bool *isTexturesLoaded;
  float scale;
} Map;

typedef struct
{
  int numOfPlayers;
  Player *players;

  int numOfEnemies;
  Enemy *enemies;

  int numOfCombatActions;
  CombatAction *combatActions;

  int level;
  bool isGameOver;
  bool isFinished;
  Texture2D atlasTexture;  // The image atals containaing all the sprites and animations
  AtlasImage *atlasImages; // Linked List of individual sprites and animations data

  Dictionary *playerWeaponDictionary;
  Dictionary *enemyWeaponDictionary;
  Dictionary *characterDictionary;
  Dictionary *enemyDictionary;

  Settings settings;

  Map map;
} GameState;

#endif // STRUCTS_H
