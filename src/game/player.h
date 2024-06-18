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


void setupPlayers();
void drawPlayers();
void updatePlayers();
void clearPlayers();

#endif // PLAYER_H
