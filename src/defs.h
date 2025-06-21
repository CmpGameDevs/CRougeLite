/*********************************************************
 *
 * 
 *   ██████╗ ███████╗███████╗███████╗
 *   ██╔══██╗██╔════╝██╔════╝██╔════╝
 *   ██║  ██║█████╗  █████╗  ███████╗
 *   ██║  ██║██╔══╝  ██╔══╝  ╚════██║
 *   ██████╔╝███████╗██║     ███████║
 *   ╚═════╝ ╚══════╝╚═╝     ╚══════╝
 *
 *
 *   Defination and constants for the game
 *
 *
 *********************************************************/


#ifndef DEFS_H
#define DEFS_H

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define DEFAULT_MAX_PLAYERS 4
#define DEFAULT_MAX_ENEMIES 100
#define DEFAULT_MAX_COMBAT_ACTIONS 100

#define PLAYER_SPEED 10

#define MAX_FILENAME_LENGTH 256

// MAP
#define MAX_TILES_NUM 1024
#define MAX_ROW_NUM 30
#define MAX_COL_NUM 60
#define MAX_CELL_ID 5

// MAP LAYER ORGANIZATION
#define LAYER_TERRAIN      0  // Ground, floors, basic terrain
#define LAYER_WALLS        1  // Walls, collision objects, solid structures  
#define LAYER_DECORATION   2  // Non-interactive decorations
#define LAYER_INTERACTABLE 3  // Doors, switches, chests
#define LAYER_PICKABLE     4  // Keys, coins, consumables

// INTERACTABLE CATEGORIES AND MAPPING
#define MAX_INTERACTABLE_MAPPINGS 100
#define MAX_TOGGLE_STATES 4

// PLAYER
#define MAX_COLLECTED_ITEMS 7

// COLLISION
#define MAX_OBJECTS_PER_CELL 10

// ANIMATION
#define MAX_FRAMES_PER_ANIMATION 60
#define MAX_ANIMATION_STATES 20

#endif // DEFS_H
