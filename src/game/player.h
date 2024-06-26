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
 * clearPlayers - free players array from heap
 */
void clearPlayers();

#endif // PLAYER_H
