/***************************************************************
 *
 *
 *    ██████╗ ██╗      █████╗ ██╗   ██╗███████╗██████╗ 
 *    ██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝██╔════╝██╔══██╗
 *    ██████╔╝██║     ███████║ ╚████╔╝ █████╗  ██████╔╝
 *    ██╔═══╝ ██║     ██╔══██║  ╚██╔╝  ██╔══╝  ██╔══██╗
 *    ██║     ███████╗██║  ██║   ██║   ███████╗██║  ██║
 *    ╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝
 *
 *     Player Module Header. (Game Object)
 *     Exposes the logic for the player object.
 *
 *     - Setup Player
 *     - Update Player
 *     - Draw Player
 *     - Clear Player
 *
 **************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "../CRougeLite.h"
#include "weapon.h"
#include "../system/map.h"

/**
 * setupPlayers - initialize all enemies manually to the game state
 */
void setupPlayers();

/**
 * drawPlayers - draw players on the scene
 */
void drawPlayers();

/**
 * updatePlayers - update the players objects every game tick
 */
void updatePlayers();

/**
 * addToInventory - add an item to the player inventory
 * 
 * @param player The player to add the item to
 * @param itemId The ID of the item to add
 * 
 * @return true if the item was added successfully, false otherwise
 */
bool addToInventory(Player *player, int itemId);

/**
 * useFromInventory - remove an item from the player inventory
 * 
 * @param player The player to remove the item from
 * @param itemId The ID of the item to remove
 * 
 * @return true if the item was removed successfully, false otherwise
 */
bool useFromInventory(Player *player, int itemId);

/**
 * clearPlayers - free players array from heap
 */
void clearPlayers();

#endif // PLAYER_H
