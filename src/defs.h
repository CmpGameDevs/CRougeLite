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

// COLLISION
#define MAX_OBJECTS_PER_CELL 10

// ANIMATION
#define MAX_FRAMES_PER_ANIMATION 60
#define MAX_ANIMATION_STATES 20

#endif // DEFS_H
