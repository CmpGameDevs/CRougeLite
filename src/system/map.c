/**********************************************
 *
 *    ███╗   ███╗ █████╗ ██████╗
 *    ████╗ ████║██╔══██╗██╔══██╗
 *    ██╔████╔██║███████║██████╔╝
 *    ██║╚██╔╝██║██╔══██║██╔═══╝
 *    ██║ ╚═╝ ██║██║  ██║██║
 *    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝
 **********************************************/

#include "map.h"
#include "draw.h"
#include <raylib.h>
#include <string.h>
#include "A-Star.h"

/**
 * initTilesMapper - initilizes the TilesMapper mapper with NULL,
 * and set path with mapper path.
 */
static void initTilesMapper() {
  GameState *game_system = gameState;
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);

  for (int i = 0; i < MAX_TILES_NUM; i++) {
    tiles_mapper->tileInfo[i].filename = NULL;
    tiles_mapper->tileInfo[i].type = TILE_TERRAIN;
    tiles_mapper->tileInfo[i].properties = TILE_PROP_NONE;
  }

  // Initialize interactable mappings
  for (int i = 0; i < MAX_INTERACTABLE_MAPPINGS; i++) {
    tiles_mapper->interactableMappings[i].tileId = -1;
    tiles_mapper->interactableMappings[i].behavior = INTERACT_TOGGLE;
    tiles_mapper->interactableMappings[i].stateCount = 0;
    tiles_mapper->interactableMappings[i].requiredItem = -1;
    tiles_mapper->interactableMappings[i].lootCount = 0;
    for (int j = 0; j < MAX_TOGGLE_STATES; j++) {
      tiles_mapper->interactableMappings[i].nextStates[j] = -1;
    }
    for (int j = 0; j < 4; j++) {
      tiles_mapper->interactableMappings[i].lootTable[j] = -1;
    }
  }

  tiles_mapper->path = "./src/resources/gfx/map-assets/mapper";
  tiles_mapper->numOfTiles = 0;
  tiles_mapper->numInteractableMappings = 0;

  loadTilesMapper();
}

/**
 * loadTilesMapper - loads the tiles mapper file at the set path into memory.
 * 
 * return: 0 if no errors, 1 if an error occurred.
 */
