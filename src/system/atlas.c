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
  gameState->atlasTexture = LoadTexture("./src/resources/gfx/atlas.png");
  loadAtlasData();
}

void loadAtlasData() {
  char *atlasData = LoadFileText("./src/resources/gfx/atlas.rtpa");

  char *line = strtok(atlasData, "\n");
  AtlasImage *images = gameState->atlasImages;
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
             filename, &originX, &originY, &x, &y, &width, &height, &padding,
             &trim, &trimX, &trimY, &trixW, &trimH);
      AtlasImage *sprite = malloc(sizeof(AtlasImage));
      sprite->filename = filename;
      sprite->origin.x = originX;
      sprite->origin.y = originY;
      sprite->source = (Rectangle){x, y, width, height};
      sprite->next = NULL;
      if (images == NULL) {
        gameState->atlasImages = sprite;
        images = sprite;
      } else {
        images->next = sprite;
        images = sprite;
      }
    }
    line = strtok(NULL, "\n");
  }
  free(line);

  UnloadFileText(atlasData);
}

AtlasImage getAtlasImage(char *filename) {
  AtlasImage *images = gameState->atlasImages;
  while (images != NULL) {
    if (strcmp(images->filename, filename) == 0) {
      return *images;
    }
    images = images->next;
  }
  return (AtlasImage){0};
}
