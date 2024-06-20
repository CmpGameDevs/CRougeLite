/**********************************************
* 
*    ███╗   ███╗ █████╗ ██████╗ 
*    ████╗ ████║██╔══██╗██╔══██╗
*    ██╔████╔██║███████║██████╔╝
*    ██║╚██╔╝██║██╔══██║██╔═══╝ 
*    ██║ ╚═╝ ██║██║  ██║██║     
*    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝     
**********************************************/

#include "../CRougeLite.h"

/**
 * initTilesMapper - initilizes the TilesMapper mapper with NULL,
 * and set path with mapper path.
*/
static void initTilesMapper();

/**
 * loadTilesMapper - loades the tiles mapper file at the set path into memory.
 *
 * return: 0 if no errors, 1 if an error occured.
*/
int loadTilesMapper();

/**
 * initMap - initilizes the map and the tilesMapper.
*/
void initMap();

/**
 * resetMapIds - resets the map ids to -1
*/
static void resetMapIds();

/**
 * parseLevelFile - parses the current level file into the memory
*/
void parseLevelFile();

/**
 * loadMapIds - reads a csv file and read it's content and
 * fill the map ids with it.
 *
 * return: 0 if no errors, 1 if an error occured.
*/
void drawMap();

/**
 * loadTileTexture - loads a tile at a certain index if it is not loaded.
 * @tileIdx: the index of the tile in the textures array
*/
static void loadTileTexture(int tileIdx);