int loadTilesMapper() {
  GameState *game_system = gameState;
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);

  char *file_content = LoadFileText(tiles_mapper->path);
  if (!file_content) {
    fprintf(stderr, "Error: Could not load mapper file at %s\n", tiles_mapper->path);
    exit(EXIT_FAILURE);
  }

  char *line = strtok(file_content, "\n");
  while (line) {
    // Skip comments and empty lines
    if (line[0] == '#' || line[0] == '\0' || line[0] == '\n') {
      line = strtok(NULL, "\n");
      continue;
    }    // Check if this is an interactable mapping line
    if (strncmp(line, "INTERACTABLE", 12) == 0) {
      // Format: INTERACTABLE tile_id behavior [next_states] [required_item] [loot_items]
      int tileId;
      char behaviour_str[32];
      char next_states_str[64] = "-1";
      char required_item_str[32] = "-1";
      char loot_items_str[128] = "-1";
      
      int parsed = sscanf(line, "INTERACTABLE %d %s %s %s %s",
                          &tileId, behaviour_str, next_states_str, required_item_str, loot_items_str);
      if (parsed < 3) {
        fprintf(stderr, "Warning: Skipping malformed INTERACTABLE line: %s\n", line);
        line = strtok(NULL, "\n");
        continue;
      }

      InteractableMapping *mapping = &tiles_mapper->interactableMappings[tiles_mapper->numInteractableMappings];
      mapping->tileId = tileId;

      // Parse behavior
      if (strcmp(behaviour_str, "TOGGLE") == 0) {
        mapping->behavior = INTERACT_TOGGLE;
      } else if (strcmp(behaviour_str, "ONCE") == 0) {
        mapping->behavior = INTERACT_ONCE;
      } else if (strcmp(behaviour_str, "CYCLE") == 0) {
        mapping->behavior = INTERACT_CYCLE;
      } else if (strcmp(behaviour_str, "CONDITIONAL") == 0) {
        mapping->behavior = INTERACT_CONDITIONAL;
      } else {
        fprintf(stderr, "Warning: Unknown interaction behavior: %s\n", behaviour_str);
        mapping->behavior = INTERACT_TOGGLE;
      }
      
      // Parse next states
      mapping->stateCount = 0;
      char *state_ptr = next_states_str;
      char *comma_pos;
      while (state_ptr && mapping->stateCount < MAX_TOGGLE_STATES) {
        comma_pos = strchr(state_ptr, ',');
        if (comma_pos) {
          *comma_pos = '\0';
          mapping->nextStates[mapping->stateCount] = atoi(state_ptr);
          mapping->stateCount++;
          state_ptr = comma_pos + 1;
        } else {
          mapping->nextStates[mapping->stateCount] = atoi(state_ptr);
          mapping->stateCount++;
          break;
        }
      }
      
      // Parse required item
      mapping->requiredItem = atoi(required_item_str);
      
      // Parse loot table
      mapping->lootCount = 0;
      if (strcmp(loot_items_str, "-1") != 0) {
        // Parse comma-separated loot manually
        char *loot_ptr = loot_items_str;
        char *comma_pos;
        while (loot_ptr && mapping->lootCount < 4) {
          comma_pos = strchr(loot_ptr, ',');
          if (comma_pos) {
            *comma_pos = '\0';
            mapping->lootTable[mapping->lootCount] = atoi(loot_ptr);
            mapping->lootCount++;
            loot_ptr = comma_pos + 1;
          } else {
            mapping->lootTable[mapping->lootCount] = atoi(loot_ptr);
            mapping->lootCount++;
            break;
          }
        }
      }
      
      printf("[DEBUG] Loaded interactable mapping: tile %d, behavior %d, states %d, required %d, loot %d\n",
             mapping->tileId, mapping->behavior, mapping->stateCount, mapping->requiredItem, mapping->lootCount);
      
      tiles_mapper->numInteractableMappings++;
      line = strtok(NULL, "\n");
      continue;
    }

    int id;
    char tile_name[128];
    char type_str[32];
    char properties_str[256] = {0};

    // Format: ID FILENAME TYPE [PROPERTIES...]
    int parsed = sscanf(line, "%d %s %s", &id, tile_name, type_str);
    
    if (parsed < 3) {
      fprintf(stderr, "Warning: Skipping malformed line: %s\n", line);
      line = strtok(NULL, "\n");
      continue;
    }
    
    char *properties_start = strstr(line, type_str);
    if (properties_start) {
      properties_start += strlen(type_str);
      while (*properties_start == ' ' || *properties_start == '\t') {
        properties_start++;
      }

      if (strlen(properties_start) > 0) {
        strncpy(properties_str, properties_start, sizeof(properties_str) - 1);
        properties_str[sizeof(properties_str) - 1] = '\0';
      }
    }

    if (id >= MAX_TILES_NUM) {
      fprintf(stderr, "Warning: Tile ID %d exceeds MAX_TILES_NUM\n", id);
      line = strtok(NULL, "\n");
      continue;
    }

    // Allocate and store filename
    tiles_mapper->tileInfo[id].filename = malloc((strlen(tile_name) + 1) * sizeof(char));
    if (tiles_mapper->tileInfo[id].filename == NULL) {
      fprintf(stderr, "Error occurred while allocating memory for tile info %d\n", id);
      UnloadFileText(file_content);
      exit(EXIT_FAILURE);
    }
    strcpy(tiles_mapper->tileInfo[id].filename, tile_name);

    // Parse tile type
    if (strcmp(type_str, "TERRAIN") == 0) {
      tiles_mapper->tileInfo[id].type = TILE_TERRAIN;
    } else if (strcmp(type_str, "WALL") == 0) {
      tiles_mapper->tileInfo[id].type = TILE_WALL;
    } else if (strcmp(type_str, "DECORATION") == 0) {
      tiles_mapper->tileInfo[id].type = TILE_DECORATION;
    } else if (strcmp(type_str, "INTERACTABLE") == 0) {
      tiles_mapper->tileInfo[id].type = TILE_INTERACTABLE;
    } else if (strcmp(type_str, "PICKABLE") == 0) {
      tiles_mapper->tileInfo[id].type = TILE_PICKABLE;
    } else if (strcmp(type_str, "DESTRUCTIBLE") == 0) {
      tiles_mapper->tileInfo[id].type = TILE_DESTRUCTIBLE;
    } else if (strcmp(type_str, "HAZARD") == 0) {
      tiles_mapper->tileInfo[id].type = TILE_HAZARD;
    } else {
      fprintf(stderr, "Warning: Unknown tile type '%s' for tile %d, defaulting to TERRAIN\n", type_str, id);
      tiles_mapper->tileInfo[id].type = TILE_TERRAIN;
    }

    // Parse properties
    tiles_mapper->tileInfo[id].properties = TILE_PROP_NONE;
    if (strlen(properties_str) > 0) {
      if (strstr(properties_str, "walkable")) {
        tiles_mapper->tileInfo[id].properties |= TILE_PROP_WALKABLE;
      }
      if (strstr(properties_str, "solid")) {
        tiles_mapper->tileInfo[id].properties |= TILE_PROP_SOLID;
      }
      if (strstr(properties_str, "collectible")) {
        tiles_mapper->tileInfo[id].properties |= TILE_PROP_COLLECTIBLE;
      }
      if (strstr(properties_str, "interactive")) {
        tiles_mapper->tileInfo[id].properties |= TILE_PROP_INTERACTIVE;
      }
      if (strstr(properties_str, "destructible")) {
        tiles_mapper->tileInfo[id].properties |= TILE_PROP_DESTRUCTIBLE;
      }
      if (strstr(properties_str, "hazardous")) {
        tiles_mapper->tileInfo[id].properties |= TILE_PROP_HAZARDOUS;
      }
    }

    tiles_mapper->numOfTiles += 1;
    line = strtok(NULL, "\n");
  }
  UnloadFileText(file_content);
  printf("Loaded %d tiles and %d interactable mappings from enhanced mapper\n", 
         tiles_mapper->numOfTiles, tiles_mapper->numInteractableMappings);
  return 0;
}

