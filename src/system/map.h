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
 * drawTileTexture - takes the info of a tile and draw its texture.
 *
 * @tileIdx: the index of the tile in the textures array.
 * @dest: the dest Rectangle location to draw on in the screen.
 * @origin: the origin vector of the texture.
 * @rotation: the rotation value of the texture.
 * @tint: the color of the texture.
 * @flipX: boolean to flip the texture or not.
 */
static void drawTileTexture(int tileIdx, Rectangle dest, Vector2 origin,
                            float rotation, Color tint, bool flipX);
