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

void setupEnemies();
void drawEnemies();
void updateEnemies();
void clearEnemies();

#endif // PLAYER_H
