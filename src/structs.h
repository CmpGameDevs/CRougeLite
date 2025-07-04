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

typedef enum { ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_P_COMBAT_ACTION, ENTITY_E_COMBAT_ACTION, ENTITY_MISC } EntityType;

typedef enum { BODY_STATIC, BODY_DYNAMIC, BODY_KINEMATIC, BODY_GHOST } BodyType;

typedef enum { ACTION_NONE, ACTION_BULLET, ACTION_SLASH } CombatActionType;

typedef enum { RANGED_WEAPON, MELEE_WEAPON, NUM_OF_WEAPON_TYPES } WeaponType;

typedef enum { E_SWORD, NUM_OF_E_WEAPON } E_WEAPON;

typedef enum { CAT, WEREWOLF, PYROMANIAC, KNIGHT, NUM_OF_P_TYPE } P_TYPE;

typedef enum {
  P_GUN,
  P_LONG_SWORD,
  P_MISSILE_LAUNCHER,
  NUM_OF_P_WEAPON
} P_WEAPON;

typedef enum {
  INPUT_UP = 1,
  INPUT_DOWN = 1 << 1,
  INPUT_LEFT = 1 << 2,
  INPUT_RIGHT = 1 << 3,
  INPUT_INTERACT = 1 << 4,
  INPUT_INVENTORY_1 = 1 << 5,
  INPUT_INVENTORY_2 = 1 << 6,
  INPUT_INVENTORY_3 = 1 << 7,
  INPUT_INVENTORY_4 = 1 << 8,
} InputType;

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
} DIRECTIONS;

typedef enum { E_CIVILIAN, E_FARMER, E_KNIGHT, E_SLIME, NUM_OF_E_TYPE } E_TYPE;

typedef enum {
  TILE_TERRAIN,      // Walkable ground tiles
  TILE_WALL,         // Solid collision tiles  
  TILE_DECORATION,   // Non-interactive visual elements
  TILE_INTERACTABLE, // Doors, switches, chests
  TILE_PICKABLE,     // Keys, coins, consumables
  TILE_DESTRUCTIBLE, // Breakable objects
  TILE_HAZARD,       // Damaging tiles (spikes, lava)
  NUM_TILE_TYPES
} TileType;

