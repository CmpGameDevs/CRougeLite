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

#include "common.h"

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
Enemy *initEnemy(E_TYPE type, E_WEAPON weapon, Vector2 position);
Bullet *initBullet(int ID, BulletInfo *bulletInfo, Vector2 src, Vector2 dest);

// Clear Resources Functions
void clearGameState();
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
