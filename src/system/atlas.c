/***************************************************************
 *
 *
 *      █████╗ ████████╗██╗      █████╗ ███████╗
 *     ██╔══██╗╚══██╔══╝██║     ██╔══██╗██╔════╝
 *     ███████║   ██║   ██║     ███████║███████╗
 *     ██╔══██║   ██║   ██║     ██╔══██║╚════██║
 *     ██║  ██║   ██║   ███████╗██║  ██║███████║
 *     ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝
 *
 *
 *     Handles loading of atlas and getting the texture.
 *     Authored by Amir Kedis
 *
 *
 **************************************************************/

#include "atlas.h"
#include <raylib.h>

void initAtlas() {
  Game_System *game = getGameSystemInstance();
  game->atlasTexture = LoadTexture("./src/resources/gfx/atlas.png");
}
