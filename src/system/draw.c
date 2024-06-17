#include "draw.h"
#include "anime.h"
#include "atlas.h"
#include <raylib.h>

void DrawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX) {
  AtlasImage image = getAtlasImage(filename);

  if (image.filename != NULL) {
    if (flipX) {
      image.source.width *= -1;
    }

    DrawTexturePro(gameState->atlasTexture, image.source, dest, origin,
                   rotation, tint);
    DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, RED);
  }
}

static void drawPlayers() {
  Player *players = gameState->players;
  int player_num = gameState->numOfPlayers;

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
    Vector2 pos = players->object.rigidBody.position;
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

static void drawEnemies() {
  Enemy *enemies = gameState->enemies;
  int enemy_num = gameState->numOfEnemies;

  if (enemies == NULL)
    return;

  while (enemy_num--) {

    char *frames[4];
    if (enemies->type == E_CIVILIAN) {
      frames[0] = "vampire_1";
      frames[1] = "vampire_2";
      frames[2] = "vampire_3";
      frames[3] = "vampire_4";
    } else if (enemies->type == E_FARMER) {
      frames[0] = "slime_1_0";
      frames[1] = "slime_1_1";
      frames[2] = "slime_1_2";
      frames[3] = "slime_1_3";
    }

    SpriteAnimation idle = createSpriteAnimation(4, frames, 6, true);

    Vector2 pos = enemies->object.rigidBody.position;
    bool flip = (enemies->drawDirection == -1) ? true : false;
    drawSpriteAnimationPro(&idle, (Rectangle){pos.x, pos.y, 64, 64},
                           (Vector2){0, 0}, 0, WHITE, flip);

    disposeSpriteAnimation(&idle);
    enemies++;
  }

  // disposeSpriteAnimation(&walk);
}

static void drawBullets() {
  int x = 320, y = 96;
  int bulletNum = gameState->numOfCombatActions;
  // Bullet *bullets = gameState->bullets;

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

  // while (bulletNum--) {
  //   Vector2 pos = bullets->startPosition;
  //   Rectangle dest = {pos.x, pos.y, 32, 32};
  //   drawSpriteAnimationPro(&fireAnime, dest, (Vector2){0, 0}, 0, WHITE,
  //   false);
  //   // bullets->position.x += bullets->bulletSpeed * cos(bullets->angle *
  //   // DEG2RAD); bullets->position.y += bullets->bulletSpeed *
  //   // sin(bullets->angle * DEG2RAD);
  //   bullets++;
  // }

  disposeSpriteAnimation(&fireAnime);
}
static bool checkCollision(Rectangle rect1, Rectangle rect2) {
  // collision x-axis?
  bool collisionX =
      rect1.x + rect1.width >= rect2.x && rect2.x + rect2.width >= rect1.x;
  // collision y-axis?
  bool collisionY =
      rect1.y + rect1.height >= rect2.y && rect2.y + rect2.height >= rect1.y;
  // collision only if on both axes
  return collisionX && collisionY;
}

// static void bulletCollision() {
//   for (int i = 0; i < gameState->numOfCombatActions; i++) {
//     for (int j = 0; j < gameState->num_of_enemies; j++) {
//       if (checkCollision((Rectangle){gameState->bullets[i].position.x,
//                                      gameState->bullets[i].position.y,
//                                      gameState->bullets[i].body.width,
//                                      gameState->bullets[i].body.height},
//                          (Rectangle){gameState->enemies[j].position.x,
//                                      gameState->enemies[j].position.y,
//                                      gameState->enemies[j].body.width,
//                                      gameState->enemies[j].body.height})) {
//         gameState->bullets[i] =
//             gameState->bullets[gameState->num_of_bullets - 1];
//         gameStateState->num_of_bullets--;
//
//         if (gameState->enemies[j].health > 0)
//           gameState->enemies[j].health -= gameState->bullets[i].bulletDamage;
//         else
//           gameState->enemies[j].health = 0;
//
//         // WTF???? Who wrote this shit??
//         if (gameState->enemies[j].health == 0) {
//           gameState->enemies[j] = gameState->enemies[game->num_of_enemies -
//           1]; gameState->num_of_enemies--;
//         }
//       }
//     }
//   }
// }

void drawScene() {
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));

  drawPlayers();

  // TODO: Delete Me later
  // Example for using atlas
  // DrawAtlasSpritePro("vampire_1", (Rectangle){0, 0, 64, 64}, (Vector2){0, 0},
  // 0, WHITE, false);

  // TODO: Delete Me later
  // bulletCollision();

  drawEnemies();

  drawBullets();

  EndDrawing();
}
