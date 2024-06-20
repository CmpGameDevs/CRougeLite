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
#include <unistd.h>

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

  loadTilesMapper();
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
  map->tileWidth = 16;
  map->tileHeight = 16;
  map->currentLevelPath = "./src/resources/gfx/map-assets/level_one/level_one_map.csv";

  map->textures = malloc(map->tilesMapper.numOfTiles * sizeof(Texture2D *));
  for (int i = 0; i < map->tilesMapper.numOfTiles; i++)
    map->textures[i] = NULL;

  parseLevelFile();
}

static void resetMapIds() {
  Game_System *game_system = getGameSystemInstance();
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
  Game_System *game_system = getGameSystemInstance();
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

void drawMap() {
  Game_System *game_system = getGameSystemInstance();
  Map *map = &(game_system->map);
  TilesMapper *tilesMapper = &(game_system->map.tilesMapper);

  Rectangle src = {0.0f, 0.0f, (float)map->tileWidth, (float)map->tileHeight};

  for (int row = 0; row < map->numOfRows; row++) {
    for (int col = 0; col < map->numOfCols; col++) {
      Rectangle dest = {(float)(col * map->tileWidth) * 2.0f, (float)(row * map->tileHeight) * 2.0f,
        map->tileWidth * 2.0f, map->tileHeight * 2.0f};  //Scaling the drawn texture by 2

      int idIdx = 0;
      int tileId = map->mapIds[row][col][idIdx++];
      while (idIdx < 5 && tileId != -1) {
        loadTileTexture(tileId);
        DrawTexturePro(*(map->textures[tileId]), src, dest, (Vector2){0, 0}, (float)0, WHITE);
        tileId = map->mapIds[row][col][idIdx++];
      }
    }
  }
}

static void loadTileTexture(int tileIdx) {
  Game_System *game_system = getGameSystemInstance();
  Map *map = &(game_system->map);

  if (map->textures[tileIdx] != NULL)
    return;

  char buffer[256];
  sprintf(buffer, "./src/resources/gfx/assets-prepare/%s", map->tilesMapper.mapper[tileIdx]);

  Texture2D texture = LoadTexture(buffer);
  map->textures[tileIdx] = &texture;
}
