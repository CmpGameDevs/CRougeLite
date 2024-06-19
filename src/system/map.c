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
#include <raylib.h>
#include <string.h>

/**
 * initTilesMapper - initilizes the TilesMapper mapper with NULL,
 * and set path with mapper path.
*/
static void initTilesMapper() {
  Game_System *game_system = getGameSystemInstance();
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);

  for (int i = 0; i < MAX_TILES_NUM; i++) {
    tiles_mapper->mapper[i] = NULL;
  }

  tiles_mapper->path = "./src/resources/gfx/map-assets/mapper";
  tiles_mapper->numOfTiles = 0;
}

/**
 * loadTilesMapper - loades the tiles mapper file at the set path into memory.
 *
 * return: 0 if no errors, 1 if an error occured.
*/
int loadTilesMapper() {
  Game_System *game_system = getGameSystemInstance();
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);

  char *file_content = LoadFileText(tiles_mapper->path);
 
  char *line = strtok(file_content, "\n");

  while (line) {
    int id;
    char tile_name[128];

    if (sscanf(line, "%d %s", &id, tile_name) != 2) {
      printf("Error occured while reading the file\n");
      UnloadFileText(file_content);
      return (1);
    }

    tiles_mapper->mapper[id] = malloc(strlen(tile_name) * sizeof(char));
    if (tiles_mapper->mapper[id] == NULL) {
      printf("Error occured while allocating memory\n");
      UnloadFileText(file_content);
      return (1);
    }

    strcpy(tiles_mapper->mapper[id], tile_name);

    tiles_mapper->numOfTiles += 1;

    line = strtok(NULL, "\n");
  }

  UnloadFileText(file_content);
  return (0);
}

/**
 * initMap - initilizes the map and the tilesMapper.
*/
void initMap() {
  Game_System *game_system = getGameSystemInstance();
  Map *map = &(game_system->map);
  
  initTilesMapper();

  map->loaded = false;
  map->currentLevel = 0;
  map->numOfRows = 0;
  map->numOfCols = 0;
  map->currentLevelPath = "./src/resources/gfx/map-assets/level_one/level_one_map.csv";
  
  parseLevelFile();
}

/**
 * parseLevelFile - parses the current level file into the memory
*/
void parseLevelFile() {
  Game_System *game_system = getGameSystemInstance();
  Map *map = &(game_system->map);

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
