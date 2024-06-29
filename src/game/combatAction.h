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
#include "path.h"

/**
 *  initBullet - initialize a bullet object
 *
 * @param ID Player's ID
 * @param bulletInfo Bullet's information from the used weapon
 * @param pathInfo Path code for different path types
 * @param src Spawn position of bullet
 * @param dest The mouse click position
 *
 * @return Pointer to the combat action object
 *
 * @details Initialize a bullet object and link it to the player
 * by `ID`, its information is provided by the fired weapon.
 *
 */
CombatAction *initBullet(int ID, BulletInfo bulletInfo, Vector2 pathInfo, Vector2 src, Vector2 dest);

/**
 *  initRangedWeaponShoot - initialize a ranged weapon shoot object
 *
 * @param ID Player's ID
 * @param weapon Ranged weapon used
 *
 * @return Pointer to the combat action object
 * @param src Spawn position of bullet
 * @param dest The mouse click position
 * @param isFriendly Is the object shot by players or not
 *
 * @details Initialize a ranged weapon shoot object and link it to the player
 * by `ID`, its information is provided by the used ranged weapon.
 *
 */
void initRangedWeaponShoot(int ID, RangedWeapon weapon, Vector2 src, Vector2 dest, bool isFriendly);

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
 * updateCombatActions - update the combat actions
 */
void updateCombatActions();

/**
 * resolveCombatActionCollision - resolve the collision of a combat 
 * action object with other entity
 * 
 * @param action Pointer to the combat action object
 * @param entity Pointer to the hit entity
 * @param isFriendly Indicates if the combat action is friendly
 */
void resolveCombatActionCollision(CombatAction *action, Entity *entity, bool isFriendly);

/**
 * drawCombatActions - draw all combat actions and update them
 */
void drawCombatActions();

/**
 * clearCombatActions - free combat actions array from heap
 */
void clearCombatActions();

#endif // PLAYER_H
