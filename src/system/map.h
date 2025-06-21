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
 * getTileType - gets the type of a tile based on its ID.
 * 
 * @param tileId The ID of the tile to get the type for
 * 
 * @return The TileType of the tile, or TILE_TERRAIN if the ID is invalid
 */
TileType getTileType(int tileId);

/**
 * isWalkable - checks if a specific cell in the map is walkable
 * 
 * @param row The row index of the cell
 * @param col The column index of the cell
 * 
 * @return true if the cell is walkable, false otherwise
 */
bool isWalkable(int row, int col);

/**
 * getInteractableAt - gets the interactable tile ID at a specific position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * 
 * @return The interactable tile ID, or -1 if no interactable exists at that position
 */
int getInteractableAt(int row, int col);

/**
 * getPickableAt - gets the pickable item ID at a specific position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * 
 * @return The pickable item ID, or -1 if no pickable exists at that position
 */
int getPickableAt(int row, int col);

/**
 * removePickable - removes a pickable item from the map at the specified position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 */
void removePickable(int row, int col);

/**
 * placePickable - places a pickable item at the specified position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * @param itemId The ID of the item to place
 */
void placePickable(int row, int col, int itemId);

/**
 * toggleInteractable - toggles an interactable tile at the specified position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * @param playerInventory The player's inventory to check for required items
 */
void toggleInteractable(int row, int col, CollectibleItem playerInventory[]);

/**
 * getInteractableMapping - gets the interactable mapping for a tile ID
 */
InteractableMapping* getInteractableMapping(int tileId);

/**
 * canInteractWith - checks if player can interact with a tile (has required items)
 */
bool canInteractWith(int tileId, CollectibleItem playerInventory[]);

/**
 * getInteractableNextState - gets the next state for an interactable
 */
int getInteractableNextState(int currentTileId);

/**
 * spawnLootFromMapping - spawns loot items based on interactable mapping
 */
void spawnLootFromMapping(int row, int col, InteractableMapping *mapping);

/**
 * getTileProperties - returns the properties bitflags of a tile
 * 
 * @param tileId The ID of the tile to get properties for
 * 
 * @return The TileProperties bitflags for the tile, or TILE_PROP_NONE if invalid
 */
TileProperties getTileProperties(int tileId);

/**
 * isPropertySet - checks if a specific property is set for a tile ID
 * 
 * @param tileProperties: the properties bitflags of the tile
 * @param property: the property to check
 * 
 * @return: true if the property is set, false otherwise
 */
bool isPropertySet(TileProperties tileProperties, TileProperties property);

/**
 * isTileWalkable - checks if a tile is walkable based on its properties
 * 
 * @param tileId The ID of the tile to check
 * 
 * @return true if the tile is walkable, false otherwise
 */
bool isTileWalkable(int tileId);

/**
 * isTileCollectible - checks if a tile is collectible
 * 
 * @param tileId The ID of the tile to check
 * 
 * @return true if the tile is collectible, false otherwise
 */
bool isTileCollectible(int tileId);

/**
 * isTileInteractive - checks if a tile is interactive
 * 
 * @param tileId The ID of the tile to check
 * 
 * @return true if the tile is interactive, false otherwise
 */
bool isTileInteractive(int tileId);

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
