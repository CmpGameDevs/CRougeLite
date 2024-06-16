#include "draw.h"
#include "anime.h"
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

  SpriteAnimation anim = createSpriteAnimation(4,
                                               (char *[]){
                                                   "slime_0_0",
                                                   "slime_0_1",
                                                   "slime_0_2",
                                                   "slime_0_3",
                                               },
                                               6, true);

  while (player_num--) {
    Vector2 pos = players->position;
    bool flip = (players->drawDirection == -1) ? true : false;
    drawSpriteAnimationPro(&anim, (Rectangle){pos.x, pos.y, 64, 64},
                           (Vector2){0, 0}, 0, WHITE, flip);

    players++;
  }

  disposeSpriteAnimation(&anim);
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
  // DrawAtlasSpritePro("vampire_1", (Rectangle){0, 0, 64, 64}, (Vector2){0, 0},
  // 0, WHITE, false);

  // TODO: Delete Me later
  SpriteAnimation anim = createSpriteAnimation(4,
                                               (char *[]){
                                                   "vampire_1",
                                                   "vampire_2",
                                                   "vampire_3",
                                                   "vampire_4",
                                               },
                                               6, true);

  drawSpriteAnimationPro(&anim, (Rectangle){0, 0, 64, 64}, (Vector2){0, 0}, 0,
                         WHITE, false);

  disposeSpriteAnimation(&anim);

  EndDrawing();
}
