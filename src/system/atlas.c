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
  loadAtlasData();
}

void loadAtlasData() {
  Game_System *game = getGameSystemInstance();
  char *atlasData = LoadFileText("./src/resources/gfx/atlas.rtpa");

  char *line = strtok(atlasData, "\n");
  AtlasImage *images = game->atlasImages;
  while (line != NULL) {

    if (line[0] == '#' || line[0] == 'a') {
      line = strtok(NULL, "\n");
      continue;
    }

    if (line[0] == 's') {
      char *filename = malloc(FILENAME_MAX);
      int originX, originY, width, height, x, y, padding, trim, trimX, trimY,
          trixW, trimH;
      sscanf(line, "%s %s %d %d %d %d %d %d %d %d %d %d %d %d", filename,
             filename, &originX, &originY, &width, &height, &x, &y, &padding,
             &trim, &trimX, &trimY, &trixW, &trimH);
      AtlasImage *sprite = malloc(sizeof(AtlasImage));
      sprite->filename = filename;
      sprite->origin.x = originX;
      sprite->origin.y = originY;
      sprite->source = (Rectangle){x, y, width, height};
      sprite->next = NULL;
      if (images == NULL) {
        game->atlasImages = sprite;
        images = sprite;
      } else {
        images->next = sprite;
        images = sprite;
      }
    }

    printf("%s\n", line);

    line = strtok(NULL, "\n");
  }
  free(line);

  UnloadFileText(atlasData);
}
