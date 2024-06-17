#include "draw.h"
#include "anime.h"
#include "atlas.h"
#include <raylib.h>

void DrawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX) {
  GameState *game = getGameSystemInstance();
  AtlasImage image = getAtlasImage(filename);

  if (image.filename != NULL) {
    if (flipX) {
      image.source.width *= -1;
    }

    DrawTexturePro(game->atlasTexture, image.source, dest, origin, rotation,
                   tint);
  }
}

static void drawPlayers(GameState *game) {
  Player *players = game->players;
  int player_num = game->numOfPlayers;

  SpriteAnimation idle = createSpriteAnimation(6,
                                               (char *[]){
                                                   "Meow-Knight_Idle_0_0",
                                                   "Meow-Knight_Idle_1_0",
                                                   "Meow-Knight_Idle_2_0",
                                                   "Meow-Knight_Idle_3_0",
                                                   "Meow-Knight_Idle_4_0",
                                                   "Meow-Knight_Idle_5_0",
                                               },
                                               6, true);

  SpriteAnimation walk = createSpriteAnimation(8,
                                               (char *[]){
                                                   "Meow-Knight_Run_0_0",
                                                   "Meow-Knight_Run_1_0",
                                                   "Meow-Knight_Run_2_0",
                                                   "Meow-Knight_Run_3_0",
                                                   "Meow-Knight_Run_4_0",
                                                   "Meow-Knight_Run_5_0",
                                                   "Meow-Knight_Run_6_0",
                                                   "Meow-Knight_Run_7_0",
                                               },
                                               8, true);

  while (player_num--) {
    Vector2 pos = players->position;
    bool flip = (players->drawDirection == -1) ? true : false;
    if (players->isMoving) {
      drawSpriteAnimationPro(&walk, (Rectangle){pos.x, pos.y, 64, 64},
                             (Vector2){0, 0}, 0, WHITE, flip);
    } else {
      drawSpriteAnimationPro(&idle, (Rectangle){pos.x, pos.y, 64, 64},
                             (Vector2){0, 0}, 0, WHITE, flip);
    }

    players++;
  }

  disposeSpriteAnimation(&idle);
  disposeSpriteAnimation(&walk);
}

static void drawBullets(GameState *game) {
  int x = 320, y = 96;
  int bulletNum = game->num_of_bullets;
  Bullet *bullets = game->bullets;

  SpriteAnimation fireAnime = createSpriteAnimation(6,
                                                    (char *[]){
                                                        "fire_1_0_0",
                                                        "fire_1_0_1",
                                                        "fire_1_0_2",
                                                        "fire_1_0_3",
                                                        "fire_1_0_4",
                                                        "fire_1_0_5",
                                                    },
                                                    12, true);

  while (bulletNum--) {
    Vector2 pos = bullets->position;
    Rectangle dest = {pos.x, pos.y, 32, 32};
    drawSpriteAnimationPro(&fireAnime, dest, (Vector2){0, 0}, 0, WHITE, false);
    bullets->position.x += bullets->bulletSpeed * cos(bullets->angle * DEG2RAD);
    bullets->position.y += bullets->bulletSpeed * sin(bullets->angle * DEG2RAD);
    bullets++;
  }

  disposeSpriteAnimation(&fireAnime);
}

void drawScene() {
  GameState *gameSystemInstance = getGameSystemInstance();
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));

  drawPlayers(gameSystemInstance);

  // TODO: Delete Me later
  // Example for using atlas
  // DrawAtlasSpritePro("vampire_1", (Rectangle){0, 0, 64, 64}, (Vector2){0, 0},
  // 0, WHITE, false);

  // TODO: Delete Me later
  SpriteAnimation vampire = createSpriteAnimation(4,
                                                  (char *[]){
                                                      "vampire_1",
                                                      "vampire_2",
                                                      "vampire_3",
                                                      "vampire_4",
                                                  },
                                                  6, true);

  SpriteAnimation slime = createSpriteAnimation(4,
                                                (char *[]){
                                                    "slime_1_0",
                                                    "slime_1_1",
                                                    "slime_1_2",
                                                    "slime_1_3",
                                                },
                                                6, true);

  drawSpriteAnimationPro(&vampire, (Rectangle){128, 128, 64, 64},
                         (Vector2){0, 0}, 0, WHITE, false);

  drawSpriteAnimationPro(&slime,
                         (Rectangle){SCREEN_WIDTH - 128 - 64, 128, 64, 64},
                         (Vector2){0, 0}, 0, WHITE, true);

  drawBullets(gameSystemInstance);

  disposeSpriteAnimation(&slime);
  disposeSpriteAnimation(&vampire);
  EndDrawing();
}
