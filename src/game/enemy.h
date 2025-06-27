/***************************************************************
 *
 *
 *    
 *    ███████╗███╗   ██╗███████╗███╗   ███╗██╗   ██╗
 *    ██╔════╝████╗  ██║██╔════╝████╗ ████║╚██╗ ██╔╝
 *    █████╗  ██╔██╗ ██║█████╗  ██╔████╔██║ ╚████╔╝ 
 *    ██╔══╝  ██║╚██╗██║██╔══╝  ██║╚██╔╝██║  ╚██╔╝  
 *    ███████╗██║ ╚████║███████╗██║ ╚═╝ ██║   ██║   
 *    ╚══════╝╚═╝  ╚═══╝╚══════╝╚═╝     ╚═╝   ╚═╝   
 *
 *     Enemy Module Header. (Game Object)
 *     Exposes the logic for the enemy object.
 *
 *     - Setup Enemy
 *     - Update Enemy
 *     - Draw Enemy
 *     - Clear Enemy
 *
 **************************************************************/

#ifndef ENEMY_H
#define ENEMY_H

#include "../CRougeLite.h"
#include "weapon.h"

/**
 * setupEnemies - initialize all enemies manually to the game state
 */
void setupEnemies();

/**
 * drawEnemies - draw enemies on the scene
 */
void drawEnemies();

/**
 * updateEnemies - update the enemies objects every game tick
 */
void updateEnemies();

/** 
  * updateEnemyPath - update the path of all enemies to the closest
  **/   
void updateEnemyPath();

/**
 * clearEnemies - free enemies array from heap
 */
void clearEnemies();

#endif // PLAYER_H
