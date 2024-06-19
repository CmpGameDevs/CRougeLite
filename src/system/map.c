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
void initTilesMapper() {
  Game_System *game_system = getGameSystemInstance();
  TilesMapper *tiles_mapper = &(game_system->map.tilesMapper);

  for (int i = 0; i < MAX_TILES_NUM; i++) {
    tiles_mapper->mapper[i] = NULL;
  }

  tiles_mapper->path = "./src/resources/gfx/map-assets/mapper";
  tiles_mapper->num_of_tiles = 0;
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

    tiles_mapper->num_of_tiles += 1;

    line = strtok(NULL, "\n");
  }

  UnloadFileText(file_content);
  return (0);
}
