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

//========================================================
// Global Functions
//========================================================
Game_System *getGameSystemInstance();

// Init Functions
Game_System *initGameSystem();
void initSettings(Game_System *gameSystemInstance);
Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon, RigidBody2d body, Vector2 position, int ID);
Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, RigidBody2d body, Vector2 position);
Bullet *initBullet(P_WEAPON weapon, int playerID, RigidBody2d body, Vector2 src, Vector2 dest);

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