/**
 * initMap - initilizes the map and the tilesMapper.
 */
void initMap() {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);

  initTilesMapper();

  map->currentLevel = 0;
  map->numOfRows = 0;
  map->numOfCols = 0;
  map->currentLevelPath =
      "./src/resources/gfx/map-assets/level_one/level_one_map.csv";
  map->scale = 3.0f;
  map->tileWidth = 16;
  map->tileHeight = 16;

  parseLevelFile();
}

static void resetMapIds() {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);

  for (int row = 0; row < MAX_ROW_NUM; row++) {
    for (int col = 0; col < MAX_COL_NUM; col++) {
      for (int id = 0; id < MAX_CELL_ID; id++) {
        map->mapIds[row][col][id] = -1;
      }
    }
  }
}

/**
 * parseLevelFile - parses the current level file into the memory
 */
void parseLevelFile() {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);

  resetMapIds();

  char *file_content = LoadFileText(map->currentLevelPath);

  char *save_ptr1 = NULL;
  char *row = strtok_r(file_content, "\n", &save_ptr1);

  int currentRow = 0;
  while (row) {
    char *save_ptr2 = NULL;
    char *col = strtok_r(row, ",", &save_ptr2);

    int currentCol = 0;
    while (col) {
      char *save_ptr3 = NULL;
      char *id = strtok_r(col, " ", &save_ptr3);

      int currentId = 0;
      while (id) {
        map->mapIds[currentRow][currentCol][currentId] = atoi(id);

        currentId++;
        id = strtok_r(NULL, " ", &save_ptr3);
      }

      currentCol++;
      col = strtok_r(NULL, ",", &save_ptr2);
    }

    map->numOfCols = currentCol;
    currentRow++;
    row = strtok_r(NULL, "\n", &save_ptr1);
  }

  map->numOfRows = currentRow;
  UnloadFileText(file_content);
}

/**
 * drawMap - draws the map on the screen
 */
