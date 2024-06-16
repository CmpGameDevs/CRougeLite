#include "draw.h"
#include "atlas.h"
#include <raylib.h>

void DrawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX) {
  Game_System *game = getGameSystemInstance();
  AtlasImage image = getAtlasImage(filename);

  if (image.filename != NULL) {
    if (flipX) {
      image.source.width *= -1;
    }

    DrawTexturePro(game->atlasTexture, image.source, dest, origin, rotation,
                   tint);
  }
}

static void drawPlayers(Game_System *game) {
  Player *players = game->players;
  int player_num = game->num_of_players;

  while (player_num--) {
    Vector2 pos = players->position;
    bool flip = (players->drawDirection == -1) ? true : false;
    DrawAtlasSpritePro("slime_1_2", (Rectangle){pos.x, pos.y, 64, 64},
                       (Vector2){0, 0}, 0, WHITE, flip);

    players++;
  }
}

static void drawBullets(Game_System *game) {
  int x = 320, y = 96;
  int bulletNum = game->num_of_bullets;
  Bullet *bullets = game->bullets;
  while (bulletNum--) {
    Vector2 pos = bullets->position;
    Rectangle dest = {pos.x, pos.y, 16, 16};
    DrawAtlasSpritePro("tile_7_7", dest, (Vector2){0, 0}, 0, WHITE, false);
    bullets->position.x += bullets->bulletSpeed * cos(bullets->angle * DEG2RAD);
    bullets->position.y += bullets->bulletSpeed * sin(bullets->angle * DEG2RAD);
    bullets++;
  }
}

void drawScene() {
  Game_System *gameSystemInstance = getGameSystemInstance();
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));

  drawPlayers(gameSystemInstance);
  drawBullets(gameSystemInstance);

  // TODO: Delete Me later
  // Example for using atlas
  DrawAtlasSpritePro("vampire_1", (Rectangle){0, 0, 64, 64}, (Vector2){0, 0}, 0,
                     WHITE, false);

  EndDrawing();
}