typedef enum {
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

typedef struct {
  void *first;
  void *second;
} Pair;

typedef struct {
  int first;
  int second;
} CoordPair;
typedef struct {
  double first;      
  CoordPair second;  
} pPair;

typedef struct {
    pPair* data;
    int size;
    int capacity;
} MinHeap;

typedef struct Entity Entity;

typedef struct {
  Entity *entities;
  int numOfEntities;
  int hitCount;
  int checkedCount;
} Hit;

typedef struct CTransform {
  Vector2 position;
  float rotation;
  float frequency;
  float amplitude;
  Vector2 scale;
} CTransform;

typedef struct {
  Vector2 velocity;
  Vector2 acceleration;
  float drag;
  BodyType type;
} RigidBody2D;

typedef struct {
  Rectangle bounds;
  bool isColliding;
} Collider2D;

typedef struct AtlasImage {
  char *filename;
  Rectangle source;
  Vector2 origin;
  struct AtlasImage *next;
} AtlasImage;

// FIXME: Rework SpriteRenderer or remove it if not used.
// Atlas Image could do the same work if it has the dest rect.
typedef struct {
  Texture2D texture;
  int width;
  int height;
} SpriteRenderer;

typedef struct SpriteAnimation {
  char *frameNames[MAX_FRAMES_PER_ANIMATION];
  int currentFrame;
  int frameCount;
  int numOfFrames;
  int fps;
  bool isLooping;
  bool isFinished;
} SpriteAnimation;

typedef struct {
  SpriteAnimation animations[MAX_ANIMATION_STATES];
  int currentState;
  bool isFinished;
} Animator;

typedef struct {
  int up;
  int down;
  int left;
  int right;
  int shoot;
  int action;
  int interact;
  int *weapons;
  float mouseWheelMove;
} Input;

typedef struct {
  CTransform transform;
  RigidBody2D rigidBody;
  Collider2D collider;
  SpriteRenderer spriteRenderer;
  Animator animator;
} GameObject;

// *********************
// GAME SPECIFIC STRUCTS
// *********************

typedef struct {
  float currentHealth;
  float maxHealth;
} Health;

// IDK if those affect the other structs or not (like leveling up)
typedef struct {
  float power;
  float speed;
  float cooldown;
} Attack;

typedef struct {
  float constant;   // Higher value means less effective defense
  float value;
  int nearHitValue; // Blocked on the last second.
  // TODO: Add defense for different type of attacks?
} Defense;

typedef struct {
  int xp;
  int level;
} Experience;

typedef struct {
  Health health;
  Attack attack;
  Defense defense;
  double speed;
} Stats;

typedef struct {
  
  float bulletSpeed;
  float bulletDamage;
  float bulletRange;
  float bulletHealth;
  bool isTracking;
  int pathCode;
  int enemyID;
  float critMultiplier;
  float critChance;
  GameObject object;
} BulletInfo;

typedef struct {
  int playerID;
  BulletInfo bulletInfo;
  Vector2 startPosition; // To know if the bullet exceeded the range.
  Vector2 dest;
} Bullet;

typedef struct {
  float criticalChance;
  float slashRange;
  float slashDamage;
  bool isActive;
  GameObject object;
} SlashInfo;

typedef struct {
  int playerID;
  SlashInfo slashInfo;
} Slash;

typedef union {
  Bullet bullet;
  Slash slash;
} CombatActionUnion;

typedef struct {
  unsigned int ID;
  float angle;
  CombatActionUnion action;
  CombatActionType type;
  Hit hit;
  bool isFriendly;
} CombatAction;

typedef struct {
  int damage;
  float cooldown;
  float lastUseTime;
  SpriteRenderer weaponSprite;
} WeaponStats;

typedef struct {
  WeaponStats stats;
  BulletInfo bulletInfo;
  int maxAmmo;
  int ammo;
  int numBullets;
} RangedWeapon;

typedef struct {
  WeaponStats stats;
  SlashInfo slashInfo;
} MeleeWeapon;

typedef union {
  RangedWeapon ranged;
  MeleeWeapon melee;
} WeaponUnion;

typedef struct {
  const char *name;
  WeaponType type;
  WeaponUnion weapon;
} Weapon;

typedef struct {
  int MAX_NUM_OF_WEAPONS;
  int currentNumOfWeapons;
  int currentWeapon;
  Weapon *weapons;
} WeaponsInventory;

typedef struct {
  Vector2 patrolStart;
  Vector2 patrolEnd;
  float detectionRange;
  float attackCooldown;
  float lastAttackTime;
  float dodgePercentage; // Dodge or Parry or Block. Or do these three
                         // separately??
  float speed;
  CoordPair* path; // Path to follow, if any.
  int currentPathIndex;
  int pathLength; // Length of the path array.
  int minDistanceToAttack;
  bool inLineOfSight; // If the enemy can see the player.
  STATE state;
} EnemyAI;

typedef struct {
  unsigned int ID;
  char *name;
  GameObject object;
  Stats stats;
  Weapon weapon;
  E_TYPE type;
  EnemyAI ai;
  bool isMoving;
  int drawDirection; // 1 for right, -1 for left
} Enemy;

typedef struct {
  int tileId;
  int count;
} CollectibleItem;

typedef struct {
  // Player Info
  char *name;
  unsigned int ID;

  // Player Selection
  P_TYPE type;

  // Player Stats
  Stats stats;
  WeaponsInventory inventory;
  GameObject object;
  Input input;
  Experience experience;
  int score;
  int fire;
  int drawDirection; // 1 for right, -1 for left
  CollectibleItem collectedItems[MAX_COLLECTED_ITEMS];
  bool isMoving;
  DIRECTIONS direction; // to get info on the direction the player is facing.
  int interactableTileIndex;
} Player;

typedef union {
  Player *player;
  Enemy *enemy;
  CombatAction *action;
} EntityUnion;

typedef struct Entity {
  EntityType type;
  EntityUnion entity;
  unsigned int ID;
} Entity;

// ******************
// GAME STATE STRUCTS
// ******************

typedef struct {
  int screenWidth;
  int screenHeight;
  int musicVolume;
  int soundVolume;
  bool fullscreen;
  bool sfx_on;
  bool playMusic;
  bool showTextures;
  bool showTexturesBounds;
  bool showColliders;
  bool showPaths;
  bool showFPS;
  bool showDebugMenu;
  bool showInventory;
  float zoom;
} Settings;

typedef union {
  Weapon weapon;
  Enemy enemy;
  Player player;
} DictionaryEntry;

typedef struct {
  int opcode;
  DictionaryEntry entry;
} Dictionary;
typedef enum {
  TILE_PROP_NONE       = 0,
  TILE_PROP_WALKABLE   = 1 << 0,
  TILE_PROP_SOLID      = 1 << 1,
  TILE_PROP_COLLECTIBLE = 1 << 2,
  TILE_PROP_INTERACTIVE = 1 << 3,
  TILE_PROP_DESTRUCTIBLE = 1 << 4,
  TILE_PROP_HAZARDOUS   = 1 << 5
} TileProperties;

typedef enum {
  INTERACT_TOGGLE,
  INTERACT_ONCE,
  INTERACT_CYCLE,
  INTERACT_CONDITIONAL
} InteractBehavior;

typedef struct {
  int tileId;
  InteractBehavior behavior;
  int nextStates[MAX_TOGGLE_STATES];
  int stateCount;
  int requiredItem;
  int lootTable[4];
  int lootCount;
} InteractableMapping;

typedef struct {
  char *filename;
  TileType type;
  TileProperties properties;
} TileInfo;

typedef struct {
  unsigned int numOfTiles;
  TileInfo tileInfo[MAX_TILES_NUM];
  InteractableMapping interactableMappings[MAX_INTERACTABLE_MAPPINGS];
  int numInteractableMappings;
  char *path; // Path of the mapper file
} TilesMapper;

typedef struct {
  Entity objectIndices[MAX_OBJECTS_PER_CELL];
  unsigned int objectCount;
} GridCell;

typedef struct {
  unsigned int currentLevel; // NOTE: maybe convert it to a Level struct
  char *currentLevelPath;
  TilesMapper tilesMapper;
  int mapIds[MAX_ROW_NUM][MAX_COL_NUM][MAX_CELL_ID];
  GridCell grid[MAX_ROW_NUM][MAX_COL_NUM];
  unsigned int numOfRows;
  unsigned int numOfCols;
  int tileHeight;
  int tileWidth;
  float scale;
} Map;

typedef struct {
  int numOfPlayers;
  Player *players;

  int numOfEnemies;
  Enemy *enemies;

  int numOfCombatActions;
  CombatAction *combatActions;

  int level;
  bool isGameStarted;
  bool isGameOver;
  bool isFinished;
  Texture2D mainMenuBackground;
  Texture2D atlasTexture; // The image atals containaing all the sprites and
                          // animations
  AtlasImage
      *atlasImages; // Linked List of individual sprites and animations data

  Dictionary *playerWeaponDictionary;
  Dictionary *enemyWeaponDictionary;
  Dictionary *characterDictionary;
  Dictionary *enemyDictionary;

  Settings settings;

  Camera2D camera;

  Map map;
} GameState;

#endif // STRUCTS_H