void drawMap() {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);
  TilesMapper *tilesMapper = &(game_system->map.tilesMapper);

  for (int row = 0; row < map->numOfRows; row++) {
    for (int col = 0; col < map->numOfCols; col++) {
      int idIdx = 0;

      do {
        int tileId = map->mapIds[row][col][idIdx++];
        if (tileId != -1) {
          int tileWidth = map->tileWidth, tileHeight = map->tileHeight;

          Rectangle dest = {(float)(col * tileWidth) * map->scale,
                            (float)(row * tileHeight) * map->scale,
                            (float)tileWidth * map->scale,
                            (float)tileHeight *
                                map->scale}; // Scaling the drawn texture by 2

          drawTileTexture(tileId, dest, (Vector2){0, 0}, 0.f, RAYWHITE, false);
        }
      } while (idIdx < 5);
    }
  }
}

/**
 * drawPath - Draws a path as colored cells
 *
 * @param path Array of path coordinates (row, col) -> (y, x)
 * @param pathLength Length of the path array
 * @param color Color to draw the path with
 */
void drawPath(CoordPair *path, int pathLength, Color color)
{
    if (!path || pathLength <= 0) return;
    
    GameState *game_system = gameState;
    Map *map = &(game_system->map);
    
    // Make color semi-transparent for better visibility
    Color cellColor = ColorAlpha(color, 0.4f);
    
    // Draw each cell in the path
    for (int i = 0; i < pathLength; i++)
    {
        // Calculate cell position and dimensions
        float cellX = (path[i].second * map->tileWidth) * map->scale;
        float cellY = (path[i].first * map->tileHeight) * map->scale;
        float cellWidth = map->tileWidth * map->scale;
        float cellHeight = map->tileHeight * map->scale;
        
        // Draw filled rectangle with borders
        DrawRectangle(cellX, cellY, cellWidth, cellHeight, cellColor);
        
        // Draw cell border with more solid color
        DrawRectangleLines(cellX, cellY, cellWidth, cellHeight, color);
    }
}

/**
 * drawEnemyPaths - Draws paths from all enemies to the player's surrounding cells
 */
void drawEnemyPaths()
{
    GameState *game_system = gameState;
    if (!game_system || game_system->numOfPlayers <= 0) return;
    
    Map *map = &(game_system->map);
    Player *player = &(game_system->players[0]);
    
    // Convert player position to grid coordinates
    Vector2 playerPos = player->object.transform.position;
    int colliderWidth = player->object.collider.bounds.width;
    int colliderHeight = player->object.collider.bounds.height;
    int playerRow = (int)((playerPos.y + colliderHeight / 2) / (map->tileHeight * map->scale));
    int playerCol = (int)((playerPos.x + colliderWidth / 2) / (map->tileWidth * map->scale));
    
    // Define directions for surrounding cells (8 directions)
    int dx[] = {-1, 0, 1, 0, -1, -1, 1, 1}; 
    int dy[] = {0, -1, 0, 1, -1, 1, -1, 1};
    
    // Find all walkable cells surrounding the player
    CoordPair surroundingCells[8]; // Maximum 8 surrounding cells
    int numSurroundingCells = 0;
    
    for (int i = 0; i < 8; i++) {
        int newRow = playerRow + dy[i];
        int newCol = playerCol + dx[i];
        
        // Check if cell is valid and walkable
        if (isValid(newRow, newCol, map->numOfRows, map->numOfCols) && isWalkable(newRow, newCol)) {
            surroundingCells[numSurroundingCells].first = newRow;
            surroundingCells[numSurroundingCells].second = newCol;
            numSurroundingCells++;
        }
    }
    
    // If no surrounding cells are available, use the player's cell
    if (numSurroundingCells == 0) {
        surroundingCells[0].first = playerRow;
        surroundingCells[0].second = playerCol;
        numSurroundingCells = 1;
    }
    
    Color colors[] = {RED, GREEN, BLUE, PURPLE, ORANGE, PINK};
    
    // For each enemy, calculate and draw path to closest surrounding cell
    for (int i = 0; i < game_system->numOfEnemies; i++)
    {
        Enemy *enemy = &(game_system->enemies[i]);
        Vector2 enemyPos = enemy->object.transform.position;
        colliderWidth = enemy->object.collider.bounds.width;
        colliderHeight = enemy->object.collider.bounds.height;
        
        // Convert enemy position to grid coordinates
        int enemyRow = (int)((enemyPos.y + colliderHeight / 2) / (map->tileHeight * map->scale));
        int enemyCol = (int)((enemyPos.x + colliderWidth / 2) / (map->tileWidth * map->scale));
        CoordPair enemyCoord = {.first = enemyRow, .second = enemyCol};
        
        // Find path to each surrounding cell and keep the shortest one
        CoordPair *shortestPath = NULL;
        int shortestPathLength = 0;
        
        for (int j = 0; j < numSurroundingCells; j++) {
            int pathLength = 0;
            CoordPair *path = aStarSearch(enemyCoord, surroundingCells[j], &pathLength);
            
            if (path && (shortestPath == NULL || pathLength < shortestPathLength)) {
                // Free previous path if it exists
                if (shortestPath) {
                    free(shortestPath);
                }
                
                shortestPath = path;
                shortestPathLength = pathLength;
            } else if (path) {
                // Free unused paths
                free(path);
            }
        }
        
        if (shortestPath) {
            // Draw path with a unique color per enemy
            Color pathColor = colors[i % 6];
            drawPath(shortestPath, shortestPathLength, pathColor);
            free(shortestPath);
        }
    }
}

