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
#include "defs.h"
#include "structs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//========================================================
// Global Shared Variables
//========================================================
extern Music music;
extern GameState *gameState;

//========================================================
// Global Functions
//========================================================
// Init Functions
GameState *initGameState();
void initDictionary();
void initSettings(GameState *gameState);
Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon, Vector2 position, int ID);
Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, Vector2 position);
Bullet *initBullet(int ID, BulletInfo *bulletInfo, Vector2 src, Vector2 dest);

// Clear Resources Functions
void clearGameState();
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
