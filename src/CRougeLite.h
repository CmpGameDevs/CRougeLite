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
#include <stdio.h>

//========================================================
// Global Shared Variables
//========================================================
extern Music music;

//========================================================
// Global Functions
//========================================================
Game_System *getGameSystemInstance();

// Init Functions
Game_System* initGameSystem();
void initSettings(Game_System* gameSystemInstance);
Player *initPlayer(const char *name, P_TYPE type, P_WEAPON weapon);
Enemy *initEnemy(E_TYPE type, E_WEAPON weapon);

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