/**
 * drawTileTexture - draws a tile texture at the specified position
 * 
 * @param tileIdx The index of the tile to draw
 * @param dest The destination rectangle where the tile will be drawn
 * @param origin The origin point for rotation
 * @param rotation The rotation angle in radians
 * @param tint The color tint to apply to the tile
 * @param flipX Whether to flip the tile horizontally
 */
static void drawTileTexture(int tileIdx, Rectangle dest, Vector2 origin,
                            float rotation, Color tint, bool flipX) {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);

  char buffer[256];
  char *tileName = map->tilesMapper.tileInfo[tileIdx].filename;
  strncpy(buffer, tileName, strlen(tileName));

  char *fileName = strtok(buffer, ".");
  drawAtlasSpritePro(fileName, dest, origin, rotation, tint, flipX);
}

/**
 * getTileType - gets the type of a tile based on its ID.
 * 
 * @param tileId The ID of the tile to get the type for
 * 
 * @return The TileType of the tile, or TILE_TERRAIN if the ID is invalid
 */
TileType getTileType(int tileId) {
  GameState *game_system = gameState;
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);
  
  if (tileId < 0 || tileId >= MAX_TILES_NUM) {
    return TILE_TERRAIN; // Default fallback
  }
  
  if (tiles_mapper->tileInfo[tileId].filename != NULL) {
    return tiles_mapper->tileInfo[tileId].type;
  }

  fprintf(stderr, "Error: Tile ID %d does not have a valid filename.\n", tileId);
  exit(EXIT_FAILURE);
}

/**
 * getTileProperties - returns the properties bitflags of a tile
 * 
 * @param tileId The ID of the tile to get properties for
 * 
 * @return The TileProperties bitflags for the tile, or TILE_PROP_NONE if invalid
 */
TileProperties getTileProperties(int tileId) {
  GameState *game_system = gameState;
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);
  
  if (tileId < 0 || tileId >= MAX_TILES_NUM) {
    return TILE_PROP_NONE;
  }
  
  if (tiles_mapper->tileInfo[tileId].filename != NULL) {
    return tiles_mapper->tileInfo[tileId].properties;
  }
  
  return TILE_PROP_NONE;
}

/**
 * isPropertySet - checks if a specific property is set for a tile ID
 * 
 * @param tileProperties The properties bitflags of the tile
 * @param property The property to check
 * 
 * @return true if the property is set, false otherwise
 */
bool isPropertySet(TileProperties tileProperties, TileProperties property) {
  return (tileProperties & property) == property;
}

/**
 * isTileWalkable - checks if a tile is walkable based on its properties
 * 
 * @param tileId The ID of the tile to check
 * 
 * @return true if the tile is walkable, false otherwise
 */
bool isTileWalkable(int tileId) {
  TileProperties props = getTileProperties(tileId);
  TileType type = getTileType(tileId);
  
  if (props & TILE_PROP_WALKABLE) return true;
  if (props & TILE_PROP_SOLID) return false;
  
  return type != TILE_WALL;
}

