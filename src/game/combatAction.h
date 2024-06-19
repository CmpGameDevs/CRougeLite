/***************************************************************
 *
 *
 * 
 *     ██████╗ ██████╗ ███╗   ███╗██████╗  █████╗ ████████╗   █████╗  ██████╗████████╗██╗ ██████╗ ███╗   ██╗
 *    ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██╔══██╗╚══██╔══╝  ██╔══██╗██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║
 *    ██║     ██║   ██║██╔████╔██║██████╔╝███████║   ██║     ███████║██║        ██║   ██║██║   ██║██╔██╗ ██║
 *    ██║     ██║   ██║██║╚██╔╝██║██╔══██╗██╔══██║   ██║     ██╔══██║██║        ██║   ██║██║   ██║██║╚██╗██║
 *    ╚██████╗╚██████╔╝██║ ╚═╝ ██║██████╔╝██║  ██║   ██║     ██║  ██║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║
 *     ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═════╝ ╚═╝  ╚═╝   ╚═╝     ╚═╝  ╚═╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
 *                                                                                                          
 *     Combat Action Module Header. (Game Object)
 *     Exposes the logic for the Combat Action object.
 *
 *     - Setup Combat Action
 *     - Update Combat Action
 *     - Draw Combat Action
 *     - Check for collision
 *     - Clear Combat Action
 *
 **************************************************************/

#ifndef COMBAT_ACTION_H
#define COMBAT_ACTION_H

#include "../CRougeLite.h"

/**
 *  initBullet - initialize a bullet object
 * 
 * @param ID Player's ID
 * @param bulletInfo Bullet's information from the used weapon
 * 
 * @return Pointer to the combat action object
 * 
 * @details Initialize a bullet object and link it to the player
 * by `ID`, its information is provided by the fired weapon.
 *
 */
CombatAction *initBullet(int ID, BulletInfo bulletInfo, Vector2 src, Vector2 dest);

/**
 *  initSlash - initialize a slash object
 * 
 * @param ID Player's ID
 * @param bulletInfo Slash's information from the used melee weapon
 * 
 * @return Pointer to the combat action object
 * 
 * @details Initialize a slash object and link it to the player
 * by `ID`, its information is provided by the used melee weapon.
 *
 */
CombatAction *initSlash(int ID, SlashInfo slashInfo, Vector2 src, Vector2 dest);

/**
 * drawCombatActions - draw all combat actions and update them
 */
void drawCombatActions();

/**
 * clearCombatActions - free combat actions array from heap
 */
void clearCombatActions();

#endif // PLAYER_H
