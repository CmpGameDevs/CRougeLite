/***************************************************************
 *
 *
 *    
 *    ██╗    ██╗███████╗ █████╗ ██████╗  ██████╗ ███╗   ██╗
 *    ██║    ██║██╔════╝██╔══██╗██╔══██╗██╔═══██╗████╗  ██║
 *    ██║ █╗ ██║█████╗  ███████║██████╔╝██║   ██║██╔██╗ ██║
 *    ██║███╗██║██╔══╝  ██╔══██║██╔═══╝ ██║   ██║██║╚██╗██║
 *    ╚███╔███╔╝███████╗██║  ██║██║     ╚██████╔╝██║ ╚████║
 *     ╚══╝╚══╝ ╚══════╝╚═╝  ╚═╝╚═╝      ╚═════╝ ╚═╝  ╚═══╝
 *
 *     Weapon Module Header. (Game Object)
 *     Exposes the logic for the weapon object.
 *
 *     - Load Weapon
 *     - Use Weapon
 *     - Draw Weapon
 *
 **************************************************************/

#ifndef WEAPON_H
#define WEAPON_H

#include "../CRougeLite.h" // NOTE: declare global extern vars
#include "combatAction.h"

Weapon initWeapon(int opcode, bool isPlayer);
WeaponsInventory initInventory();
void updateRangedWeapon(Weapon *weapon, bool isFired, int ID, Vector2 src, Vector2 dest, float deltaTime, bool isFriendly);
void updateMeleeWeapon(Weapon *weapon, bool isFired, int ID, Vector2 src, Vector2 dest, float deltaTime);
void clearInventory(WeaponsInventory *inventory);

#endif // INPUT_H