/**
 * isTileCollectible - checks if a tile is collectible
 * 
 * @param tileId The ID of the tile to check
 * 
 * @return true if the tile is collectible, false otherwise
 */
bool isTileCollectible(int tileId) {
  TileProperties props = getTileProperties(tileId);
  return (props & TILE_PROP_COLLECTIBLE) || getTileType(tileId) == TILE_PICKABLE;
}

/**
 * isTileInteractive - checks if a tile is interactive
 * 
 * @param tileId The ID of the tile to check
 * 
 * @return true if the tile is interactive, false otherwise
 */
bool isTileInteractive(int tileId) {
  TileProperties props = getTileProperties(tileId);
  return (props & TILE_PROP_INTERACTIVE) || getTileType(tileId) == TILE_INTERACTABLE;
}

/**
 * isWalkable - checks if a specific cell in the map is walkable
 * 
 * @param row The row index of the cell
 * @param col The column index of the cell
 * 
 * @return true if the cell is walkable, false otherwise
 */
bool isWalkable(int row, int col) {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);
  
  if (row < 0 || row >= map->numOfRows || col < 0 || col >= map->numOfCols) {
    return false;
  }
  
  // Check wall layer (LAYER_WALLS = 1)
  int wallTile = map->mapIds[row][col][LAYER_WALLS];
  if (wallTile != -1 && !isTileWalkable(wallTile)) {
    return false;
  }
  
  // Check for closed doors or non-walkable interactables
  int interactable = map->mapIds[row][col][LAYER_INTERACTABLE];
  if (interactable != -1 && !isTileWalkable(interactable)) {
    return false;
  }
  
  return true;
}

/**
 * getInteractableAt - gets the interactable tile ID at a specific position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * 
 * @return The interactable tile ID, or -1 if no interactable exists at that position
 */
int getInteractableAt(int row, int col) {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);
  
  if (row < 0 || row >= map->numOfRows || col < 0 || col >= map->numOfCols) {
    return -1;
  }
  
  return map->mapIds[row][col][LAYER_INTERACTABLE];
}

/**
 * getPickableAt - gets the pickable item ID at a specific position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * 
 * @return The pickable item ID, or -1 if no pickable exists at that position
 */
int getPickableAt(int row, int col) {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);
  
  if (row < 0 || row >= map->numOfRows || col < 0 || col >= map->numOfCols) {
    return -1;
  }
  
  return map->mapIds[row][col][LAYER_PICKABLE];
}

/**
 * removePickable - removes a pickable item from the map at the specified position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 */
void removePickable(int row, int col) {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);
  
  if (row >= 0 && row < map->numOfRows && col >= 0 && col < map->numOfCols) {
    map->mapIds[row][col][LAYER_PICKABLE] = -1;
  }
}

/**
 * placePickable - places a pickable item at the specified position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * @param itemId The ID of the item to place
 */
void placePickable(int row, int col, int itemId) {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);
  
  if (row >= 0 && row < map->numOfRows && col >= 0 && col < map->numOfCols) {
    map->mapIds[row][col][LAYER_PICKABLE] = itemId;
  }
}

/**
 * toggleInteractable - toggles an interactable tile at the specified position
 * 
 * @param row The row index of the tile
 * @param col The column index of the tile
 * @param playerInventory The player's inventory to check for required items
 */
void toggleInteractable(int row, int col, CollectibleItem playerInventory[]) {
  GameState *game_system = gameState;
  Map *map = &(game_system->map);
  
  if (row < 0 || row >= map->numOfRows || col < 0 || col >= map->numOfCols) {
    return;
  }
  
  int currentTileId = map->mapIds[row][col][LAYER_INTERACTABLE];
  if (currentTileId == -1) return;
  
  InteractableMapping *mapping = getInteractableMapping(currentTileId);

  if (!canInteractWith(currentTileId, playerInventory)) {
    printf("Cannot interact with tile %d - missing required item %d\n", 
           currentTileId, mapping->requiredItem);
    return;
  }

  int nextTileId = getInteractableNextState(currentTileId);
  map->mapIds[row][col][LAYER_INTERACTABLE] = nextTileId;

  // Spawn loot if this was a single-use interactable
  if (mapping->behavior == INTERACT_ONCE || mapping->behavior == INTERACT_CONDITIONAL) {
    spawnLootFromMapping(row, col, mapping);
  }
}

// INTERACTABLE MAPPING FUNCTIONS
InteractableMapping* getInteractableMapping(int tileId) {
  GameState *game_system = gameState;
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);
  for (int i = 0; i < tiles_mapper->numInteractableMappings; i++) {
    if (tiles_mapper->interactableMappings[i].tileId == tileId) {
      return &tiles_mapper->interactableMappings[i];
    }
  }
  
  return NULL;
}

bool canInteractWith(int tileId, CollectibleItem playerInventory[]) {
  InteractableMapping *mapping = getInteractableMapping(tileId);
  if (!mapping) return false;
  printf("Checking interaction for tile %d (required item: %d)\n", 
         tileId, mapping->requiredItem);
  if (mapping->requiredItem == -1) return true;
  
  for (int i = 0; i < MAX_COLLECTED_ITEMS; i++) {
    if (playerInventory[i].tileId == mapping->requiredItem) {
      printf("Player has required item %d for tile %d\n", 
             mapping->requiredItem, tileId);
      return true;
    }
  }

  return false;
}

int getInteractableNextState(int currentTileId) {
  InteractableMapping *mapping = getInteractableMapping(currentTileId);
  if (!mapping) return currentTileId;
  
  switch (mapping->behavior) {
    case INTERACT_TOGGLE:
      if (mapping->stateCount > 0) {
        return mapping->nextStates[0];
      }
      break;
    case INTERACT_ONCE:
      if (mapping->stateCount > 0) {
        return mapping->nextStates[0];
      }
      break;
    case INTERACT_CYCLE:
      for (int i = 0; i < mapping->stateCount; i++) {
        if (mapping->nextStates[i] == currentTileId) {
          return mapping->nextStates[(i + 1) % mapping->stateCount];
        }
      }
      if (mapping->stateCount > 0) {
        return mapping->nextStates[0];
      }
      break;
    case INTERACT_CONDITIONAL:
      if (mapping->stateCount > 0) {
        return mapping->nextStates[0];
      }
      break;
  }
  
  return currentTileId;
}

void spawnLootFromMapping(int row, int col, InteractableMapping *mapping) {
  if (!mapping || mapping->lootCount == 0) return;
  GameState *game_system = gameState;
  Map *map = &(game_system->map);
  int dx[] = {0, 1, 0, -1};
  int dy[] = {1, 0, -1, 0};
  int possiblePositionsCount = 1;
  int possiblePositionsIndices[9] = {-1};
  possiblePositionsIndices[0] = row * map->numOfCols + col;

  // Check adjacent cells for possible loot positions
  for (int i = 0; i < 4; i++) {
    int newRow = row + dy[i];
    int newCol = col + dx[i];
    
    if (newRow >= 0 && newRow < map->numOfRows &&
        newCol >= 0 && newCol < map->numOfCols) {
      if (getPickableAt(newRow, newCol) == -1 && isWalkable(newRow, newCol)) {
        possiblePositionsCount++;
        possiblePositionsIndices[possiblePositionsCount - 1] = newRow * map->numOfCols + newCol;
      }
    }
  }

  // Spawn loot items at random positions
  for (int i = 0; i < mapping->lootCount && possiblePositionsCount; i++) {
    int lootItem = mapping->lootTable[i];
    if (lootItem != -1) {
      int randomIndex = GetRandomValue(0, possiblePositionsCount - 1);
      int randomPosition = possiblePositionsIndices[randomIndex];
      int targetRow = randomPosition / map->numOfCols;
      int targetCol = randomPosition % map->numOfCols;

      placePickable(targetRow, targetCol, lootItem);
      possiblePositionsIndices[randomIndex] = possiblePositionsIndices[possiblePositionsCount - 1];
      possiblePositionsIndices[possiblePositionsCount - 1] = -1;
      possiblePositionsCount--;
    }
  }
}
